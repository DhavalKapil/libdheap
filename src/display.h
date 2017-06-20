/**
 * This handles printing messages to various streams
 */
#ifndef _DISPLAY_GUARD_H
#define _DISPLAY_GUARD_H

#include <stdio.h>

/**
 * Displays error messages
 *
 * @param msg The error message
 */
void display_error (const char *msg, ...);

/**
 * Outputs log to a particular stream
 *
 * @param stream The output file stream
 * @param log The log message
 */
void display_log (FILE *stream, const char *log, ...);

/**
 * Outputs debug statements to a particular stream
 *
 * @param stream The output file stream
 * @param msh The debug message
 */
void display_debug (FILE *stream, const char *msg, ...);

#endif