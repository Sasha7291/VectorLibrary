/**
 * @file      vector_size.h
 * @brief     Platform-independent ssize_t definition
 * @author    Sasha729
 * @version   1.0.1
 * @date      22.02.2026
 *
 * @details   This file provides a cross-platform definition of ssize_t
 *            for systems that may not have it defined.
 */

#ifndef __VECTOR_SIZE__
#define __VECTOR_SIZE__


#include <stdint.h>


/**
 * @def   ssize_t
 * @brief Signed size type for indices that can be -1
 *
 * If ssize_t is not already defined by the system, define it as
 * int64_t on 64-bit Windows or int32_t on 32-bit Windows.
 * On other systems, include sys/types.h for the system definition.
 */
#if !defined(__ssize_t_defined) && !defined(_SSIZE_T_DEFINED) && !defined(_SSIZE_T_) && !defined(_SSIZE_T_DECLARED)
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef _WIN64
            typedef int64_t ssize_t;
        #else
            typedef int32_t ssize_t;
        #endif
    #else
        #include <sys/types.h>
    #endif
#endif


#endif // __VECTOR_SIZE__
