/*
 * static_vector_template.h
 *
 *  Created on: 24 февр. 2026 г.
 *      Author: Terentev
 */

#ifndef __STATIC_VECTOR_TEMPLATE_H__
#define __STATIC_VECTOR_TEMPLATE_H__

#ifdef T

#include "template.h"
#include "vector_config.h"
#include "vector_error.h"

#include <stdbool.h>


#ifndef VECTOR_T
#define VECTOR_T 				TEMPLATE(vector_t, T)
#endif // VECTOR_T

#ifndef VECTOR
#define VECTOR 					TEMPLATE(vector, T)
#endif // VECTOR

#ifndef VECTOR_FUNC
#define VECTOR_FUNC(_name) 		TEMPLATE(vector, TEMPLATE(_name, T))
#endif // VECTOR_FUNC

#ifndef VECTOR_ITERATOR
#define VECTOR_ITERATOR 		TEMPLATE(vector_iterator, T)
#endif // VECTOR_ITERATOR


typedef T * TEMPLATE(vector_iterator, T);

typedef struct TEMPLATE(vector_private_t, T) TEMPLATE(vector_private, T);

typedef struct VECTOR_T VECTOR;
struct VECTOR_T VECTOR_PACKED_STRUCT
{
#ifndef VECTOR_LITE
	void (*assign)(VECTOR *, T, vector_error_t *);
	void (*assign_range)(VECTOR *self, const VECTOR *, vector_error_t *);
    T (*at)(const VECTOR *, vector_index_t, vector_error_t *);
    T (*back)(const VECTOR *);
    VECTOR_ITERATOR (*begin)(const VECTOR *);
    T *(*data)(const VECTOR *);
   void (*emplace_indx)(
		VECTOR *, 
		vector_index_t, 
		T, 
		vector_error_t *
	);
    void (*emplace_it)(
    	VECTOR *,
		const VECTOR_ITERATOR,
		T,
		vector_error_t *
	);
    VECTOR_ITERATOR (*end)(const VECTOR *);
	vector_index_t (*find_first_not_of)(const VECTOR *, T);
	vector_index_t (*find_last_not_of)(const VECTOR *, T);
	vector_index_t (*find_first_of)(const VECTOR *, T);
	vector_index_t (*find_last_of)(const VECTOR *, T);
    T (*front)(const VECTOR *);
    vector_index_t (*indx)(
    	const VECTOR *,
		const VECTOR_ITERATOR,
		vector_error_t *
	);
	VECTOR_ITERATOR (*it)(
		const VECTOR *,
		vector_index_t,
		vector_error_t *
	);
    void (*reverse_indx)(
    	VECTOR *,
		vector_index_t,
		vector_index_t,
		vector_error_t *
	);
    void (*reverse_it)(
    	VECTOR *,
		VECTOR_ITERATOR,
		VECTOR_ITERATOR,
		vector_error_t *
	);
    vector_size_t (*size)(const VECTOR *);
    void (*swap_indx)(
    	VECTOR *,
		vector_index_t,
		vector_index_t,
		vector_error_t *
	);
    void (*swap_it)(
    	VECTOR *,
		VECTOR_ITERATOR,
		VECTOR_ITERATOR,
		vector_error_t *
	);
#endif // VECTOR_LITE

    const uint64_t __begin_index;
};

VECTOR *VECTOR_FUNC(create_vector)(T init_value, vector_error_t *error);
VECTOR *VECTOR_FUNC(copy_create_vector)(
	const VECTOR *self,
	vector_error_t *error
);
VECTOR *VECTOR_FUNC(move_create_vector)(
	VECTOR *self,
	vector_error_t *error
);
void VECTOR_FUNC(destroy_vector)(VECTOR *self);

#ifdef VECTOR_LITE
VECTOR_INLINE void VECTOR_FUNC(assign)(
    VECTOR *self,
    T value,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(assign_range)(
    VECTOR *self,
    const VECTOR *range,
    vector_error_t *error
);
VECTOR_INLINE T VECTOR_FUNC(at)(
	const VECTOR *self,
	vector_index_t index,
	vector_error_t *error
);
VECTOR_INLINE T VECTOR_FUNC(back)(const VECTOR *self);
VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(begin)(const VECTOR *self);
VECTOR_INLINE T *VECTOR_FUNC(data)(const VECTOR *self);
VECTOR_INLINE void VECTOR_FUNC(emplace_indx)(
    VECTOR *self,
	vector_index_t index,
    T value,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(emplace_it)(
    VECTOR *self,
    const VECTOR_ITERATOR it,
    T value,
    vector_error_t *error
);
VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(end)(const VECTOR *self);
VECTOR_INLINE vector_index_t VECTOR_FUNC(find_first_not_of)(
	const VECTOR *self,
	T value
);
VECTOR_INLINE vector_index_t VECTOR_FUNC(find_last_not_of)(
	const VECTOR *self,
	T value
);
VECTOR_INLINE vector_index_t VECTOR_FUNC(find_first_of)(
	const VECTOR *self,
	T value
);
VECTOR_INLINE vector_index_t VECTOR_FUNC(find_last_of)(
	const VECTOR *self,
	T value
);
VECTOR_INLINE T VECTOR_FUNC(front)(const VECTOR *self);
VECTOR_INLINE vector_index_t VECTOR_FUNC(indx)(
	const VECTOR *self,
    const VECTOR_ITERATOR it,
	vector_error_t *error
);
VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(it)(
    const VECTOR *self,
	vector_index_t index,
	vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(reverse_indx)(
    VECTOR *self,
	vector_index_t begin_index,
	vector_index_t end_index,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(reverse_it)(
    VECTOR *self,
    VECTOR_ITERATOR itb,
    VECTOR_ITERATOR ite,
    vector_error_t *error
);
VECTOR_INLINE vector_size_t VECTOR_FUNC(size)(const VECTOR *self);
VECTOR_INLINE void VECTOR_FUNC(swap_indx)(
    VECTOR *self,
	vector_index_t index_a,
	vector_index_t index_b,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(swap_it)(
    VECTOR *self,
    VECTOR_ITERATOR it_a,
    VECTOR_ITERATOR it_b,
    vector_error_t *error
);
#endif // VECTOR_LITE


#undef VECTOR_T
#undef VECTOR
#undef VECTOR_FUNC
#undef VECTOR_ITERATOR


#endif // T

#endif // __STATIC_VECTOR_TEMPLATE_H__
