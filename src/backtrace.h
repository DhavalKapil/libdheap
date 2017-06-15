/**
 * This implements functions to print the stack trace of the process
 * to debug errors. The original 'backtrace' defined by GNU uses malloc
 * internally that causes an infinite recursion, so a custom version is written.
 */

#ifndef _BACKTRACE_GUARD_H
#define _BACKTRACE_GUARD_H

/**
 * Fills up an array with trace of function calls
 *
 * @param backtrace The array to fill
 * @param start The starting index to go back in trace
 * @param len The total number of addresses to fill up
 *
 * start=0 will start filling directly from the call to this function
 * Invalid entries are filled with NULL
 */
void set_backtrace (void **backtrace, unsigned int start, unsigned int len);

/**
 * Prints the backtrace uptil a certail length. Stops on NULL
 *
 * @param backtrace The array to print
 * @param len The length of the array
 */
void print_backtrace (void **backtrace, unsigned int len);

#endif
