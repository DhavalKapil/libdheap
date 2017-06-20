#include "libdheap.h"
#include "backtrace.h"
#include "libc_malloc.h"
#include "chunk.h"
#include "canary.h"
#include "display.h"
#include "config.h"

#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define BACKTRACE_LEN 10

// The storage arena for all allocated chunks
chunks_storage chunks;
// Whether the initialization is done or not. Needed because
// initialize it called many times (mostly unpredictable)
static int libdheap_initialized = 0;
// FILE stream pointer for logging
FILE *libdheap_log;
// Mutex lock
pthread_mutex_t libdheap_mutex = PTHREAD_MUTEX_INITIALIZER;

void initialize (void) {
  init_chunks(&chunks);
  libdheap_log = stderr;
  libdheap_initialized = 1;
}

void *malloc (size_t size) {
  void *ptr;
  struct chunk *ch;
  void *backtrace[BACKTRACE_LEN];

  /**
   * Initialization
   */
  if (!libdheap_initialized) {
    initialize();
  }

  display_debug(libdheap_log, "malloc(%d)", (int)size);

  pthread_mutex_lock(&libdheap_mutex);
  // Adjust size for canary
  size = get_padded_size(size);
  ptr = libc_malloc(size);
  display_debug(libdheap_log,
                "Allocating chunk: %p %p",
                (ptr + sizeof(canary)), &(chunks.root));
  if (ptr == NULL) {
    // Memory full
    display_log(libdheap_log,
                "No memory available for chunk, returning NULL");
    pthread_mutex_unlock(&libdheap_mutex);
    return NULL;
  }

  ch = libc_malloc(sizeof(struct chunk));
  if (ch == NULL) {
    // Memory full
    display_log(libdheap_log,
                "No memory available for chunk structure, returning NULL");
    libc_free(ptr);
    pthread_mutex_unlock(&libdheap_mutex);
    return NULL;
  }
  ch->ptr = (struct chunk *)(ptr - sizeof(size_t)); // To accomodate size
  ch->requested_size = size; // Includes padding for canary
  ch->allocated_size = *((size_t *)ch->ptr) & ~(0x1 | 0x2 | 0x4);
  guard_chunk(ch);

  // Check for overlap
  if (check_overlap(&chunks, ch)) {
    // Memory corruption detected
    display_log(libdheap_log,
                "Chunk returned by malloc overlaps with existing chunks!");
    set_backtrace(backtrace, 1, BACKTRACE_LEN);
    print_backtrace(backtrace, BACKTRACE_LEN);
    if (is_libdheap_exit_on_error()) {
      exit(1);
    } else {
      pthread_mutex_unlock(&libdheap_mutex);
      return NULL;
    }
  }
  if (!insert_chunk(&chunks, ch)) {
    display_log(libdheap_log,
                "Couldn't insert in storage, returning NULL");
    libc_free(ptr);
    libc_free(ch);
    pthread_mutex_unlock(&libdheap_mutex);
    return NULL;
  }

  pthread_mutex_unlock(&libdheap_mutex);
  return (ptr + sizeof(canary));
}

void free (void *ptr) {
  void *ch_ptr;
  struct node *n;
  struct chunk *ch;
  void *backtrace[BACKTRACE_LEN];

  /**
   * Initialization
   */
  if (!libdheap_initialized) {
    initialize();
  }

  display_debug(libdheap_log, "free(%p)", ptr);
  if (ptr == NULL) {
    return;
  }
  display_debug(libdheap_log, "Freeing chunk: %p", ptr);
  pthread_mutex_lock(&libdheap_mutex);

  ptr -= sizeof(canary);
  ch_ptr = ptr - sizeof(size_t); // To accomodate size
  n = find_node(&chunks, ch_ptr);
  if (n == NULL) {
    // Freeing invalid chunk
    // Possibly a double free
    display_log(libdheap_log, "Freeing non allocated chunk!");
    set_backtrace(backtrace, 1, BACKTRACE_LEN);
    print_backtrace(backtrace, BACKTRACE_LEN);
    if (is_libdheap_exit_on_error()) {
      exit(1);
    } else {
      pthread_mutex_unlock(&libdheap_mutex);
      return;
    }
  }

  ch = n->ch;
  // Check heap canary
  if (!check_canary(ch)) {
    // Canary modified
    display_log(libdheap_log, "Inconsistent heap canary!");
    set_backtrace(backtrace, 1, BACKTRACE_LEN);
    print_backtrace(backtrace, BACKTRACE_LEN);
    if (is_libdheap_exit_on_error()) {
      exit(1);
    } else {
      pthread_mutex_unlock(&libdheap_mutex);
      return;
    }
  }

  remove_chunk(&chunks, ch);

  // Freeing storage related chunks
  libc_free(ch);
  libc_free(ptr);
  pthread_mutex_unlock(&libdheap_mutex);
}

void *calloc (size_t nmemb, size_t size) {
  void *ptr = malloc(nmemb * size);
  if (ptr == NULL) {
    return NULL;
  }
  memset(ptr, 0, nmemb * size);
  return ptr;
}

void *realloc (void *ptr, size_t size) {
  void *new_ptr;
  size_t ptr_size;
  size_t min_size;

  display_debug(libdheap_log, "realloc(%p, %d)", ptr, (int)size);

  if (ptr == NULL) {
    return malloc(size);
  }

  if (size == 0) {
    free(ptr);
    return NULL;
  }

  new_ptr = malloc(size);

  if (new_ptr != NULL) {
    ptr_size = malloc_usable_size(ptr);
    min_size = ptr_size > size ? size : ptr_size;
    memcpy(new_ptr, ptr, min_size);
  }

  free(ptr);
  return new_ptr;
}

size_t malloc_usable_size (void *ptr) {
  // For our purposes, usable size = requested size
  struct node *n;
  void *backtrace[BACKTRACE_LEN];

  /**
   * Initialization
   */
  if (!libdheap_initialized) {
    initialize();
  }

  display_debug(libdheap_log, "malloc_usable_size(%p)", ptr);

  ptr -= sizeof(canary) + sizeof(size_t);
  n = find_node(&chunks, ptr);
  if (n == NULL) {
    display_log(libdheap_log,
                "Invalid chunk passed to malloc_usable_size! %p",
                &(chunks.root));
    set_backtrace(backtrace, 1, BACKTRACE_LEN);
    print_backtrace(backtrace, BACKTRACE_LEN);
    if (is_libdheap_exit_on_error()) {
      exit(1);
    } else {
      return 0;
    }
  }

  return n->ch->requested_size;
}
