/**
 * This implements functions to manage allocated chunks. Internally, they are
 * stored in AVL tree to allow faster lookups and inserts.
 * It is assumed that the chunks do not overlap within this data structure.
 * Therefore, special checks needed to be made while doing insertions.
 */

#ifndef _CHUNK_GUARD_H
#define _CHUNK_GUARD_H

// Forward declaring chunk for canary
struct chunk;

#include "canary.h"

#include <stddef.h>

/**
 * The chunk representation
 * The ptr here points to the begining of the chunk (size in malloc_chunk)
 * Canary is put after requested_size instead of allocated_size to detect
 * more errors. The size of the chunk requested is automatically adjusted
 * to add space for the two canaries.
 */
struct chunk {
  void *ptr;
  size_t requested_size;
  size_t allocated_size;
  canary begin_guard;
  canary end_guard; // At the end of requested size
};

// The node representation
struct node {
  struct chunk *ch;
  int height;
  struct node *left;
  struct node *right;
};

// Another name for a root node
// During balancing, the AVL tree's root will change
// Hence, a double pointer
typedef struct {
  struct node *root;
} chunks_storage;

#define CHUNK_BEGIN(ch) (ch->ptr)
#define CHUNK_END(ch) ((void *)((char *)(ch->ptr) + ch->allocated_size))

/**
 * Initializes storage
 *
 * @param chunks The storage object
 */
void init_chunks (chunks_storage *chunks);

/**
 * Inserts a new chunk
 *
 * @param chunks The storage object
 * @param ch The chunk to be inserted
 *
 * @return 1 if insert is successful
 */
int insert_chunk (chunks_storage *chunks, struct chunk *ch);

/**
 * Removes a chunk
 * Assumes that the chunk is already in storage
 * The caller needs to check for this
 *
 * @param chunks The storage object
 * @param ch The chunk to be removed
 */
void remove_chunk (chunks_storage *chunks, struct chunk *ch);

/**
 * Retrieves a node from the storage corresponding to a pointer
 * The pointer has to be at the start of the node
 *
 * @param chunks The storage object
 * @param ptr The pointer to be searched for
 *
 * @return The node containing that pointer
 */
struct node *find_node (chunks_storage *chunks, void *ptr);

/**
 * Checks whether memory area is overlapping with existing chunks or not
 *
 * @param chunks The storage object
 * @param ch The chunk that needs to be checked for overlap
 *
 * @return 1 if it is overlapping
 */
int check_overlap (chunks_storage *chunks, struct chunk *ch);

/**
 * Debug function used to print contents of AVL tree in 'in order traversal'
 * This function is not even used in DEBUG mode. This is only for testing
 * purposes.
 *
 * @param root The root of the tree
 */
void print_ptrs (struct node *root);

#endif
