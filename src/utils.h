#ifndef UTILS_H
#define UTILS_H

/* available:
 * UTIL_STD
 * UTIL_TYPES
 * UTIL_RAYLIB
 * UTIL_ALL
 */

#if defined(UTIL_STD) || defined(UTIL_ALL) 
#   include <stdio.h>
#   include <stdlib.h>
#   include <memory.h>
#   include <string.h>
#   include <time.h>
#   include <stdint.h>
#   include <math.h>
#   include <assert.h>
#endif

/* Neat logging macros, requires a format string (except for LOG) */
#define LOG(...) printf(__VA_ARGS__); 

#define INFO_FORTMAT(fmt, ...) printf("[INFO]    " fmt "\n", __VA_ARGS__)
#define INFO(...) INFO_FORTMAT(__VA_ARGS__)

#define DEBUG_FORMAT(fmt, ...) printf("[DEBUG]   " fmt "\n", __VA_ARGS__)
#define DEBUG(...) DEBUG_FORMAT(__VA_ARGS__)

#define ERROR_FORMAT(fmt, ...) printf("[ERROR]   " fmt "\n", __VA_ARGS__)
#define ERROR(...) ERROR_FORMAT(__VA_ARGS__)

#if defined(UTIL_TYPES) || defined(UTIL_ALL)
#   include <stdint.h>

    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    typedef int8_t  s8;
    typedef int16_t s16;
    typedef int32_t s32;
    typedef int64_t s64;

    typedef float  f32;
    typedef double f64;
#endif

#if defined(UTIL_RAYLIB) || defined(UTIL_ALL)
#   include "../res/raylib/raylib.h"
    typedef Vector2 vec2;
    typedef Vector3 vec3;
#endif

#if defined(UTIL_ALL)
#   undef UTIL_ALL
#endif

#if defined(UTIL_STD)
#   undef UTIL_STD
#endif

#if defined(UTIL_TYPES)
#   undef UTIL_TYPES
#endif

char **process_words(char *buffer);

char *read_file(const char *file_name); 

#endif // !UTILS_H

// Chapter 55
