/*
 * vector_allocator.c
 *
 *  Created on: 24 февр. 2026 г.
 *      Author: Terentev
 */

#include "vector_allocator.h"

#include <string.h>


static inline void *vector_default_malloc(vector_size_t size);
static inline void *vector_default_calloc(
	vector_size_t count,
	vector_size_t size
);
static inline void *vector_default_realloc(
	void *ptr,
	vector_size_t new_size
);
static inline void vector_default_free(void *ptr);


static vector_allocator_t __vector_default_allocator =
{
    .malloc = vector_default_malloc,
    .calloc = vector_default_calloc,
    .realloc = vector_default_realloc,
    .free = vector_default_free,
    .context = NULL,
    .name = "default"
};


inline vector_allocator_t *vector_default_allocator()
{
	return &__vector_default_allocator;
}


static inline void *vector_default_malloc(vector_size_t size)
{
    return malloc(size);
}

static inline void *vector_default_calloc(
	vector_size_t count,
	vector_size_t size
)
{
    return calloc(count, size);
}

static inline void *vector_default_realloc(
	void *ptr,
	vector_size_t new_size
)
{
    return realloc(ptr, new_size);
}

static inline void vector_default_free(void *ptr)
{
    free(ptr);
}
