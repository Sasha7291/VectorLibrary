/*
 * vector_allocator.h
 *
 *  Created on: 24 февр. 2026 г.
 *      Author: Terentev
 */

#ifndef SRC_VECTOR_ALLOCATOR_H_
#define SRC_VECTOR_ALLOCATOR_H_


#include "vector_config.h"


typedef struct VECTOR_PACKED_STRUCT vector_allocator
{
    void *(*malloc)(vector_size_t size);
    void *(*calloc)(vector_size_t count, vector_size_t size);
    void *(*realloc)(void* ptr, vector_size_t new_size);
    void  (*free)(void* ptr);
    void *context;
    const char *name;
} vector_allocator_t;


inline vector_allocator_t *vector_default_allocator();


#endif /* SRC_VECTOR_ALLOCATOR_H_ */
