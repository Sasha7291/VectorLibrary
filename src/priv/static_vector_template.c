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
#include "vector_config.h"
#include "vector_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#ifndef VECTOR
#define VECTOR 				TEMPLATE(vector, T)
#endif // VECTOR

#ifndef VECTOR_FUNC
#define VECTOR_FUNC(_name) 	TEMPLATE(vector, TEMPLATE(_name, T))
#endif // VECTOR_FUNC

#ifndef VECTOR_ITERATOR
#define VECTOR_ITERATOR		TEMPLATE(vector_iterator, T)
#endif // VECTOR_ITERATOR


typedef T * TEMPLATE(vector_iterator, T);


static VECTOR __vector_pool[VECTOR_MAX_N_VECTORS];
static bool __vector_used[VECTOR_MAX_N_VECTORS];
static T __vector_data[VECTOR_MAX_N_VECTORS * VECTOR_STATIC_BUFFER_SIZE];


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
        ((_indx) >= 0) && ((vector_size_t)(_indx) < VECTOR_STATIC_BUFFER_SIZE), \
        _err_ptr, \
        VECTOR_ERROR_INDEX, \
        _ret \
    )
#endif // VECTOR_CHECK_INDEX

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

#ifndef VECTOR_CHECK_INPUT_RANGE
#define VECTOR_CHECK_INPUT_RANGE(_range, _err_ptr, _ret) ;
#endif // VECTOR_CHECK_INPUT_RANGE

#endif // VECTOR_CHECK_ON

#ifndef VECTOR_DATA
#define VECTOR_DATA(_range, _index) \
	(__vector_data[(_range)->__begin_index + (_index)])
#endif // VECTOR_DATA

#ifndef VECTOR_BEGIN
#define VECTOR_BEGIN(_range) \
	(__vector_data + (_range)->__begin_index)
#endif // VECTOR_BEGIN


#ifndef VECTOR_LITE
static VECTOR_INLINE void VECTOR_FUNC(assign)(
    VECTOR *self,
    T value,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(assign_range)(
    VECTOR *self,
    const VECTOR *range,
    vector_error_t *error
);
static VECTOR_INLINE T VECTOR_FUNC(at)(
	const VECTOR *self,
	vector_index_t index,
	vector_error_t *error
);
static VECTOR_INLINE T VECTOR_FUNC(back)(const VECTOR *self);
static VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(begin)(const VECTOR *self);
static VECTOR_INLINE T *VECTOR_FUNC(data)(const VECTOR *self);
static VECTOR_INLINE void VECTOR_FUNC(emplace_indx)(
    VECTOR *self,
	vector_index_t index,
    T value,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(emplace_it)(
    VECTOR *self,
    const VECTOR_ITERATOR it,
    T value,
    vector_error_t *error
);
static VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(end)(const VECTOR *self);
static VECTOR_INLINE vector_index_t VECTOR_FUNC(find_first_not_of)(
	const VECTOR *self,
	T value
);
static VECTOR_INLINE vector_index_t VECTOR_FUNC(find_last_not_of)(
	const VECTOR *self,
	T value
);
static VECTOR_INLINE vector_index_t VECTOR_FUNC(find_first_of)(
	const VECTOR *self,
	T value
);
static VECTOR_INLINE vector_index_t VECTOR_FUNC(find_last_of)(
	const VECTOR *self,
	T value
);
static VECTOR_INLINE T VECTOR_FUNC(front)(const VECTOR *self);
static VECTOR_INLINE vector_index_t VECTOR_FUNC(indx)(
	const VECTOR *self,
    const VECTOR_ITERATOR it,
	vector_error_t *error
);
static VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(it)(
    const VECTOR *self,
	vector_index_t index,
	vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(reverse_indx)(
    VECTOR *self,
	vector_index_t begin_index,
	vector_index_t end_index,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(reverse_it)(
    VECTOR *self,
    VECTOR_ITERATOR itb,
    VECTOR_ITERATOR ite,
    vector_error_t *error
);
static VECTOR_INLINE vector_size_t VECTOR_FUNC(size)(const VECTOR *self);
static VECTOR_INLINE void VECTOR_FUNC(swap_indx)(
    VECTOR *self,
	vector_index_t index_a,
	vector_index_t index_b,
    vector_error_t *error
);
static VECTOR_INLINE void VECTOR_FUNC(swap_it)(
    VECTOR *self,
    VECTOR_ITERATOR it_a,
    VECTOR_ITERATOR it_b,
    vector_error_t *error
);
#endif // VECTOR_LITE


VECTOR *VECTOR_FUNC(create_vector)(T init_value, vector_error_t *error)
{
	for (vector_index_t i = 0; i < VECTOR_MAX_STATIC_VECTORS; ++i)
	{
	    if (!__vector_used[i])
	    {
	        __vector_used[i] = true;
	        VECTOR *new_vec = &__vector_pool[i];
	        *new_vec = (VECTOR){ 
				.__begin_index = i * VECTOR_MAX_STATIC_VECTORS 
			};

	        for (vector_index_t j = 0; j < VECTOR_STATIC_BUFFER_SIZE; ++j)
				VECTOR_DATA(new_vec, j) = init_value;

#ifndef VECTOR_LITE
			new_vec->assign = VECTOR_FUNC(assign);
			new_vec->assign_range = VECTOR_FUNC(assign_range);
			new_vec->at = VECTOR_FUNC(at);
			new_vec->back = VECTOR_FUNC(back);
			new_vec->begin = VECTOR_FUNC(begin);
			new_vec->data = VECTOR_FUNC(data);
			new_vec->emplace_indx = VECTOR_FUNC(emplace_indx);
			new_vec->emplace_it = VECTOR_FUNC(emplace_it);
			new_vec->end = VECTOR_FUNC(end);
			new_vec->find_first_not_of = VECTOR_FUNC(find_first_not_of);
			new_vec->find_last_not_of = VECTOR_FUNC(find_last_not_of);
			new_vec->find_first_of = VECTOR_FUNC(find_first_of);
			new_vec->find_last_of = VECTOR_FUNC(find_last_of);
			new_vec->front = VECTOR_FUNC(front);
			new_vec->indx = VECTOR_FUNC(indx);
			new_vec->it = VECTOR_FUNC(it);
			new_vec->reverse_indx = VECTOR_FUNC(reverse_indx);
			new_vec->reverse_it = VECTOR_FUNC(reverse_it);
			new_vec->size = VECTOR_FUNC(size);
			new_vec->swap_indx = VECTOR_FUNC(swap_indx);
			new_vec->swap_it = VECTOR_FUNC(swap_it);
#endif // VECTOR_LITE

	        if (error)
	        	*error = VECTOR_SUCCESS;

	        return new_vec;
	    }
	}

	if (error)
		*error = VECTOR_ERROR_NO_FREE_VECTOR;

	return NULL;
}

VECTOR *VECTOR_FUNC(copy_create_vector)(
	const VECTOR *other,
	vector_error_t *error
)
{
	VECTOR_CHECK_INPUT_RANGE(other, error, NULL)

	for (vector_index_t i = 0; i < VECTOR_MAX_STATIC_VECTORS; ++i)
	{
	    if (!__vector_used[i])
	    {
	        __vector_used[i] = true;
			
	        VECTOR *new_vec = &__vector_pool[i];
	        (*new_vec) = (VECTOR){ 
				.__begin_index = i * VECTOR_MAX_STATIC_VECTORS 
			};

	    	memcpy(
				VECTOR_BEGIN(new_vec)
				VECTOR_BEGIN(other),
				VECTOR_STATIC_BUFFER_SIZE
	    	);

#ifndef VECTOR_LITE
			new_vec->assign = other->assign;
			new_vec->assign_range = other->assign_range;
			new_vec->at = other->at;
			new_vec->back = other->back;
			new_vec->begin = other->begin;
			new_vec->data = other->data;
			new_vec->emplace_indx = other->emplace_indx;
			new_vec->emplace_it = other->emplace_it;
			new_vec->end = other->end;
			new_vec->find_first_not_of = other->find_first_not_of;
			new_vec->find_last_not_of = other->find_last_not_of;
			new_vec->find_first_of = other->find_first_of;
			new_vec->find_last_of = other->find_last_of;
			new_vec->front = other->front;
			new_vec->indx = other->indx;
			new_vec->it = other->it;
			new_vec->reverse_indx = other->reverse_indx;
			new_vec->reverse_it = other->reverse_it;
			new_vec->size = other->size;
			new_vec->swap_indx = other->swap_indx;
			new_vec->swap_it = other->swap_it;
#endif // VECTOR_LITE

			if (error)
				*error = VECTOR_SUCCESS;

			return new_vec;
		}
	}

	if (error)
		*error = VECTOR_ERROR_NO_FREE_VECTOR;

	return NULL;
}

VECTOR *VECTOR_FUNC(move_create_vector)(
	VECTOR *other,
	vector_error_t *error
)
{
	VECTOR_CHECK_INPUT_RANGE(other, error, NULL)

	if (error)
		*error = VECTOR_SUCCESS;

	return other;
}

void VECTOR_FUNC(destroy_vector)(VECTOR *self)
{
	if (self == NULL)
		return;

	__vector_used[self->__begin_index] = false;
}


VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(assign)(
	VECTOR *self,
	T value,
	vector_error_t *error
)
{
    for (vector_index_t i = 0; i < VECTOR_STATIC_BUFFER_SIZE; ++i)
		VECTOR_DATA(self, i) = value;

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(assign_range)(
	VECTOR *self,
	const VECTOR *range,
	vector_error_t *error
)
{
	VECTOR_CHECK_INPUT_RANGE(range, error, )

	memcpy(
		VECTOR_BEGIN(self),
		VECTOR_BEGIN(range),
		VECTOR_STATIC_BUFFER_SIZE
	);

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE T VECTOR_FUNC(at)(
	const VECTOR *self,
	vector_index_t index,
	vector_error_t *error
)
{
	VECTOR_CHECK_INDEX(self, index, error, (T)0)

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;

    return VECTOR_DATA(self, index);
}

VECTOR_STATIC VECTOR_INLINE T VECTOR_FUNC(back)(const VECTOR *self)
{
	return VECTOR_DATA(self, VECTOR_STATIC_BUFFER_SIZE - 1);
}

VECTOR_STATIC VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(begin)(const VECTOR *self)
{
    return VECTOR_BEGIN(self);
}

VECTOR_STATIC VECTOR_INLINE T *VECTOR_FUNC(data)(const VECTOR *self)
{
	return VECTOR_BEGIN(self);
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(emplace_indx)(
	VECTOR *self,
	vector_index_t index,
	T value,
	vector_error_t *error
)
{
	VECTOR_CHECK_INDEX((const VECTOR *)self, index, error, )

	VECTOR_DATA(self, index) = value;

	if (error != NULL)
		(*error) = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(emplace_it)(
	VECTOR *self,
	const VECTOR_ITERATOR it,
	T value,
	vector_error_t *error
)
{
	VECTOR_FUNC(emplace_indx)(
		self,
		VECTOR_FUNC(indx)((const VECTOR *)self, it, error),
		value,
		error
	);
}

VECTOR_STATIC VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(end)(const VECTOR *self)
{
	return VECTOR_BEGIN(self) + VECTOR_STATIC_BUFFER_SIZE;
}

VECTOR_STATIC VECTOR_INLINE vector_index_t VECTOR_FUNC(find_first_not_of)(
	const VECTOR *self,
	T value
)
{
    for (vector_index_t i = 0; i < VECTOR_STATIC_BUFFER_SIZE; ++i)
        if (VECTOR_DATA(self, i) != value)
            return i;

    return VECTOR_INVALID_INDEX;
}

VECTOR_STATIC VECTOR_INLINE vector_index_t VECTOR_FUNC(find_last_not_of)(
	const VECTOR *self,
	T value
)
{
    for (vector_index_t i = VECTOR_STATIC_BUFFER_SIZE - 1; i >= 0; --i)
        if (VECTOR_DATA(self, i) != value)
            return i;

    return VECTOR_INVALID_INDEX;
}

VECTOR_STATIC VECTOR_INLINE vector_index_t VECTOR_FUNC(find_first_of)(
	const VECTOR *self,
	T value
)
{
    for (vector_index_t i = 0; i < VECTOR_STATIC_BUFFER_SIZE; ++i)
        if (VECTOR_DATA(self, i) == value)
            return i;

    return VECTOR_INVALID_INDEX;
}

VECTOR_STATIC VECTOR_INLINE vector_index_t VECTOR_FUNC(find_last_of)(
	const VECTOR *self,
	T value
)
{
	for (vector_index_t i = VECTOR_STATIC_BUFFER_SIZE - 1; i >= 0; --i)
        if (VECTOR_DATA(self, i) == value)
            return i;

    return VECTOR_INVALID_INDEX;
}

VECTOR_STATIC VECTOR_INLINE T VECTOR_FUNC(front)(const VECTOR *self)
{
    return VECTOR_DATA(self, 0);
}

VECTOR_STATIC VECTOR_INLINE vector_index_t VECTOR_FUNC(indx)(
	const VECTOR *self,
	const VECTOR_ITERATOR it,
	vector_error_t *error
)
{
	const vector_index_t index =
		(vector_index_t)(it - VECTOR_BEGIN(self));

	VECTOR_CHECK_INDEX(self, index, error, VECTOR_INVALID_INDEX)

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;

	return index;
}

VECTOR_STATIC VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(it)(
	const VECTOR *self,
	vector_index_t index,
	vector_error_t *error
)
{
	VECTOR_CHECK_INDEX(self, index, error, NULL)

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;

	return VECTOR_BEGIN(self) + index;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(reverse_indx)(
	VECTOR *self,
	vector_index_t begin_index,
	vector_index_t end_index,
	vector_error_t *error
)
{
	if (begin_index == end_index)
	{
		if (error != NULL)
			*error = VECTOR_ERROR_SUCCESS;
		
		return;
	}
	
	VECTOR_CHECK_INDEX((const VECTOR *)self, begin_index, error, )
	VECTOR_CHECK_INDEX((const VECTOR *)self, end_index, error, )

	T temp;
	while (begin_index < end_index)
	{
		temp = VECTOR_DATA(self, begin_index);
		VECTOR_DATA(self, begin_index) = VECTOR_DATA(self, end_index);
		VECTOR_DATA(self, end_index) = temp;
		
		++begin_index;
		--end_index;
	}

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(reverse_it)(
	VECTOR *self,
	VECTOR_ITERATOR itb,
	VECTOR_ITERATOR ite,
	vector_error_t *error
)
{
	VECTOR_FUNC(reverse_indx)(
		self,
		VECTOR_FUNC(indx)((const VECTOR *)self, itb, error),
		VECTOR_FUNC(indx)((const VECTOR *)self, ite, error),
		error
	);
}

VECTOR_STATIC VECTOR_INLINE vector_size_t VECTOR_FUNC(size)(const VECTOR *self)
{
    return VECTOR_STATIC_BUFFER_SIZE;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(swap_indx)(
	VECTOR *self,
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
	
	VECTOR_CHECK_INDEX((const VECTOR *)self, index_a, error, )
	VECTOR_CHECK_INDEX((const VECTOR *)self, index_b, error, )

    const T temp = VECTOR_DATA(self, index_a);
    VECTOR_DATA(self, index_a) = VECTOR_DATA(self, index_b);
    VECTOR_DATA(self, index_b) = temp;

	if (error != NULL)
		*error = VECTOR_ERROR_SUCCESS;
}

VECTOR_STATIC VECTOR_INLINE void VECTOR_FUNC(swap_it)(
	VECTOR *self,
	VECTOR_ITERATOR it_a,
	VECTOR_ITERATOR it_b,
	vector_error_t *error
)
{
	VECTOR_FUNC(swap_indx)(
		self,
		VECTOR_FUNC(indx)((const VECTOR *)self, it_a, error),
		VECTOR_FUNC(indx)((const VECTOR *)self, it_b, error),
		error
	);
}


#undef VECTOR
#undef VECTOR_FUNC
#undef VECTOR_ITERATOR
#undef PRIVATE


#endif // T

