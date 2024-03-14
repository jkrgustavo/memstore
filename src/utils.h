#ifndef UTILS_H
#define UTILS_H

/* To include the standard headers, define UTIL_STD before including the header like so:
 * #define UTIL_STD
 * #include "utils.h"
 */

#ifdef UTIL_STD
#  include <stdio.h>
#  include <stdlib.h>
#  include <memory.h>
#  include <string.h>
#  include <time.h>
#endif

/* Neat logging macros, requires a format string (except for LOG) */
#define LOG(...) printf(__VA_ARGS__); 

#define INFO_FORTMAT(fmt, ...) printf("[INFO]    " fmt "\n", __VA_ARGS__)
#define INFO(...) INFO_FORTMAT(__VA_ARGS__)

#define DEBUG_FORMAT(fmt, ...) printf("[DEBUG]   " fmt "\n", __VA_ARGS__)
#define DEBUG(...) DEBUG_FORMAT(__VA_ARGS__)

#define ERROR_FORMAT(fmt, ...) printf("[ERROR]   " fmt "\n", __VA_ARGS__)
#define ERROR(...) ERROR_FORMAT(__VA_ARGS__)

typedef unsigned long int u4;
typedef unsigned char u1;

char **process_words(char *buffer);

char *read_file(const char *file_name); 

#endif // !UTILS_H

// Chapter 55
