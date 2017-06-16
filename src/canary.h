/**
 * This implements functionality to generate and test stack canaries.
 * Internally it uses /dev/urandom as the pseudo random generator
 */
#ifndef _CANARY_GUARD_H
#define _CANARY_GUARD_H

#include <stddef.h>

// size_t is generally equal to the size of a (void *) pointer.
// Having canaries of this size generally help in keeping alignment
typedef size_t canary;

#include "chunk.h"

/**
 * Returns the padded size after accomodating required canaries
 *
 * @param size The size requested
 *
 * @return The padded size
 */
size_t get_padded_size (size_t size);

/**
 * Generates a (pseudo)random canary value
 *
 * @return The canary
 */
canary generate_canary ();

/**
 * Adds canary guards to a chunk
 *
 * @param ch The chunk to be guarded
 */
void guard_chunk (struct chunk *ch);

/**
 * Checks whether canary is intact for a chunk or not
 *
 * @param ch The chunk to be checked
 *
 * @return 1 if canary is intact, 0 otherwise
 */
int check_canary (struct chunk *ch);

#endif
