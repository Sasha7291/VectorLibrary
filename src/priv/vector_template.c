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
#include "vector_error.h"
#include "vector_size.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct TEMPLATE(vector_private_t, T)
{
    T *__data;
    size_t __size;
    size_t __allocated_size;
};

typedef T * TEMPLATE(iterator_t, T);


#ifndef VECTOR
#define VECTOR 				TEMPLATE(vector, T)
#endif // VECTOR

#ifndef VECTOR_FUNC
#define VECTOR_FUNC(_name) 	TEMPLATE(vector, TEMPLATE(_name, T))
#endif // VECTOR_FUNC

#ifndef VECTOR_ITERATOR
#define VECTOR_ITERATOR		TEMPLATE(iterator_t, T)
#endif // VECTOR_ITERATOR

#ifndef PRIVATE
#define PRIVATE(_self)		((_self)->__private)
#endif // PRIVATE

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
        (size_t)(_indx) < (*(_range))->size((_range)), \
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

#else

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

#endif // VECTOR_CHECK_ON // VECTOR_CHECK_OFF

#ifndef VECTOR_GROWTH_FACTOR
    #ifdef VECTOR_OPTIMIZE_SIZE
        #define VECTOR_GROWTH_FACTOR 1.5
    #else
        #define VECTOR_GROWTH_FACTOR 2
    #endif
#endif

#ifndef VECTOR_INITIAL_CAPACITY
    #ifdef VECTOR_SMALL_MEMORY
        #define VECTOR_INITIAL_CAPACITY 4
    #else
        #define VECTOR_INITIAL_CAPACITY 8
    #endif
#endif


static bool VECTOR_FUNC(ensure_capacity)(
    VECTOR **self,
    size_t required,
    vector_error_t *error
);
#ifndef VECTOR_LITE
#define VECTOR_STATIC static
static void VECTOR_FUNC(append_range)(
    VECTOR **self,
    const VECTOR *range,
    vector_error_t *error
);
static void VECTOR_FUNC(assign)(
    VECTOR **self,
    size_t count,
    T value,
    vector_error_t *error
);
static void VECTOR_FUNC(assign_range)(
    VECTOR **self,
    const VECTOR *range,
    vector_error_t *error
);
static T VECTOR_FUNC(at)(
	const VECTOR **self,
	size_t index,
	vector_error_t *error
);
static T VECTOR_FUNC(back)(const VECTOR **self);
static VECTOR_ITERATOR VECTOR_FUNC(begin)(const VECTOR **self);
static size_t VECTOR_FUNC(capacity)(const VECTOR **self);
static void VECTOR_FUNC(clear)(VECTOR **self);
static T *VECTOR_FUNC(data)(const VECTOR **self);
static void VECTOR_FUNC(emplace_indx)(
    VECTOR **self,
    size_t index,
    T value,
    vector_error_t *error
);
static void VECTOR_FUNC(emplace_it)(
    VECTOR **self,
    const VECTOR_ITERATOR it,
    T value,
    vector_error_t *error
);
static bool VECTOR_FUNC(empty)(const VECTOR **self);
static VECTOR_ITERATOR VECTOR_FUNC(end)(const VECTOR **self);
static void VECTOR_FUNC(erase_indx)(
    VECTOR **self,
    size_t index,
    vector_error_t *error
);
static void VECTOR_FUNC(erase_it)(
    VECTOR **self,
    VECTOR_ITERATOR it,
    vector_error_t *error
);
static ssize_t VECTOR_FUNC(find_first_not_of)(const VECTOR **self, T value);
static ssize_t VECTOR_FUNC(find_last_not_of)(const VECTOR **self, T value);
static ssize_t VECTOR_FUNC(find_first_of)(const VECTOR **self, T value);
static ssize_t VECTOR_FUNC(find_last_of)(const VECTOR **self, T value);
static T VECTOR_FUNC(front)(const VECTOR **self);
static ssize_t VECTOR_FUNC(index)(
	const VECTOR **self,
    const VECTOR_ITERATOR it,
	vector_error_t *error
);
static void VECTOR_FUNC(insert_indx)(
    VECTOR **self,
    size_t before,
    T value,
    vector_error_t *error
);
static void VECTOR_FUNC(insert_it)(
    VECTOR **self,
    const VECTOR_ITERATOR it_before,
    T value,
    vector_error_t *error
);
static void VECTOR_FUNC(insert_range_indx)(
    VECTOR **self,
    size_t before,
    const VECTOR *range,
    vector_error_t *error
);
static void VECTOR_FUNC(insert_range_it)(
    VECTOR **self,
    const VECTOR_ITERATOR it,
    const VECTOR *range,
    vector_error_t *error
);
static VECTOR_ITERATOR VECTOR_FUNC(iterator)(
    const VECTOR **self,
    size_t index,
	vector_error_t *error
);
static void VECTOR_FUNC(pop_back)(VECTOR **self);
static void VECTOR_FUNC(push_back)(
    VECTOR **self,
    T value,
    vector_error_t *error
);
static void VECTOR_FUNC(reserve)(
    VECTOR **self,
    size_t new_capacity,
    vector_error_t *error
);
static void VECTOR_FUNC(resize)(
    VECTOR **self,
    size_t new_size,
    vector_error_t *error
);
static void VECTOR_FUNC(resize_with)(
    VECTOR **self,
    size_t new_size,
    T value,
    vector_error_t *error
);
static void VECTOR_FUNC(reverse_indx)(
    VECTOR **self,
    size_t begin_index,
    size_t end_index,
    vector_error_t *error
);
static void VECTOR_FUNC(reverse_it)(
    VECTOR **self,
    VECTOR_ITERATOR itb,
    VECTOR_ITERATOR ite,
    vector_error_t *error
);
static size_t VECTOR_FUNC(size)(const VECTOR **self);
static void VECTOR_FUNC(swap_indx)(
    VECTOR **self,
    size_t index_a,
    size_t index_b,
    vector_error_t *error
);
static void VECTOR_FUNC(swap_it)(
    VECTOR **self,
    VECTOR_ITERATOR it_a,
    VECTOR_ITERATOR it_b,
    vector_error_t *error
);

#else // VECTOR_LITE
#define VECTOR_STATIC
#endif // VECTOR_LITE


VECTOR *VECTOR_FUNC(create_vector)(
	size_t init_size,
	T init_value,
	vector_error_t *error
)
{
	VECTOR *new_vec = (VECTOR *)malloc(sizeof(VECTOR));
	VECTOR_CHECK_INPUT_RANGE(new_vec, error, NULL)
	
    PRIVATE(new_vec) = (TEMPLATE(vector_private, T) *)malloc(
    	sizeof(TEMPLATE(vector_private, T))
	);
	if (PRIVATE(new_vec) == NULL)
	{
        *error = VECTOR_ERROR_ALLOC;
		
		free(new_vec);
		
        return NULL;
    }
    
    PRIVATE(new_vec)->__data = NULL;
    PRIVATE(new_vec)->__size = 0;
    PRIVATE(new_vec)->__allocated_size = 0;
    
    if (init_size > 0) 
	{
        if (!VECTOR_FUNC(ensure_capacity)(&new_vec, init_size, error))
		{
			free(PRIVATE(new_vec));
            free(new_vec);
			
            return NULL;
        }
        
        for (size_t i = 0; i < init_size; ++i)
            PRIVATE(new_vec)->__data[i] = init_value;

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
	new_vec->empty = VECTOR_FUNC(empty);
	new_vec->end = VECTOR_FUNC(end);
	new_vec->erase_indx = VECTOR_FUNC(erase_indx);
	new_vec->erase_it = VECTOR_FUNC(erase_it);
	new_vec->find_first_not_of = VECTOR_FUNC(find_first_not_of);
	new_vec->find_last_not_of = VECTOR_FUNC(find_last_not_of);
	new_vec->find_first_of = VECTOR_FUNC(find_first_of);
	new_vec->find_last_of = VECTOR_FUNC(find_last_of);
	new_vec->front = VECTOR_FUNC(front);
	new_vec->index = VECTOR_FUNC(index);
	new_vec->insert_indx = VECTOR_FUNC(insert_indx);
	new_vec->insert_it = VECTOR_FUNC(insert_it);
	new_vec->insert_range_indx = VECTOR_FUNC(insert_range_indx);
	new_vec->insert_range_it = VECTOR_FUNC(insert_range_it);
	new_vec->iterator = VECTOR_FUNC(iterator);
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
#endif // VECTOR_LITE
    return new_vec;
}

VECTOR *VECTOR_FUNC(copy_create_vector)(
	const VECTOR *other,
	vector_error_t *error
)
{
	VECTOR_CHECK_INPUT_RANGE(other, error, NULL)

	VECTOR *new_vec = VECTOR_FUNC(create_vector)(
		PRIVATE(other)->__size,
		(T)0,
		error
	);
	VECTOR_CHECK_INPUT_RANGE(new_vec, error, NULL)

	const T *data __attribute__((unused)) = memcpy(
		PRIVATE(new_vec)->__data,
		PRIVATE(other)->__data,
		PRIVATE(other)->__size * sizeof(T)
	);

	VECTOR_CHECK_DATA(data, error, NULL)

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
	
	VECTOR *new_vec = (VECTOR *)malloc(sizeof(VECTOR));

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
	new_vec->find_first_not_of = other_vec->find_first_not_of;
	new_vec->find_last_not_of = other_vec->find_last_not_of;
	new_vec->find_first_of = other_vec->find_first_of;
	new_vec->find_last_of = other_vec->find_last_of;
	new_vec->front = other_vec->front;
	new_vec->index = other_vec->index;
	new_vec->insert_indx = other_vec->insert_indx;
	new_vec->insert_it = other_vec->insert_it;
	new_vec->insert_range_indx = other_vec->insert_range_indx;
	new_vec->insert_range_it = other_vec->insert_range_it;
	new_vec->iterator = other_vec->iterator;
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
#endif // VECTOR_LITE	
	free(other_vec);
	other_vec = NULL;

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;

    return new_vec;
}

void VECTOR_FUNC(destroy_vector)(VECTOR **self)
{
    VECTOR *vec = *self;

    if (self == NULL || vec == NULL)
		return;

    if (PRIVATE(vec)->__data != NULL)
        free(PRIVATE(vec)->__data);

    if (PRIVATE(vec) != NULL)
    	free(PRIVATE(vec));

    free(vec);
    vec = NULL;
    *self = NULL;
}

void VECTOR_FUNC(destroy_vector_with_dtor)(
	VECTOR **self,
	void (*element_destructor)(T *)
)
{
	VECTOR *vec = *self;

    if (self == NULL || vec == NULL || element_destructor == NULL)
		return;

    for (size_t i = 0; i < PRIVATE(vec)->__size; ++i)
        element_destructor(&PRIVATE(vec)->__data[i]);

    VECTOR_FUNC(destroy_vector)(self);
}

static bool VECTOR_FUNC(ensure_capacity)(
	VECTOR **self,
	size_t required,
	vector_error_t *error
)
{
    VECTOR *vec = *self;

    if (required <= PRIVATE(vec)->__allocated_size)
        return true;
    
    size_t new_capacity = PRIVATE(vec)->__allocated_size;
	
    if (new_capacity == 0)
        new_capacity = VECTOR_INITIAL_CAPACITY;
    
    while (new_capacity < required)
        new_capacity *= VECTOR_GROWTH_FACTOR;
    
    const T *new_data = (T *)realloc(
    	PRIVATE(vec)->__data,
		new_capacity * sizeof(T)
    );
	
    VECTOR_CHECK_ALLOC(new_data, error, NULL)
    
    PRIVATE(vec)->__data = (T *)new_data;
    PRIVATE(vec)->__allocated_size = new_capacity;

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
	
    return 1;
}

VECTOR_STATIC void VECTOR_FUNC(append_range)(
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

VECTOR_STATIC void VECTOR_FUNC(assign)(
	VECTOR **self,
	size_t count,
	T value,
	vector_error_t *error
)
{
	VECTOR_CHECK_ENSURE_CAPACITY(self, count, error, )
	
	for (size_t i = 0; i < count; ++i)
		PRIVATE(*self)->__data[i] = value;

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC void VECTOR_FUNC(assign_range)(
	VECTOR **self,
	const VECTOR *range,
	vector_error_t *error
)
{
	const size_t range_size = PRIVATE(range)->__size;
	
	VECTOR_CHECK_INPUT_RANGE(range, error, )
	VECTOR_CHECK_ENSURE_CAPACITY(self, range_size, error, )
	
	for (size_t i = 0; i < range_size; ++i)
		PRIVATE(*self)->__data[i] = PRIVATE(range)->__data[i];

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC inline T VECTOR_FUNC(at)(
	const VECTOR **self,
	size_t index,
	vector_error_t *error
)
{
	VECTOR_CHECK_INDEX(self, index, error, (T)0)

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;

    return PRIVATE(*self)->__data[index];
}

VECTOR_STATIC inline T VECTOR_FUNC(back)(const VECTOR **self)
{
	const VECTOR *vec = *self;

    return PRIVATE(vec)->__data[PRIVATE(vec)->__size - 1];
}

VECTOR_STATIC inline VECTOR_ITERATOR VECTOR_FUNC(begin)(const VECTOR **self)
{
    return PRIVATE(*self)->__data;
}

VECTOR_STATIC inline size_t VECTOR_FUNC(capacity)(const VECTOR **self)
{
    return PRIVATE(*self)->__allocated_size;
}

VECTOR_STATIC inline void VECTOR_FUNC(clear)(VECTOR **const self)
{
    PRIVATE(*self)->__size = 0;
}

VECTOR_STATIC inline T *VECTOR_FUNC(data)(const VECTOR **self)
{
    return PRIVATE(*self)->__data;
}

VECTOR_STATIC inline void VECTOR_FUNC(emplace_indx)(
	VECTOR **self,
	size_t index,
	T value,
	vector_error_t *error
)
{
	VECTOR_CHECK_INDEX((const VECTOR **)self, index, error, )

    PRIVATE(*self)->__data[index] = value;

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC inline void VECTOR_FUNC(emplace_it)(
	VECTOR **self,
	const VECTOR_ITERATOR it,
	T value,
	vector_error_t *error
)
{
	VECTOR_FUNC(emplace_indx)(
		self,
		VECTOR_FUNC(index)((const VECTOR **)self, it, error),
		value,
		error
	);
}

VECTOR_STATIC inline bool VECTOR_FUNC(empty)(const VECTOR **self)
{
    return (PRIVATE(*self)->__size == 0);
}

VECTOR_STATIC inline VECTOR_ITERATOR VECTOR_FUNC(end)(const VECTOR **self)
{
	const VECTOR *vec = *self;

    return PRIVATE(vec)->__data + PRIVATE(vec)->__size;
}

VECTOR_STATIC void VECTOR_FUNC(erase_indx)(
	VECTOR **self,
	size_t index,
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
    
    const T *data __attribute__((unused)) = memmove(
		PRIVATE(vec)->__data + index,
        PRIVATE(vec)->__data + index + 1,
        (PRIVATE(vec)->__size - index - 1) * sizeof(T)
	);
    
    VECTOR_CHECK_DATA(data, error, )

    --PRIVATE(vec)->__size;

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC void VECTOR_FUNC(erase_it)(
	VECTOR **self,
	VECTOR_ITERATOR it,
	vector_error_t *error
)
{
	VECTOR_FUNC(erase_indx)(
		self,
		VECTOR_FUNC(index)((const VECTOR **)self, it, error),
		error
	);
}

VECTOR_STATIC ssize_t VECTOR_FUNC(find_first_not_of)(
	const VECTOR **self,
	T value
)
{
    if (VECTOR_FUNC(empty)(self))
    	return VECTOR_NOT_FOUND_INDEX;

    for (size_t i = 0; i < PRIVATE(*self)->__size; ++i)
        if (VECTOR_FUNC(at)(self, i, NULL) != value)
            return (ssize_t)i;

    return VECTOR_NOT_FOUND_INDEX;
}

VECTOR_STATIC ssize_t VECTOR_FUNC(find_last_not_of)(
	const VECTOR **self,
	T value
)
{
    if (VECTOR_FUNC(empty)(self))
    	return VECTOR_NOT_FOUND_INDEX;

    for (ssize_t i = PRIVATE(*self)->__size - 1; i >= 0; --i)
        if (VECTOR_FUNC(at)(self, i, NULL) != value)
            return i;

    return VECTOR_NOT_FOUND_INDEX;
}

VECTOR_STATIC ssize_t VECTOR_FUNC(find_first_of)(
	const VECTOR **self,
	T value
)
{
    for (size_t i = 0; i < PRIVATE(*self)->__size; ++i)
        if (VECTOR_FUNC(at)(self, i, NULL) == value)
            return (ssize_t)i;

    return VECTOR_NOT_FOUND_INDEX;
}

VECTOR_STATIC ssize_t VECTOR_FUNC(find_last_of)(
	const VECTOR **self,
	T value
)
{
	for (ssize_t i = PRIVATE(*self)->__size - 1; i >= 0; --i)
        if (VECTOR_FUNC(at)(self, i, NULL) == value)
            return i;

    return VECTOR_NOT_FOUND_INDEX;
}

VECTOR_STATIC inline T VECTOR_FUNC(front)(const VECTOR **self)
{
    return PRIVATE(*self)->__data[0];
}

VECTOR_STATIC inline ssize_t VECTOR_FUNC(index)(
	const VECTOR **self,
	const VECTOR_ITERATOR it,
	vector_error_t *error
)
{
	const ssize_t index = (ssize_t)(it - PRIVATE(*self)->__data);

	VECTOR_CHECK_INDEX(self, index, error, VECTOR_NOT_FOUND_INDEX)

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;

	return index;
}

VECTOR_STATIC void VECTOR_FUNC(insert_indx)(
	VECTOR **self,
	size_t before,
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
    
    const T *data __attribute__((unused)) = memmove(
		PRIVATE(vec)->__data + before + 1,
        PRIVATE(vec)->__data + before,
        (PRIVATE(vec)->__size - before) * sizeof(T)
	);

	VECTOR_CHECK_DATA(data, error, )
    
    PRIVATE(vec)->__data[before] = value;
    ++PRIVATE(vec)->__size;

    if (error != NULL)
    	*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC void VECTOR_FUNC(insert_it)(
	VECTOR **self,
	const VECTOR_ITERATOR it_before,
	T value,
	vector_error_t *error
)
{
	VECTOR_FUNC(insert_indx)(
		self,
		VECTOR_FUNC(index)((const VECTOR **)self, it_before, error),
		value,
		error
	);
}

VECTOR_STATIC void VECTOR_FUNC(insert_range_indx)(
	VECTOR **self,
	size_t before,
	const VECTOR *range,
	vector_error_t *error
)
{
	VECTOR_CHECK_INPUT_RANGE(range, error, )
    
    const VECTOR **range_ptr = &range;
    const size_t range_size = VECTOR_FUNC(size)(range_ptr);
    
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
        const T *data __attribute__((unused)) = memmove(
			PRIVATE(vec)->__data + before + range_size,
            PRIVATE(vec)->__data + before,
            (PRIVATE(vec)->__size - before) * sizeof(T)
		);

        VECTOR_CHECK_DATA(data, error, )
    }
    
    for (size_t i = 0; i < range_size; ++i)
        PRIVATE(vec)->__data[before + i] = VECTOR_FUNC(at)(
        	range_ptr,
			i,
			error
		);
    
    PRIVATE(vec)->__size += range_size;

    if (error != NULL)
    	*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC void VECTOR_FUNC(insert_range_it)(
	VECTOR **self,
	const VECTOR_ITERATOR it_before,
	const VECTOR *range,
	vector_error_t *error
)
{
	VECTOR_FUNC(insert_range_indx)(
		self,
		VECTOR_FUNC(index)((const VECTOR **)self, it_before, error),
		range,
		error
	);
}

VECTOR_STATIC inline VECTOR_ITERATOR VECTOR_FUNC(iterator)(
	const VECTOR **self,
	size_t index,
	vector_error_t *error
)
{
	VECTOR_CHECK_INDEX(self, index, error, NULL)

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;
	
	return PRIVATE(*self)->__data + index;
}

VECTOR_STATIC inline void VECTOR_FUNC(pop_back)(VECTOR **self)
{
	VECTOR *vec = *self;

	if (PRIVATE(vec)->__size > 0)
		--PRIVATE(vec)->__size;
}

VECTOR_STATIC inline void VECTOR_FUNC(push_back)(
	VECTOR **self,
	T value,
	vector_error_t *error
)
{	
	VECTOR *vec = *self;

	VECTOR_CHECK_ENSURE_CAPACITY(self, PRIVATE(vec)->__size + 1, error, )
    
    PRIVATE(vec)->__data[PRIVATE(vec)->__size] = value;
    ++PRIVATE(vec)->__size;
    
	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC void VECTOR_FUNC(reserve)(
	VECTOR **self,
	size_t new_capacity,
	vector_error_t *error
)
{
	VECTOR_CHECK_ENSURE_CAPACITY(self, new_capacity, error, )
}

VECTOR_STATIC void VECTOR_FUNC(resize)(
	VECTOR **self,
	size_t new_size,
	vector_error_t *error
)
{
	VECTOR_FUNC(resize_with)(self, new_size, (T)0, error);
}

VECTOR_STATIC void VECTOR_FUNC(resize_with)(
	VECTOR **self,
	size_t new_size,
	T value,
	vector_error_t *error
)
{
    VECTOR *vec = *self;

	VECTOR_CHECK_ENSURE_CAPACITY(self, new_size, error, )
	
	for (size_t i = PRIVATE(vec)->__size; i < new_size; ++i)
		PRIVATE(vec)->__data[i] = value;
	
	PRIVATE(vec)->__size = new_size;

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC void VECTOR_FUNC(reverse_indx)(
	VECTOR **self,
	size_t begin_index,
	size_t end_index,
	vector_error_t *error
)
{
	if (VECTOR_FUNC(empty)((const VECTOR **)self))
		return;

	VECTOR_CHECK_INDEX((const VECTOR **)self, begin_index, error, )
	VECTOR_CHECK_INDEX((const VECTOR **)self, end_index, error, )

	size_t half_index = begin_index + ((end_index - begin_index) >> 1);
	for (size_t i = begin_index; i < half_index; ++i)
		VECTOR_FUNC(swap_indx)(
			self, i,
			begin_index + end_index - 1 - i,
			NULL
		);

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC void VECTOR_FUNC(reverse_it)(
	VECTOR **self,
	VECTOR_ITERATOR itb,
	VECTOR_ITERATOR ite,
	vector_error_t *error
)
{
	VECTOR_FUNC(reverse_indx)(
		self,
		VECTOR_FUNC(index)((const VECTOR **)self, itb, error),
		VECTOR_FUNC(index)((const VECTOR **)self, ite, error),
		error
	);
}

VECTOR_STATIC inline size_t VECTOR_FUNC(size)(const VECTOR **self)
{
    return PRIVATE(*self)->__size;
}

VECTOR_STATIC void VECTOR_FUNC(swap_indx)(
	VECTOR **self,
	size_t index_a,
	size_t index_b,
	vector_error_t *error
)
{
	VECTOR_CHECK_INDEX((const VECTOR **)self, index_a, error, )
	VECTOR_CHECK_INDEX((const VECTOR **)self, index_b, error, )

	VECTOR *vec = *self;
	
	if (index_a == index_b)
		return;
	
    T temp = PRIVATE(vec)->__data[index_a];
    PRIVATE(vec)->__data[index_a] = PRIVATE(vec)->__data[index_b];
    PRIVATE(vec)->__data[index_b] = temp;

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC void VECTOR_FUNC(swap_it)(
	VECTOR **self,
	VECTOR_ITERATOR it_a,
	VECTOR_ITERATOR it_b,
	vector_error_t *error
)
{
	VECTOR_FUNC(swap_indx)(
		self,
		VECTOR_FUNC(index)((const VECTOR **)self, it_a, error),
		VECTOR_FUNC(index)((const VECTOR **)self, it_b, error),
		error
	);
}


#undef VECTOR
#undef VECTOR_FUNC
#undef VECTOR_ITERATOR
#undef PRIVATE
#undef VECTOR_GROWTH_FACTOR
#undef VECTOR_INITIAL_CAPACITY
#undef FAILED_TO_ALLOC_MES


#endif // T
