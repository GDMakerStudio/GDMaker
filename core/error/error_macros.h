/**********************************************************************************
 * MIT License
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of 
 * this software and associated documentation files (the "Software"), to deal in the 
 * Software without restriction, including without limitation the rights to use, copy, 
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, subject to the 
 * following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies 
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************/


#ifndef ERROR_MACROS_H
#define ERROR_MACROS_H

#include "error_list.h"


#include <stdio.h>
#include <string.h>



#define ERROR_PRINT(msg) \
    do { \
        fprintf(stderr, "Error: %s at %s:%d\n", msg, __FILE__, __LINE__); \
    } while(0)

#define ERROR_WARN(msg) \
    do { \
        fprintf(stderr, "Warning: %s at %s:%d\n", msg, __FILE__, __LINE__); \
    } while(0)

#define ERR_FAIL_COND_MSG(cond, msg) \
    do { \
        if (cond) { \
            fprintf(stderr, "Error: %s at %s:%d\n", msg, __FILE__, __LINE__); \
            return msg; \
        } \
    } while(0)

#define ERR_FAIL_NULL_MSG(ptr, msg) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "Error: %s at %s:%d\n", msg, __FILE__, __LINE__); \
            return msg; \
        } \
    } while(0)

#define ERR_FAIL_INDEX_MSG(index, size, msg) \
    do { \
        if ((index) < 0 || (index) >= (size)) { \
            fprintf(stderr, "Error: %s at %s:%d\n", msg, __FILE__, __LINE__); \
            return msg; \
        } \
    } while(0)

// Use these macros for common error conditions
#define ERR_FAIL_COND(cond) ERR_FAIL_COND_MSG(cond, error_list[1]) // error_list[1] for generic "Failed" message
#define ERR_FAIL_NULL(ptr) ERR_FAIL_NULL_MSG(ptr, error_list[1]) // error_list[1] for generic "Failed" message
#define ERR_FAIL_INDEX(index, size) ERR_FAIL_INDEX_MSG(index, size, error_list[1]) // error_list[1] for generic "Failed" message







#endif // ERROR_MACROS_H