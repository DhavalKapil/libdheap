/**
 * This header file manages the configuration of libdheap
 */

#ifndef _CONFIG_GUARD_H
#define _CONFIG_GUARD_H

/**
 * Configurations
 * These can be configured by setting the corresponding
 * environment variable.
 *
 * Whether to output debugging statements or not (by default 0)
 * LIBDHEAP_DEBUG = 0/1
 *
 * Whether to crash the program (exit) on finding a memory corruption
 * or not (by default 0)
 * LIBDHEAP_EXIT_ON_ERROR
 */

/**
 * Returns LIBDHEAP_DEBUG
 */
int is_libdheap_debug (void);

/**
 * Returns LIBDHEAP_EXIT_ON_ERROR
 */
int is_libdheap_exit_on_error (void);

#endif
