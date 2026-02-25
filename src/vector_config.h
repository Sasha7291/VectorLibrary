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

#ifndef __VECTOR_CONFIG__
#define __VECTOR_CONFIG__


#include <stdint.h>


#ifdef VECTOR_NO_DYNAMIC_ALLOC
#undef VECTOR_SMALL_MEMORY
#undef VECTOR_USE_CUSTOM_ALLOCATOR
#endif // VECTOR_NO_DYNAMIC_ALLOC


#if defined(VECTOR_8BIT_SIZE)
#ifndef VECTOR_INDEX_MASK
#define VECTOR_INDEX_MASK	0xFF
#endif // VECTOR_INDEX_MASK
#ifndef VECTOR_SIZE_MAX
#define VECTOR_SIZE_MAX 	UINT8_MAX
#endif // VECTOR_SIZE_MAX
#ifndef VECTOR_INDEX_MAX
#define VECTOR_INDEX_MAX 	INT16_MAX
#endif // VECTOR_INDEX_MAX
	typedef int16_t vector_index_t;
	typedef uint8_t vector_size_t;
#elif defined(VECTOR_16BIT_SIZE)
#ifndef VECTOR_INDEX_MASK
#define VECTOR_INDEX_MASK	0xFFFF
#endif // VECTOR_INDEX_MASK
#ifndef VECTOR_SIZE_MAX
#define VECTOR_SIZE_MAX 	UINT16_MAX
#endif // VECTOR_SIZE_MAX
#ifndef VECTOR_INDEX_MAX
#define VECTOR_INDEX_MAX 	INT32_MAX
#endif // VECTOR_INDEX_MAX
	typedef int32_t vector_index_t;
	typedef uint16_t vector_size_t;
#else
#ifndef VECTOR_INDEX_MASK
#define VECTOR_INDEX_MASK	0xFFFFFFFF
#endif // VECTOR_INDEX_MASK
#ifndef VECTOR_SIZE_MAX
#define VECTOR_SIZE_MAX 	UINT32_MAX
#endif // VECTOR_SIZE_MAX
#ifndef VECTOR_INDEX_MAX
#define VECTOR_INDEX_MAX 	INT64_MAX
#endif // VECTOR_INDEX_MAX
	typedef int64_t vector_index_t;
	typedef uint32_t vector_size_t;
#endif

#ifdef VECTOR_USE_PACKED_STRUCT
#ifndef VECTOR_PACKED_STRUCT
#define VECTOR_PACKED_STRUCT __attribute__((packed))
#endif
#else // VECTOR_USE_PACKED_STRUCT
#ifndef VECTOR_PACKED_STRUCT
#define VECTOR_PACKED_STRUCT
#endif
#endif // VECTOR_USE_PACKED_STRUCT

#ifdef VECTOR_OPTIMIZE_SIZE
#ifndef VECTOR_GROWTH_FACTOR
#define VECTOR_GROWTH_FACTOR 1.5
#endif // VECTOR_GROWTH_FACTOR
#else // VECTOR_OPTIMIZE_SIZE
#ifndef VECTOR_GROWTH_FACTOR
#define VECTOR_GROWTH_FACTOR 2
#endif // VECTOR_GROWTH_FACTOR
#endif // VECTOR_OPTIMIZE_SIZE

#ifdef VECTOR_SMALL_MEMORY
#ifndef VECTOR_INITIAL_CAPACITY
#define VECTOR_INITIAL_CAPACITY 4
#endif // VECTOR_INITIAL_CAPACITY
#else // VECTOR_SMALL_MEMORY
#ifndef VECTOR_INITIAL_CAPACITY
#define VECTOR_INITIAL_CAPACITY 8
#endif // VECTOR_INITIAL_CAPACITY
#endif // VECTOR_SMALL_MEMORY

#ifdef VECTOR_LITE
#ifndef VECTOR_STATIC
#define VECTOR_STATIC
#endif // VECTOR_STATIC
#else // VECTOR_LITE
#ifndef VECTOR_STATIC
#define VECTOR_STATIC static
#endif // VECTOR_STATIC
#endif // VECTOR_LITE

#ifndef VECTOR_INVALID_INDEX
#define VECTOR_INVALID_INDEX	((vector_index_t)-1)
#endif // VECTOR_INVALID_INDEX


#endif // __VECTOR_CONFIG__
