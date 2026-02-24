/**
 * @file      vector_template.h
 * @brief     Template declarations for vector structure and methods
 * @author    Sasha729
 * @version   1.0.1
 * @date      22.02.2026
 *
 * @details   This header file declares the vector structure and its methods
 *            as a template. It is included multiple times with different T
 *            definitions to generate type-specific declarations.
 *
 * @note      When adding custom types, note that the vector does not clean up
 *            memory inside objects - it only deletes the objects themselves.
 *            Use destructor functions for complex types.
 *
 * @see       vector_template.c
 */

#ifdef T

#include "template.h"
#include "vector_allocator.h"
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
	void (*append_range)(VECTOR **, const VECTOR *, vector_error_t *);
	void (*assign)(VECTOR **, vector_size_t, T, vector_error_t *);
	void (*assign_range)(VECTOR **self, const VECTOR *, vector_error_t *);
    T (*at)(const VECTOR **, vector_size_t, vector_error_t *);
    T (*back)(const VECTOR **);
    VECTOR_ITERATOR (*begin)(const VECTOR **);
    vector_size_t (*capacity)(const VECTOR **);
    void (*clear)(VECTOR **);
    T *(*data)(const VECTOR **);
    void (*emplace_indx)(VECTOR **, vector_size_t, T, vector_error_t *);
    void (*emplace_it)(
    	VECTOR **,
		const VECTOR_ITERATOR,
		T,
		vector_error_t *
	);
    bool (*empty)(const VECTOR **);
    VECTOR_ITERATOR (*end)(const VECTOR **);
    void (*erase_indx)(VECTOR **, vector_size_t, vector_error_t *);
    void (*erase_it)(VECTOR **, VECTOR_ITERATOR, vector_error_t *);
	vector_index_t (*find_first_not_of)(const VECTOR **, T);
	vector_index_t (*find_last_not_of)(const VECTOR **, T);
	vector_index_t (*find_first_of)(const VECTOR **, T);
	vector_index_t (*find_last_of)(const VECTOR **, T);
    T (*front)(const VECTOR **);
    vector_index_t (*indx)(
    	const VECTOR **,
		const VECTOR_ITERATOR,
		vector_error_t *
	);
    void (*insert_indx)(VECTOR **, vector_size_t, T, vector_error_t *);
    void (*insert_it)(
    	VECTOR **,
		const VECTOR_ITERATOR,
		T,
		vector_error_t *
	);
	void (*insert_range_indx)(
		VECTOR **,
		vector_size_t,
		const VECTOR *,
		vector_error_t *
	);
	void (*insert_range_it)(
		VECTOR **,
		const VECTOR_ITERATOR,
		const VECTOR *,
		vector_error_t *
	);
	VECTOR_ITERATOR (*it)(
		const VECTOR **,
		vector_size_t,
		vector_error_t *
	);
    void (*pop_back)(VECTOR **);
    void (*push_back)(VECTOR **, T, vector_error_t *);
    void (*reserve)(VECTOR **, vector_size_t, vector_error_t *);
    void (*resize)(VECTOR **, vector_size_t, vector_error_t *);
    void (*resize_with)(VECTOR **, vector_size_t, T, vector_error_t *);
    void (*reverse_indx)(
    	VECTOR **,
		vector_size_t,
		vector_size_t,
		vector_error_t *
	);
    void (*reverse_it)(
    	VECTOR **,
		VECTOR_ITERATOR,
		VECTOR_ITERATOR,
		vector_error_t *
	);
    vector_size_t (*size)(const VECTOR **);
    void (*swap_indx)(
    	VECTOR **,
		vector_size_t,
		vector_size_t,
		vector_error_t *
	);
    void (*swap_it)(
    	VECTOR **,
		VECTOR_ITERATOR,
		VECTOR_ITERATOR,
		vector_error_t *
	);

#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
    const vector_allocator_t *(*allocator)(const VECTOR **);
	void (*set_allocator)(
		const VECTOR **,
		vector_allocator_t *,
		vector_error_t *
	);
#endif // VECTOR_USE_CUSTOM_ALLOCATOR

#endif // VECTOR_LITE
	
	TEMPLATE(vector_private, T) *__private;
};

VECTOR *VECTOR_FUNC(create_vector)(
#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
	const vector_allocator_t *alloc,
#endif // VECTOR_USE_CUSTOM_ALLOCATOR
	vector_size_t init_size,
	T init_value,
	vector_error_t *
);
VECTOR *VECTOR_FUNC(copy_create_vector)(
	const VECTOR *self,
	vector_error_t *error
);
VECTOR *VECTOR_FUNC(move_create_vector)(VECTOR **self, vector_error_t *);
void VECTOR_FUNC(destroy_vector)(VECTOR **self);

#ifdef VECTOR_LITE
VECTOR_INLINE void VECTOR_FUNC(append_range)(
    VECTOR **self,
    const VECTOR *range,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(assign)(
    VECTOR **self,
    vector_size_t count,
    T value,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(assign_range)(
    VECTOR **self,
    const VECTOR *range,
    vector_error_t *error
);
VECTOR_INLINE T VECTOR_FUNC(at)(
	const VECTOR **self,
	vector_size_t index,
	vector_error_t *error
);
VECTOR_INLINE T VECTOR_FUNC(back)(const VECTOR **self);
VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(begin)(const VECTOR **self);
VECTOR_INLINE vector_size_t VECTOR_FUNC(capacity)(const VECTOR **self);
VECTOR_INLINE void VECTOR_FUNC(clear)(VECTOR **self);
VECTOR_INLINE T *VECTOR_FUNC(data)(const VECTOR **self);
VECTOR_INLINE void VECTOR_FUNC(emplace_indx)(
    VECTOR **self,
    vector_size_t index,
    T value,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(emplace_it)(
    VECTOR **self,
    const VECTOR_ITERATOR it,
    T value,
    vector_error_t *error
);
VECTOR_INLINE bool VECTOR_FUNC(empty)(const VECTOR **self);
VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(end)(const VECTOR **self);
VECTOR_INLINE void VECTOR_FUNC(erase_indx)(
    VECTOR **self,
    vector_size_t index,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(erase_it)(
    VECTOR **self,
    VECTOR_ITERATOR it,
    vector_error_t *error
);
VECTOR_INLINE vector_index_t VECTOR_FUNC(find_first_not_of)(
	const VECTOR **self,
	T value
);
VECTOR_INLINE vector_index_t VECTOR_FUNC(find_last_not_of)(
	const VECTOR **self,
	T value
);
VECTOR_INLINE vector_index_t VECTOR_FUNC(find_first_of)(
	const VECTOR **self,
	T value
);
VECTOR_INLINE vector_index_t VECTOR_FUNC(find_last_of)(
	const VECTOR **self,
	T value
);
VECTOR_INLINE T VECTOR_FUNC(front)(const VECTOR **self);
VECTOR_INLINE vector_index_t VECTOR_FUNC(indx)(
	const VECTOR **self,
    const VECTOR_ITERATOR it,
	vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(insert_indx)(
    VECTOR **self,
    vector_size_t before,
    T value,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(insert_it)(
    VECTOR **self,
    const VECTOR_ITERATOR it_before,
    T value,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(insert_range_indx)(
    VECTOR **self,
    vector_size_t before,
    const VECTOR *range,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(insert_range_it)(
    VECTOR **self,
    const VECTOR_ITERATOR it,
    const VECTOR *range,
    vector_error_t *error
);
VECTOR_INLINE VECTOR_ITERATOR VECTOR_FUNC(it)(
    const VECTOR **self,
    vector_size_t index,
	vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(pop_back)(VECTOR **self);
VECTOR_INLINE void VECTOR_FUNC(push_back)(
    VECTOR **self,
    T value,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(reserve)(
    VECTOR **self,
    vector_size_t new_capacity,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(resize)(
    VECTOR **self,
    vector_size_t new_size,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(resize_with)(
    VECTOR **self,
    vector_size_t new_size,
    T value,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(reverse_indx)(
    VECTOR **self,
    vector_size_t begin_index,
    vector_size_t end_index,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(reverse_it)(
    VECTOR **self,
    VECTOR_ITERATOR itb,
    VECTOR_ITERATOR ite,
    vector_error_t *error
);
VECTOR_INLINE vector_size_t VECTOR_FUNC(size)(const VECTOR **self);
VECTOR_INLINE void VECTOR_FUNC(swap_indx)(
    VECTOR **self,
    vector_size_t index_a,
    vector_size_t index_b,
    vector_error_t *error
);
VECTOR_INLINE void VECTOR_FUNC(swap_it)(
    VECTOR **self,
    VECTOR_ITERATOR it_a,
    VECTOR_ITERATOR it_b,
    vector_error_t *error
);

#ifdef VECTOR_USE_CUSTOM_ALLOCATOR
const vector_allocator_t *VECTOR_FUNC(allocator)(const VECTOR **self);
void VECTOR_FUNC(set_allocator)(
	const VECTOR **self,
	vector_allocator_t *alloc,
	vector_error_t error*
);
#endif // VECTOR_USE_CUSTOM_ALLOCATOR
#endif // VECTOR_LITE


#undef VECTOR_T
#undef VECTOR
#undef VECTOR_FUNC
#undef VECTOR_ITERATOR


#endif // T
