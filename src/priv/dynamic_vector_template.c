/**
 * @file      vector_template.c
 * @brief     Template implementation of vector operations
 * @author    Sasha729
 * @version   1.0.1
 * @date      22.02.2026
 *
 * @details   This file contains the template implementation of all vector operations.
 *            It is included multiple times with different T definitions to generate
 *            type-specific implementations.
 *
 * @note      This file should not be included directly.
 *            Use the generated vector_impl.c instead.
 *
 * @see       vector_template.h
 * @see       vector_impl.c
 */

#ifdef T

#include "template.h"
#include "vector_allocator.h"
#include "vector_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct VECTOR_PACKED_STRUCT TEMPLATE(vector_private_t, T)
{
	T *__data;
    vector_size_t __size;
    vector_size_t __allocated_size;
#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
    vector_allocator_t *__allocator;
#endif // VECTOR_USE_CUSTOM_ALLOCATOR
};


#ifndef VECTOR
#define VECTOR 				TEMPLATE(vector, T)
#endif // VECTOR

#ifndef VECTOR_FUNC
#define VECTOR_FUNC(_name) 	TEMPLATE(vector, TEMPLATE(_name, T))
#endif // VECTOR_FUNC

#ifndef VECTOR_ITERATOR
#define VECTOR_ITERATOR		TEMPLATE(vector_iterator, T)
#endif // VECTOR_ITERATOR

#ifndef PRIVATE
#define PRIVATE(_range)		((_range)->__private)
#endif // PRIVATE

#ifdef VECTOR_USE_CUSTOM_ALLOCATOR

#ifndef VECTOR_MALLOC
#define VECTOR_MALLOC(_self, _size) \
	PRIVATE((_self))->__allocator->malloc((_size));
#endif // VECTOR_MALLOC

#ifndef VECTOR_CALLOC
#define VECTOR_CALLOC(_self, _count, _size) \
	PRIVATE((_self))->__allocator->calloc((_count), (_size));
#endif // VECTOR_CALLOC

#ifndef VECTOR_REALLOC
#define VECTOR_REALLOC(_self, _ptr, _size) \
	PRIVATE((_self))->__allocator->realloc((_ptr), (_size));
#endif // VECTOR_REALLOC

#ifndef VECTOR_FREE
#define VECTOR_FREE(_self, _ptr) \
	PRIVATE((_self))->__allocator->free((_ptr));
#endif // VECTOR_FREE

#else // VECTOR_USE_CUSTOM_ALLOCATOR

#ifndef VECTOR_MALLOC
#define VECTOR_MALLOC(_self, _size) 		malloc((_size));
#endif // VECTOR_MALLOC

#ifndef VECTOR_CALLOC
#define VECTOR_CALLOC(_self, _count, _size) calloc((_count), (_size));
#endif // VECTOR_CALLOC

#ifndef VECTOR_REALLOC
#define VECTOR_REALLOC(_self, _ptr, _size) 	realloc((_ptr), (_size));
#endif // VECTOR_REALLOC

#ifndef VECTOR_FREE
#define VECTOR_FREE(_self, _ptr) 			free((_ptr));
#endif // VECTOR_FREE

#endif // VECTOR_USE_CUSTOM_ALLOCATOR

typedef T * TEMPLATE(vector_iterator, T);

#ifdef VECTOR_CHECK_ON

#ifndef VECTOR_CHECK
#define VECTOR_CHECK(_cond, _err_ptr, _err, _ret) \
    do { \
        if (!(_cond)) { \
            if ((_err_ptr) != NULL) \
                *(_err_ptr) = (_err); \
            return _ret; \
        } \
    } while (0);
#endif // VECTOR_CHECK

#ifndef VECTOR_CHECK_INDEX
#define VECTOR_CHECK_INDEX(_range, _indx, _err_ptr, _ret) \
    VECTOR_CHECK( \
        ((_indx) >= 0) && ((vector_size_t)(_indx) < (*(_range))->size((_range))), \
        _err_ptr, \
        VECTOR_ERROR_INDEX, \
        _ret \
    )
#endif // VECTOR_CHECK_INDEX

#ifndef VECTOR_CHECK_ALLOC
#define VECTOR_CHECK_ALLOC(_data, _err_ptr, _ret) \
    VECTOR_CHECK( \
        (_data) != NULL, \
        _err_ptr, \
        VECTOR_ERROR_ALLOC, \
        _ret \
    )
#endif // VECTOR_CHECK_ALLOC

#ifndef VECTOR_CHECK_DATA
#define VECTOR_CHECK_DATA(_data, _err_ptr, _ret) \
    VECTOR_CHECK( \
        (_data) != NULL, \
        _err_ptr, \
        VECTOR_ERROR_NULL, \
        _ret \
    )
#endif // VECTOR_CHECK_DATA

#ifndef VECTOR_CHECK_ENSURE_CAPACITY
#define VECTOR_CHECK_ENSURE_CAPACITY(_range, _size, _err_ptr, _ret) \
    VECTOR_CHECK( \
        VECTOR_FUNC(ensure_capacity)((_range), (_size), (_err_ptr)), \
        _err_ptr, \
        VECTOR_ERROR_CAPACITY, \
        _ret \
    )
#endif // VECTOR_CHECK_ENSURE_CAPACITY

#ifndef VECTOR_CHECK_INPUT_RANGE
#define VECTOR_CHECK_INPUT_RANGE(_range, _err_ptr, _ret) \
    VECTOR_CHECK( \
        (_range) != NULL, \
        _err_ptr, \
        VECTOR_ERROR_NULL, \
        _ret \
    )
#endif // VECTOR_CHECK_INPUT_RANGE

#else // VECTOR_CHECK_ON

#ifndef VECTOR_CHECK
#define VECTOR_CHECK(_cond, _err_ptr, _err, _ret) ;
#endif // VECTOR_CHECK

#ifndef VECTOR_CHECK_INDEX
#define VECTOR_CHECK_INDEX(_range, _indx, _err_ptr, _ret) ;
#endif // VECTOR_CHECK_INDEX

#ifndef VECTOR_CHECK_ALLOC
#define VECTOR_CHECK_ALLOC(_data, _err_ptr, _ret) ;
#endif // VECTOR_CHECK_ALLOC

#ifndef VECTOR_CHECK_DATA
#define VECTOR_CHECK_DATA(_data, _err_ptr, _ret) ;
#endif // VECTOR_CHECK_DATA

#ifndef VECTOR_CHECK_ENSURE_CAPACITY
#define VECTOR_CHECK_ENSURE_CAPACITY(_range, _size, _err_ptr, _ret) ;
#endif // VECTOR_CHECK_ENSURE_CAPACITY

#ifndef VECTOR_CHECK_INPUT_RANGE
#define VECTOR_CHECK_INPUT_RANGE(_range, _err_ptr, _ret) ;
#endif // VECTOR_CHECK_INPUT_RANGE

#endif // VECTOR_CHECK_ON

#ifndef VECTOR_DATA
#define VECTOR_DATA(_range, _index) \
	(PRIVATE((_range))->__data[(_index)])
#endif // VECTOR_DATA

#ifndef VECTOR_BEGIN
#define VECTOR_BEGIN(_range) \
	(PRIVATE((_range))->__data)
#endif // VECTOR_BEGIN


static VECTOR_INLINE bool VECTOR_FUNC(ensure_capacity)(
    VECTOR **self,
    vector_size_t required,
    vector_error_t *error
);


#ifndef VECTOR_LITE
static VECTOR_INLINE void VECTOR_FUNC(append_range)(
    VECTOR **self,
    const VECTOR *range,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(assign)(
    VECTOR **self,
    vector_size_t count,
    T value,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(assign_range)(
    VECTOR **self,
    const VECTOR *range,
    vector_error_t *error
);
static VECTOR_INLINE T VECTOR_FUNC(at)(
	const VECTOR **self,
	vector_index_t index,
	vector_error_t *error
);
static VECTOR_INLINE T VECTOR_FUNC(back)(const VECTOR **self);
static VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(begin)(const VECTOR **self);
static VECTOR_INLINE vector_size_t VECTOR_FUNC(capacity)(const VECTOR **self);
static VECTOR_INLINE void VECTOR_FUNC(clear)(VECTOR **self);
static VECTOR_INLINE T *VECTOR_FUNC(data)(const VECTOR **self);
static VECTOR_INLINE void VECTOR_FUNC(emplace_indx)(
    VECTOR **self,
    vector_index_t index,
    T value,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(emplace_it)(
    VECTOR **self,
    const VECTOR_ITERATOR it,
    T value,
    vector_error_t *error
);
static VECTOR_INLINE void FUNC(emplace_range_indx)(
	VECTOR **self, 
	vector_index_t index, 
	const VECTOR *range,
    vector_error_t *error
);
static VECTOR_INLINE void FUNC(emplace_range_it)(
	VECTOR **self, 
	ITERATOR it, 
	const VECTOR *range,
    vector_error_t *error
);
static VECTOR_INLINE bool VECTOR_FUNC(empty)(const VECTOR **self);
static VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(end)(const VECTOR **self);
static VECTOR_INLINE void VECTOR_FUNC(erase_indx)(
    VECTOR **self,
    vector_index_t index,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(erase_it)(
    VECTOR **self,
    VECTOR_ITERATOR it,
    vector_error_t *error
);
static VECTOR_INLINE vector_index_t VECTOR_FUNC(find_first_not_of)(
	const VECTOR **self,
	T value
);
static VECTOR_INLINE vector_index_t VECTOR_FUNC(find_last_not_of)(
	const VECTOR **self,
	T value
);
static VECTOR_INLINE vector_index_t VECTOR_FUNC(find_first_of)(
	const VECTOR **self,
	T value
);
static VECTOR_INLINE vector_index_t VECTOR_FUNC(find_last_of)(
	const VECTOR **self,
	T value
);
static VECTOR_INLINE T VECTOR_FUNC(front)(const VECTOR **self);
static VECTOR_INLINE vector_index_t VECTOR_FUNC(indx)(
	const VECTOR **self,
    const VECTOR_ITERATOR it,
	vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(insert_indx)(
    VECTOR **self,
    vector_index_t before,
    T value,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(insert_it)(
    VECTOR **self,
    const VECTOR_ITERATOR it_before,
    T value,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(insert_range_indx)(
    VECTOR **self,
    vector_index_t before,
    const VECTOR *range,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(insert_range_it)(
    VECTOR **self,
    const VECTOR_ITERATOR it,
    const VECTOR *range,
    vector_error_t *error
);
static VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(it)(
    const VECTOR **self,
    vector_index_t index,
	vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(pop_back)(VECTOR **self);
static VECTOR_INLINE void VECTOR_FUNC(push_back)(
    VECTOR **self,
    T value,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(reserve)(
    VECTOR **self,
    vector_index_t new_capacity,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(resize)(
    VECTOR **self,
    vector_size_t new_size,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(resize_with)(
    VECTOR **self,
    vector_size_t new_size,
    T value,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(reverse_indx)(
    VECTOR **self,
    vector_index_t begin_index,
    vector_index_t end_index,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(reverse_it)(
    VECTOR **self,
    VECTOR_ITERATOR itb,
    VECTOR_ITERATOR ite,
    vector_error_t *error
);
static VECTOR_INLINE vector_size_t VECTOR_FUNC(size)(const VECTOR **self);
static VECTOR_INLINE void VECTOR_FUNC(swap_indx)(
    VECTOR **self,
    vector_index_t index_a,
    vector_index_t index_b,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(swap_it)(
    VECTOR **self,
    VECTOR_ITERATOR it_a,
    VECTOR_ITERATOR it_b,
    vector_error_t *error
);

#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
static VECTOR_INLINE vector_allocator_t *VECTOR_FUNC(allocator)(
	const VECTOR **,
	vector_error_t *
);
static VECTOR_INLINE void VECTOR_FUNC(set_allocator)(
	VECTOR **self,
	vector_allocator_t *alloc,
	vector_error_t error*
);
#endif // VECTOR_USE_CUSTOM_ALLOCATOR
#endif // VECTOR_LITE


VECTOR *VECTOR_FUNC(create_vector)(
#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
	const vector_allocator_t *alloc,
#endif // VECTOR_USE_CUSTOM_ALLOCATOR
	vector_size_t init_size,
	T init_value,
	vector_error_t *error
)
{
#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
	if (alloc == NULL)
		alloc == vector_default_allocator();
#endif // VECTOR_USE_CUSTOM_ALLOCATOR

	VECTOR *new_vec =
#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
		(VECTOR *)alloc->malloc(sizeof(VECTOR));
#else // VECTOR_USE_CUSTOM_ALLOCATOR
		(VECTOR *)VECTOR_MALLOC(sizeof(VECTOR));
#endif // VECTOR_USE_CUSTOM_ALLOCATOR
	VECTOR_CHECK_INPUT_RANGE(new_vec, error, NULL)
	
    PRIVATE(new_vec) =
#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
    	(TEMPLATE(vector_private, T) *)alloc->malloc(sizeof(VECTOR));
#else // VECTOR_USE_CUSTOM_ALLOCATOR
		(TEMPLATE(vector_private, T) *)VECTOR_MALLOC(sizeof(VECTOR));
#endif // VECTOR_USE_CUSTOM_ALLOCATOR
	if (PRIVATE(new_vec) == NULL)
	{
		if (error != NULL)
			*error = VECTOR_ERROR_ALLOC;
		
		VECTOR_FREE(other, new_vec);
		
        return NULL;
    }
    
    VECTOR_BEGIN(new_vec) = NULL;
    PRIVATE(new_vec)->__size = 0;
    PRIVATE(new_vec)->__allocated_size = 0;
#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
    PRIVATE(new_vec)->__allocator = alloc;
#endif // VECTOR_USE_CUSTOM_ALLOCATOR
    
    if (init_size > 0) 
	{
        if (!VECTOR_FUNC(ensure_capacity)(&new_vec, init_size, error))
		{
			VECTOR_FREE(other, PRIVATE(new_vec));
			VECTOR_FREE(other, new_vec);
			
            return NULL;
        }
        
        for (vector_index_t i = 0; i < (vector_index_t)init_size; ++i)
            VECTOR_DATA(new_vec, i) = init_value;

        PRIVATE(new_vec)->__size = init_size;
    }

#ifndef VECTOR_LITE
	new_vec->append_range = VECTOR_FUNC(append_range);
	new_vec->assign = VECTOR_FUNC(assign);
	new_vec->assign_range = VECTOR_FUNC(assign_range);
	new_vec->at = VECTOR_FUNC(at);
	new_vec->back = VECTOR_FUNC(back);
	new_vec->begin = VECTOR_FUNC(begin);
	new_vec->capacity = VECTOR_FUNC(capacity);
	new_vec->clear = VECTOR_FUNC(clear);
	new_vec->data = VECTOR_FUNC(data);
	new_vec->emplace_indx = VECTOR_FUNC(emplace_indx);
	new_vec->emplace_it = VECTOR_FUNC(emplace_it);
	new_vec->emplace_range_indx = VECTOR_FUNC(emplace_range_indx);
	new_vec->emplace_range_it = VECTOR_FUNC(emplace_range_it);
	new_vec->empty = VECTOR_FUNC(empty);
	new_vec->end = VECTOR_FUNC(end);
	new_vec->erase_indx = VECTOR_FUNC(erase_indx);
	new_vec->erase_it = VECTOR_FUNC(erase_it);
	new_vec->find_first_not_of = VECTOR_FUNC(find_first_not_of);
	new_vec->find_last_not_of = VECTOR_FUNC(find_last_not_of);
	new_vec->find_first_of = VECTOR_FUNC(find_first_of);
	new_vec->find_last_of = VECTOR_FUNC(find_last_of);
	new_vec->front = VECTOR_FUNC(front);
	new_vec->indx = VECTOR_FUNC(indx);
	new_vec->insert_indx = VECTOR_FUNC(insert_indx);
	new_vec->insert_it = VECTOR_FUNC(insert_it);
	new_vec->insert_range_indx = VECTOR_FUNC(insert_range_indx);
	new_vec->insert_range_it = VECTOR_FUNC(insert_range_it);
	new_vec->it = VECTOR_FUNC(it);
	new_vec->pop_back = VECTOR_FUNC(pop_back);
	new_vec->push_back = VECTOR_FUNC(push_back);
	new_vec->reserve = VECTOR_FUNC(reserve);
	new_vec->resize = VECTOR_FUNC(resize);
	new_vec->resize_with = VECTOR_FUNC(resize_with);
	new_vec->reverse_indx = VECTOR_FUNC(reverse_indx);
	new_vec->reverse_it = VECTOR_FUNC(reverse_it);
	new_vec->size = VECTOR_FUNC(size);
	new_vec->swap_indx = VECTOR_FUNC(swap_indx);
	new_vec->swap_it = VECTOR_FUNC(swap_it);

#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
	new_vec->allocator = VECTOR_FUNC(allocator);
	new_vec->set_allocator = VECTOR_FUNC(set_allocator);
#endif // VECTOR_USE_CUSTOM_ALLOCATOR
#endif // VECTOR_LITE

    return new_vec;
}

VECTOR *VECTOR_FUNC(copy_create_vector)(
	const VECTOR *other,
	vector_error_t *error
)
{
	VECTOR_CHECK_INPUT_RANGE(other, error, NULL)

	VECTOR *new_vec = (VECTOR *)VECTOR_MALLOC(other, sizeof(VECTOR));
	VECTOR_CHECK_INPUT_RANGE(new_vec, error, NULL)

    PRIVATE(new_vec) = (TEMPLATE(vector_private, T) *)VECTOR_MALLOC(
		other,
    	sizeof(TEMPLATE(vector_private, T))
	);
	if (PRIVATE(new_vec) == NULL)
	{
		if (error != NULL)
			*error = VECTOR_ERROR_ALLOC;

		VECTOR_FREE(other, new_vec);

        return NULL;
    }

	const TEMPLATE(vector_private_t, T) *private __attribute__((unused)) =
		(TEMPLATE(vector_private_t, T) *)memcpy(
			PRIVATE(new_vec),
			PRIVATE(other),
			sizeof(TEMPLATE(vector_private_t, T))
		);
	VECTOR_CHECK_DATA(private, error, NULL)

#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
    PRIVATE(new_vec)->__allocator = PRIVATE(other)->__allocator;
#endif // VECTOR_USE_CUSTOM_ALLOCATOR

    if (!VECTOR_FUNC(ensure_capacity)(&new_vec, other->size(&other), error))
	{
		VECTOR_FREE(other, PRIVATE(new_vec));
		VECTOR_FREE(other, new_vec);

        return NULL;
    }

	const T *data __attribute__((unused)) = (T *)memcpy(
		VECTOR_DATA(new_vec),
		VECTOR_DATA(other),
		PRIVATE(other)->__size * sizeof(T)
	);
	VECTOR_CHECK_DATA(data, error, NULL)

#ifndef VECTOR_LITE
	new_vec->append_range = other->append_range;
	new_vec->assign = other->assign;
	new_vec->assign_range = other->assign_range;
	new_vec->at = other->at;
	new_vec->back = other->back;
	new_vec->begin = other->begin;
	new_vec->capacity = other->capacity;
	new_vec->clear = other->clear;
	new_vec->data = other->data;
	new_vec->emplace_indx = other->emplace_indx;
	new_vec->emplace_it = other->emplace_it;
	new_vec->emplace_range_indx = other->emplace_range_indx;
	new_vec->emplace_range_it = other->emplace_range_it;
	new_vec->empty = other->empty;
	new_vec->end = other->end;
	new_vec->erase_indx = other->erase_indx;
	new_vec->erase_it = other->erase_it;
	new_vec->find_first_not_of = other->find_first_not_of;
	new_vec->find_last_not_of = other->find_last_not_of;
	new_vec->find_first_of = other->find_first_of;
	new_vec->find_last_of = other->find_last_of;
	new_vec->front = other->front;
	new_vec->indx = other->indx;
	new_vec->insert_indx = other->insert_indx;
	new_vec->insert_it = other->insert_it;
	new_vec->insert_range_indx = other->insert_range_indx;
	new_vec->insert_range_it = other->insert_range_it;
	new_vec->it = other->it;
	new_vec->pop_back = other->pop_back;
	new_vec->push_back = other->push_back;
	new_vec->reserve = other->reserve;
	new_vec->resize = other->resize;
	new_vec->resize_with = other->resize_with;
	new_vec->reverse_indx = other->reverse_indx;
	new_vec->reverse_it = other->reverse_it;
	new_vec->size = other->size;
	new_vec->swap_indx = other->swap_indx;
	new_vec->swap_it = other->swap_it;
#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
	new_vec->allocator = other->allocator;
	new_vec->set_allocator = other->set_allocator;
#endif // VECTOR_USE_CUSTOM_ALLOCATOR
#endif // VECTOR_LITE

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;

	return new_vec;
}

VECTOR *VECTOR_FUNC(move_create_vector)(
	VECTOR **other,
	vector_error_t *error
)
{
	VECTOR_CHECK_INPUT_RANGE(other, error, NULL)
	VECTOR_CHECK_INPUT_RANGE(*other, error, NULL)
	
	VECTOR *new_vec = (VECTOR *)VECTOR_MALLOC(other, sizeof(VECTOR));
	VECTOR_CHECK_ALLOC(new_vec, error, NULL);

	VECTOR *other_vec = *other;

    PRIVATE(new_vec) = PRIVATE(other_vec);

#ifndef VECTOR_LITE
	new_vec->append_range = other_vec->append_range;
	new_vec->assign = other_vec->assign;
	new_vec->assign_range = other_vec->assign_range;
	new_vec->at = other_vec->at;
	new_vec->back = other_vec->back;
	new_vec->begin = other_vec->begin;
	new_vec->capacity = other_vec->capacity;
	new_vec->clear = other_vec->clear;
	new_vec->data = other_vec->data;
	new_vec->emplace_indx = other_vec->emplace_indx;
	new_vec->emplace_it = other_vec->emplace_it;
	new_vec->empty = other_vec->empty;
	new_vec->end = other_vec->end;
	new_vec->erase_indx = other_vec->erase_indx;
	new_vec->erase_it = other_vec->erase_it;
	new_vec->emplace_range_indx = other->emplace_range_indx;
	new_vec->emplace_range_it = other->emplace_range_it;
	new_vec->find_first_not_of = other_vec->find_first_not_of;
	new_vec->find_last_not_of = other_vec->find_last_not_of;
	new_vec->find_first_of = other_vec->find_first_of;
	new_vec->find_last_of = other_vec->find_last_of;
	new_vec->front = other_vec->front;
	new_vec->indx = other_vec->indx;
	new_vec->insert_indx = other_vec->insert_indx;
	new_vec->insert_it = other_vec->insert_it;
	new_vec->insert_range_indx = other_vec->insert_range_indx;
	new_vec->insert_range_it = other_vec->insert_range_it;
	new_vec->it = other_vec->it;
	new_vec->pop_back = other_vec->pop_back;
	new_vec->push_back = other_vec->push_back;
	new_vec->reserve = other_vec->reserve;
	new_vec->resize = other_vec->resize;
	new_vec->resize_with = other_vec->resize_with;
	new_vec->reverse_indx = other_vec->reverse_indx;
	new_vec->reverse_it = other_vec->reverse_it;
	new_vec->size = other_vec->size;
	new_vec->swap_indx = other_vec->swap_indx;
	new_vec->swap_it = other_vec->swap_it;
#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
	new_vec->allocator = other_vec->allocator;
	new_vec->set_allocator = other_vec->set_allocator;
#endif // VECTOR_USE_CUSTOM_ALLOCATOR
#endif // VECTOR_LITE	

	VECTOR_FREE(new_vector, other_vec);
	other_vec = NULL;

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;

    return new_vec;
}

void VECTOR_FUNC(destroy_vector)(VECTOR **self)
{
    if (self == NULL)
		return;

    VECTOR *vec = *self;
    if (vec == NULL)
		return;
	
	VECTOR_CHECK_ALLOC(PRIVATE(vec)->__allocator, error, )

    if (PRIVATE(vec)->__data != NULL)
    	VECTOR_FREE(vec, PRIVATE(vec)->__data);

    if (PRIVATE(vec) != NULL)
    	VECTOR_FREE(vec, PRIVATE(vec));

    VECTOR_FREE(vec, vec);

    vec = NULL;
}

static VECTOR_INLINE bool VECTOR_FUNC(ensure_capacity)(
	VECTOR **self,
	vector_size_t required,
	vector_error_t *error
)
{
    VECTOR *vec = *self;

    if (required <= PRIVATE(vec)->__allocated_size)
        return true;

    vector_size_t new_capacity = PRIVATE(vec)->__allocated_size;

    if (new_capacity == 0)
        new_capacity = VECTOR_INITIAL_CAPACITY;

    while (new_capacity < required)
        new_capacity *= VECTOR_GROWTH_FACTOR;

    const T *new_data = (T *)VECTOR_REALLOC(
		vec,
    	VECTOR_BEGIN(vec),
		new_capacity * sizeof(T)
    );

    VECTOR_CHECK_ALLOC(new_data, error, false)

    VECTOR_BEGIN(vec) = new_data;
    PRIVATE(vec)->__allocated_size = new_capacity;

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;

    return true;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(append_range)(
	VECTOR **self,
	const VECTOR *range,
	vector_error_t *error
)
{
	VECTOR_CHECK_INPUT_RANGE(range, error, )
	
	VECTOR_FUNC(insert_range_indx)(
		self,
		PRIVATE(*self)->__size,
		range,
		error
	);
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(assign)(
	VECTOR **self,
	vector_size_t count,
	T value,
	vector_error_t *error
)
{
	VECTOR_CHECK_ENSURE_CAPACITY(self, count, error, )
	
	VECTOR *vec = *self;
	
	for (vector_index_t i = 0; i < (vector_index_t)count; ++i)
		VECTOR_DATA(vec, i) = value;

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(assign_range)(
	VECTOR **self,
	const VECTOR *range,
	vector_error_t *error
)
{
	const vector_size_t range_size = PRIVATE(range)->__size;
	
	VECTOR_CHECK_INPUT_RANGE(range, error, )
	VECTOR_CHECK_ENSURE_CAPACITY(self, range_size, error, )
	
	VECTOR *vec = *self;
	
	for (vector_index_t i = 0; i < (vector_index_t)range_size; ++i)
		VECTOR_DATA(vec, i) = VECTOR_DATA(range, i);

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE T VECTOR_FUNC(at)(
	const VECTOR **self,
	vector_index_t index,
	vector_error_t *error
)
{
	VECTOR_CHECK_INDEX(self, index, error, (T)0)

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;

    return VECTOR_DATA(*self, index);
}

VECTOR_STATIC VECTOR_INLINE T VECTOR_FUNC(back)(const VECTOR **self)
{
	const VECTOR *vec = *self;

    return VECTOR_DATA(vec, PRIVATE(vec)->__size - 1);
}

VECTOR_STATIC VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(begin)(const VECTOR **self)
{
    return VECTOR_BEGIN(*self);
}

VECTOR_STATIC VECTOR_INLINE vector_size_t VECTOR_FUNC(capacity)(const VECTOR **self)
{
    return PRIVATE(*self)->__allocated_size;
}

VECTOR_STATIC VECTOR_INLINE  void VECTOR_FUNC(clear)(VECTOR **const self)
{
    PRIVATE(*self)->__size = 0;
}

VECTOR_STATIC VECTOR_INLINE T *VECTOR_FUNC(data)(const VECTOR **self)
{
    return VECTOR_BEGIN(*self);
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(emplace_indx)(
	VECTOR **self,
	vector_index_t index,
	T value,
	vector_error_t *error
)
{
	VECTOR_CHECK_INDEX((const VECTOR **)self, index, error, )

    VECTOR_DATA(*self, index) = value;

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(emplace_it)(
	VECTOR **self,
	const VECTOR_ITERATOR it,
	T value,
	vector_error_t *error
)
{
	VECTOR_FUNC(emplace_indx)(
		self,
		VECTOR_FUNC(indx)((const VECTOR **)self, it, error),
		value,
		error
	);
}

VECTOR_STATIC VECTOR_INLINE void FUNC(emplace_range_indx)(
	VECTOR **self, 
	vector_index_t index, 
	const VECTOR *range,
    vector_error_t *error
)
{
	VECTOR_CHECK_INDEX((const VECTOR **)self, index, error, )
	VECTOR_CHECK_INDEX(
		(const VECTOR **)self, 
		index + PRIVATE(range)->__size, 
		error, 
	)
	
	VECTOR *vec = *self;
	
	for (
		vector_index_t i = index; 
		i < (vector_index_t)(index + PRIVATE(range)->__size); 
		++i
	)
	{
		VECTOR_DATA(vec, i) = VECTOR_DATA(range, i);
	}
	
	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void FUNC(emplace_range_it)(
	VECTOR **self, 
	ITERATOR it, 
	const VECTOR *range,
    vector_error_t *error
)
{
	(*self)->emplace_range_indx(
		self,
		(*self)->index(self, it),
		range
	);
}

VECTOR_STATIC VECTOR_INLINE bool VECTOR_FUNC(empty)(const VECTOR **self)
{
    return (PRIVATE(*self)->__size == 0);
}

VECTOR_STATIC VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(end)(const VECTOR **self)
{
	const VECTOR *vec = *self;

    return VECTOR_BEGIN(vec) + PRIVATE(vec)->__size;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(erase_indx)(
	VECTOR **self,
	vector_index_t index,
	vector_error_t *error
)
{
	VECTOR_CHECK_INDEX((const VECTOR **)self, index, error, )

	VECTOR *vec = *self;

    if (index == PRIVATE(vec)->__size - 1)
	{
        VECTOR_FUNC(pop_back)(self);
		
        return;
    }
    
    const T *data __attribute__((unused)) = (T *)memmove(
		VECTOR_BEGIN(vec) + index,
        VECTOR_BEGIN(vec) + index + 1,
        (PRIVATE(vec)->__size - index - 1) * sizeof(T)
	);
    VECTOR_CHECK_DATA(data, error, )

    --PRIVATE(vec)->__size;

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(erase_it)(
	VECTOR **self,
	VECTOR_ITERATOR it,
	vector_error_t *error
)
{
	VECTOR_FUNC(erase_indx)(
		self,
		VECTOR_FUNC(indx)((const VECTOR **)self, it, error),
		error
	);
}

VECTOR_STATIC VECTOR_INLINE vector_index_t VECTOR_FUNC(find_first_not_of)(
	const VECTOR **self,
	T value
)
{
    if (VECTOR_FUNC(empty)(self))
    	return VECTOR_INVALID_INDEX;

    for (vector_index_t i = 0; i < (vector_index_t)PRIVATE(*self)->__size; ++i)
        if (VECTOR_DATA(self, i) != value)
            return i;

    return VECTOR_INVALID_INDEX;
}

VECTOR_STATIC VECTOR_INLINE vector_index_t VECTOR_FUNC(find_last_not_of)(
	const VECTOR **self,
	T value
)
{
    if (VECTOR_FUNC(empty)(self))
    	return VECTOR_INVALID_INDEX;

    for (vector_index_t i = (vector_index_t)PRIVATE(*self)->__size - 1; i >= 0; --i)
        if (VECTOR_DATA(self, i) != value)
            return i;

    return VECTOR_INVALID_INDEX;
}

VECTOR_STATIC VECTOR_INLINE vector_index_t VECTOR_FUNC(find_first_of)(
	const VECTOR **self,
	T value
)
{
    if (VECTOR_FUNC(empty)(self))
    	return VECTOR_INVALID_INDEX;

    for (vector_index_t i = 0; i < (vector_index_t)PRIVATE(*self)->__size; ++i)
        if (VECTOR_DATA(self, i) == value)
            return i;

    return VECTOR_INVALID_INDEX;
}

VECTOR_STATIC VECTOR_INLINE vector_index_t VECTOR_FUNC(find_last_of)(
	const VECTOR **self,
	T value
)
{
    if (VECTOR_FUNC(empty)(self))
    	return VECTOR_INVALID_INDEX;

	for (vector_index_t i = (vector_index_t)PRIVATE(*self)->__size - 1; i >= 0; --i)
        if (VECTOR_DATA(self, i) == value)
            return i;

    return VECTOR_INVALID_INDEX;
}

VECTOR_STATIC VECTOR_INLINE T VECTOR_FUNC(front)(const VECTOR **self)
{
    return VECTOR_DATA(*self, 0);
}

VECTOR_STATIC VECTOR_INLINE vector_index_t VECTOR_FUNC(indx)(
	const VECTOR **self,
	const VECTOR_ITERATOR it,
	vector_error_t *error
)
{
	const vector_index_t index =
		(vector_index_t)(it - VECTOR_BEGIN(*self));

	VECTOR_CHECK_INDEX(self, index, error, VECTOR_INVALID_INDEX)

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;

	return index;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(insert_indx)(
	VECTOR **self,
	vector_index_t before,
	T value,
	vector_error_t *error
)
{	
	VECTOR *vec = *self;

	VECTOR_CHECK_INDEX((const VECTOR **)self, before, error, )
    
    if (before == PRIVATE(vec)->__size)
	{
        VECTOR_FUNC(push_back)(self, value, error);
		
        return;
    }
	
	VECTOR_CHECK_ENSURE_CAPACITY(self, PRIVATE(vec)->__size + 1, error, )
    
    const T *data __attribute__((unused)) = (T *)memmove(
		VECTOR_BEGIN(vec) + before + 1,
        VECTOR_BEGIN(vec) + before,
        (PRIVATE(vec)->__size - before) * sizeof(T)
	);

	VECTOR_CHECK_DATA(data, error, )
    
    VECTOR_DATA(vec, before) = value;
    ++PRIVATE(vec)->__size;

    if (error != NULL)
    	*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(insert_it)(
	VECTOR **self,
	const VECTOR_ITERATOR it_before,
	T value,
	vector_error_t *error
)
{
	VECTOR_FUNC(insert_indx)(
		self,
		VECTOR_FUNC(indx)((const VECTOR **)self, it_before, error),
		value,
		error
	);
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(insert_range_indx)(
	VECTOR **self,
	vector_index_t before,
	const VECTOR *range,
	vector_error_t *error
)
{
	VECTOR_CHECK_INPUT_RANGE(range, error, )
    
    const VECTOR **range_ptr = &range;
    const vector_size_t range_size = VECTOR_FUNC(size)(range_ptr);
    
    if (range_size == 0)
		return;
	
    VECTOR_CHECK_INDEX((const VECTOR **)self, before, error, )
	VECTOR_CHECK_ENSURE_CAPACITY(
		self,
		PRIVATE(*self)->__size + range_size,
		error,
	)

	VECTOR *vec = *self;

    if (before < PRIVATE(vec)->__size)
    {
        const T *data __attribute__((unused)) = (T *)memmove(
			VECTOR_BEGIN(vec) + before + range_size,
            VECTOR_BEGIN(vec) + before,
            (PRIVATE(vec)->__size - before) * sizeof(T)
		);

        VECTOR_CHECK_DATA(data, error, )
    }
    
    for (vector_index_t i = 0; i < (vector_index_t)range_size; ++i)
        VECTOR_DATA(vec, before + i) = VECTOR_DATA(vec, i);
    
    PRIVATE(vec)->__size += range_size;

    if (error != NULL)
    	*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(insert_range_it)(
	VECTOR **self,
	const VECTOR_ITERATOR it_before,
	const VECTOR *range,
	vector_error_t *error
)
{
	VECTOR_FUNC(insert_range_indx)(
		self,
		VECTOR_FUNC(indx)((const VECTOR **)self, it_before, error),
		range,
		error
	);
}

VECTOR_STATIC VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(it)(
	const VECTOR **self,
	vector_index_t index,
	vector_error_t *error
)
{
	VECTOR_CHECK_INDEX(self, index, error, NULL)

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;
	
	return VECTOR_BEGIN(*self) + index;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(pop_back)(VECTOR **self)
{
	VECTOR *vec = *self;

	if (PRIVATE(vec)->__size > 0)
		--PRIVATE(vec)->__size;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(push_back)(
	VECTOR **self,
	T value,
	vector_error_t *error
)
{	
	VECTOR *vec = *self;

	VECTOR_CHECK_ENSURE_CAPACITY(self, PRIVATE(vec)->__size + 1, error, )
    
    VECTOR_DATA(vec, PRIVATE(vec)->__size) = value;
    ++PRIVATE(vec)->__size;
    
	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(reserve)(
	VECTOR **self,
	vector_size_t new_capacity,
	vector_error_t *error
)
{
	VECTOR_CHECK_ENSURE_CAPACITY(self, new_capacity, error, )
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(resize)(
	VECTOR **self,
	vector_size_t new_size,
	vector_error_t *error
)
{
	VECTOR_FUNC(resize_with)(self, new_size, (T)0, error);
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(resize_with)(
	VECTOR **self,
	vector_size_t new_size,
	T value,
	vector_error_t *error
)
{
    VECTOR *vec = *self;

	VECTOR_CHECK_ENSURE_CAPACITY(self, new_size, error, )
	
	for (
		vector_index_t i = (vector_index_t)PRIVATE(vec)->__size; 
		i < (vector_index_t)new_size; 
		++i
	)
	{
		VECTOR_DATA(vec, i) = value;
	}
	
	PRIVATE(vec)->__size = new_size;

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(reverse_indx)(
	VECTOR **self,
	vector_index_t begin_index,
	vector_index_t end_index,
	vector_error_t *error
)
{
	--end_index;
	
	if (begin_index == end_index)
	{
		if (error != NULL)
			*error = VECTOR_ERROR_SUCCESS;
		
		return;
	}
	
	if (VECTOR_FUNC(empty)((const VECTOR **)self))
		return;

	VECTOR_CHECK_INDEX((const VECTOR **)self, begin_index, error, )
	VECTOR_CHECK_INDEX((const VECTOR **)self, end_index, error, )
	
	VECTOR *vec = *self;
	T temp = (T)0;
	
	while (begin_index < end_index)
	{
		temp = VECTOR_DATA(vec, begin_index);
		VECTOR_DATA(vec, begin_index) = VECTOR_DATA(vec, end_index);
		VECTOR_DATA(vec, end_index) = temp;
		
		++begin_index;
		--end_index;
	}

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(reverse_it)(
	VECTOR **self,
	VECTOR_ITERATOR itb,
	VECTOR_ITERATOR ite,
	vector_error_t *error
)
{
	VECTOR_FUNC(reverse_indx)(
		self,
		VECTOR_FUNC(indx)((const VECTOR **)self, itb, error),
		VECTOR_FUNC(indx)((const VECTOR **)self, ite, error),
		error
	);
}

VECTOR_STATIC VECTOR_INLINE vector_size_t VECTOR_FUNC(size)(const VECTOR **self)
{
    return PRIVATE(*self)->__size;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(swap_indx)(
	VECTOR **self,
	vector_index_t index_a,
	vector_index_t index_b,
	vector_error_t *error
)
{
	if (index_a == index_b)
	{
		if (error != NULL)
			*error = VECTOR_ERROR_SUCCESS;
		
		return;
	}
	
	VECTOR_CHECK_INDEX((const VECTOR **)self, index_a, error, )
	VECTOR_CHECK_INDEX((const VECTOR **)self, index_b, error, )

	VECTOR *vec = *self;
	
    const T temp = VECTOR_DATA(vec, index_a);
    VECTOR_DATA(vec, index_a) = VECTOR_DATA(vec, index_b);
    VECTOR_DATA(vec, index_b) = temp;

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(swap_it)(
	VECTOR **self,
	VECTOR_ITERATOR it_a,
	VECTOR_ITERATOR it_b,
	vector_error_t *error
)
{
	VECTOR_FUNC(swap_indx)(
		self,
		VECTOR_FUNC(indx)((const VECTOR **)self, it_a, error),
		VECTOR_FUNC(indx)((const VECTOR **)self, it_b, error),
		error
	);
}

#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
VECTOR_STATIC VECTOR_INLINE vector_allocator_t *VECTOR_FUNC(allocator)(
	const VECTOR **,
	vector_error_t *
)
{
	VECTOR_CHECK_ALLOC(PRIVATE(*self)->__allocator, error, NULL)
	
	return PRIVATE(*self)->__allocator;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(set_allocator)(
	VECTOR **self,
	vector_allocator_t *alloc,
	vector_error_t error*
)
{
	VECTOR_CHECK_ALLOC(alloc, error, NULL)

	PRIVATE(*self)->__allocator = alloc;
}
#endif // VECTOR_USE_CUSTOM_ALLOCATOR


#undef VECTOR
#undef VECTOR_FUNC
#undef VECTOR_ITERATOR
#undef PRIVATE
#undef VECTOR_MALLOC
#undef VECTOR_CALLOC
#undef VECTOR_REALLOC
#undef VECTOR_FREE


#endif // T
