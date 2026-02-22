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
#include "vector_size.h"
#include "vector_error.h"

#include <stdbool.h>


#ifndef VECTOR_T
#define VECTOR_T 				TEMPLATE(vector_t, T)
#endif // VECTOR_T

#ifndef VECTOR
#define VECTOR 					TEMPLATE(vector, T)
#endif // VECTOR

#ifndef VECTOR_FUNC
#define VECTOR_FUNC(_name) 		TEMPLATE(_name, T)
#endif // VECTOR_FUNC

#ifndef VECTOR_ITERATOR
#define VECTOR_ITERATOR 		TEMPLATE(iterator_t, T)
#endif // VECTOR_ITERATOR

#ifndef VECTOR_NOT_FOUND_INDEX
#define VECTOR_NOT_FOUND_INDEX	((ssize_t)-1)
#endif // VECTOR_NOT_FOUND_INDEX


typedef T * TEMPLATE(iterator_t, T);

typedef struct TEMPLATE(vector_private_t, T) TEMPLATE(vector_private, T);

typedef struct VECTOR_T VECTOR;
struct VECTOR_T
{
#ifndef VECTOR_LITE
	void (*append_range)(VECTOR **, const VECTOR *, vector_error_t *);
	void (*assign)(VECTOR **, size_t, T, vector_error_t *);
	void (*assign_range)(VECTOR **self, const VECTOR *, vector_error_t *);
    T (*at)(const VECTOR **, size_t, vector_error_t *);
    T (*back)(const VECTOR **);
    VECTOR_ITERATOR (*begin)(const VECTOR **);
    size_t (*capacity)(const VECTOR **);
    void (*clear)(VECTOR **);
    T *(*data)(const VECTOR **);
    void (*emplace_indx)(VECTOR **, size_t, T, vector_error_t *);
    void (*emplace_it)(
    	VECTOR **,
		const VECTOR_ITERATOR,
		T,
		vector_error_t *
	);
    bool (*empty)(const VECTOR **);
    VECTOR_ITERATOR (*end)(const VECTOR **);
    void (*erase_indx)(VECTOR **, size_t, vector_error_t *);
    void (*erase_it)(VECTOR **, VECTOR_ITERATOR, vector_error_t *);
	ssize_t (*find_first_not_of)(const VECTOR **, T);
	ssize_t (*find_last_not_of)(const VECTOR **, T);
	ssize_t (*find_first_of)(const VECTOR **, T);
	ssize_t (*find_last_of)(const VECTOR **, T);
    T (*front)(const VECTOR **);
    ssize_t (*index)(
    	const VECTOR **,
		const VECTOR_ITERATOR,
		vector_error_t *
	);
    void (*insert_indx)(VECTOR **, size_t, T, vector_error_t *);
    void (*insert_it)(
    	VECTOR **,
		const VECTOR_ITERATOR,
		T,
		vector_error_t *
	);
	void (*insert_range_indx)(
		VECTOR **,
		size_t,
		const VECTOR *,
		vector_error_t *
	);
	void (*insert_range_it)(
		VECTOR **,
		const VECTOR_ITERATOR,
		const VECTOR *,
		vector_error_t *
	);
	VECTOR_ITERATOR (*iterator)(const VECTOR **, size_t, vector_error_t *);
    void (*pop_back)(VECTOR **);
    void (*push_back)(VECTOR **, T, vector_error_t *);
    void (*reserve)(VECTOR **, size_t, vector_error_t *);
    void (*resize)(VECTOR **, size_t, vector_error_t *);
    void (*resize_with)(VECTOR **, size_t, T, vector_error_t *);
    void (*reverse_indx)(VECTOR **, size_t, size_t, vector_error_t *);
    void (*reverse_it)(
    	VECTOR **,
		VECTOR_ITERATOR,
		VECTOR_ITERATOR,
		vector_error_t *
	);
    size_t (*size)(const VECTOR **);
    void (*swap_indx)(VECTOR **, size_t, size_t, vector_error_t *);
    void (*swap_it)(
    	VECTOR **,
		VECTOR_ITERATOR,
		VECTOR_ITERATOR,
		vector_error_t *
	);
#endif // VECTOR_LITE
	
	TEMPLATE(vector_private, T) *__private;
};

VECTOR *VECTOR_FUNC(create_vector)(
	size_t init_size,
	T init_value,
	vector_error_t *
);
VECTOR *VECTOR_FUNC(copy_create_vector)(
	const VECTOR *self,
	vector_error_t *error
);
VECTOR *VECTOR_FUNC(move_create_vector)(VECTOR **self, vector_error_t *);
void VECTOR_FUNC(destroy_vector)(VECTOR **self);
void VECTOR_FUNC(destroy_vector_with_dtor)(
	VECTOR **self,
	void (*element_destructor)(T *)
);

#ifdef VECTOR_LITE
void VECTOR_FUNC(append_range)(VECTOR **self, const VECTOR *range);
void VECTOR_FUNC(assign)(VECTOR **self, size_t count, T value);
void VECTOR_FUNC(assign_range)(VECTOR **self, const VECTOR *range);
inline T VECTOR_FUNC(at)(const VECTOR **self, size_t index);
inline T VECTOR_FUNC(back)(const VECTOR **self);
inline VECTOR_ITERATOR VECTOR_FUNC(begin)(const VECTOR **self);
inline size_t VECTOR_FUNC(capacity)(const VECTOR **self);
void VECTOR_FUNC(clear)(VECTOR **self);
inline T *VECTOR_FUNC(data)(const VECTOR **self);
inline void VECTOR_FUNC(emplace_indx)(VECTOR **self, size_t index, T value);
inline void VECTOR_FUNC(emplace_it)(VECTOR **self, const VECTOR_ITERATOR it, T value);
inline bool VECTOR_FUNC(empty)(const VECTOR **self);
inline VECTOR_ITERATOR VECTOR_FUNC(end)(const VECTOR **self);
void VECTOR_FUNC(erase_indx)(VECTOR **self, size_t index);
void VECTOR_FUNC(erase_it)(VECTOR **self, VECTOR_ITERATOR it);
ssize_t VECTOR_FUNC(find_first_not_of)(const VECTOR **self, T value);
ssize_t VECTOR_FUNC(find_last_not_of)(const VECTOR **self, T value);
ssize_t VECTOR_FUNC(find_first_of)(const VECTOR **self, T value);
ssize_t VECTOR_FUNC(find_last_of)(const VECTOR **self, T value);
inline T VECTOR_FUNC(front)(const VECTOR **self);
inline ssize_t VECTOR_FUNC(index)(const VECTOR **self, const VECTOR_ITERATOR it);
void VECTOR_FUNC(insert_indx)(VECTOR **self, size_t before, T value);
void VECTOR_FUNC(insert_it)(VECTOR **self, const VECTOR_ITERATOR it_before, T value);
void VECTOR_FUNC(insert_range_indx)(VECTOR **self, size_t before, const VECTOR *range);
void VECTOR_FUNC(insert_range_it)(VECTOR **self, const VECTOR_ITERATOR it, const VECTOR *range);
inline VECTOR_ITERATOR VECTOR_FUNC(iterator)(const VECTOR **self, size_t index);
inline void VECTOR_FUNC(pop_back)(VECTOR **self);
inline void VECTOR_FUNC(push_back)(VECTOR **self, T value);
void VECTOR_FUNC(reserve)(VECTOR **self, size_t new_capacity);
void VECTOR_FUNC(resize)(VECTOR **self, size_t new_size);
void VECTOR_FUNC(resize_with)(VECTOR **self, size_t new_size, T value);
void VECTOR_FUNC(reverse_indx)(VECTOR **self, size_t begin_index, size_t end_index);
void VECTOR_FUNC(reverse_it)(VECTOR **self, VECTOR_ITERATOR itb, VECTOR_ITERATOR ite);
inline size_t VECTOR_FUNC(size)(const VECTOR **self);
void VECTOR_FUNC(swap_indx)(VECTOR **self, size_t index_a, size_t index_b);
void VECTOR_FUNC(swap_it)(VECTOR **self, VECTOR_ITERATOR it_a, VECTOR_ITERATOR it_b);
#endif // VECTOR_LITE


#undef VECTOR_T
#undef VECTOR
#undef VECTOR_FUNC
#undef VECTOR_ITERATOR


#endif // T
