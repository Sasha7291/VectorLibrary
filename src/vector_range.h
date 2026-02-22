/**
 * @file      vector_range.h
 * @brief     Range-based iteration macros for vectors
 * @author    Sasha729
 * @version   1.0.1
 * @date      22.02.2026
 *
 * @details   This file provides macros for convenient iteration over vectors
 *            using both iterators and indices, with bounds checking support.
 */

#ifndef __VECTOR_RANGE_H__
#define __VECTOR_RANGE_H__


#include "vector_size.h"


#ifndef VECTOR_RANGE_CHECK
#define VECTOR_RANGE_CHECK(_range) \
    for (int _ok = ((_range) != NULL); _ok != 0; _ok = 0)
#endif // VECTOR_RANGE_CHECK

#ifndef VECTOR_RANGE_IT_CHECK
#define VECTOR_RANGE_IT_CHECK(_range, _itb, _ite) \
    for ( \
        int _ok = ((_itb) < (_ite) \
            && (_itb) >= (_range)->begin(&(_range)) \
            && (_ite) <= (_range)->end(&(_range))); \
        _ok != 0; \
        _ok = 0 \
    )
#endif // VECTOR_RANGE_IT_CHECK

#ifndef VECTOR_RANGE_INDX_CHECK
#define VECTOR_RANGE_INDX_CHECK(_range, _indxb, _indxe) \
    do { \
        const typeof((_range)->begin(&(_range))) _itb \
            = (_range)->iterator(&(_range), _indxb); \
        const typeof((_range)->end(&(_range))) _ite \
            = (_range)->iterator(&(_range), _indxe); \
        \
        VECTOR_RANGE_IT_CHECK(_range, _itb, _ite); \
    } while (0)
#endif // VECTOR_RANGE_INDX_CHECK


/**
 * @def   VECTOR_FOREACH_IT
 * @brief Iterate over vector using iterators
 *
 * @param _range	Vector pointer (VECTOR **)
 * @param _itb    	Begin iterator
 * @param _ite    	End iterator
 * @param _it     	Iterator variable name
 */
#ifndef VECTOR_FOREACH_IT
#define VECTOR_FOREACH_IT(_range, _itb, _ite, _it) \
	VECTOR_RANGE_CHECK(_range) \
	VECTOR_RANGE_IT_CHECK(_range, _itb, _ite) \
	for (_it = _itb; _it != _ite; ++_it)
#endif // VECTOR_FOREACH_IT

/**
 * @def   VECTOR_FOREACH_IT_R
 * @brief Reverse iterate over vector using iterators
 *
 * @param _range  	Vector pointer (VECTOR **)
 * @param _ritb	Begin reverse iterator
 * @param _rite    End reverse iterator
 * @param _it     	Iterator variable name
 */
#ifndef VECTOR_FOREACH_IT_R
#define VECTOR_FOREACH_IT_R(_range, _ritb, _rite, _it) \
	VECTOR_RANGE_CHECK(_range) \
	VECTOR_RANGE_IT_CHECK(_range, _rite, _ritb) \
	for (_it = _ritb; _it != _rite; --_it)
#endif // VECTOR_FOREACH_IT_R

/**
 * @def   VECTOR_FOREACH_INDX
 * @brief Iterate over vector using indices
 *
 * @param _range  	Vector pointer (VECTOR **)
 * @param _indxb	Begin index
 * @param _indxe	End index
 * @param _it     	Iterator variable name
 */
#ifndef VECTOR_FOREACH_INDX
#define VECTOR_FOREACH_INDX(_range, _indxb, _indxe, _it) \
	VECTOR_RANGE_CHECK(_range) \
	VECTOR_RANGE_INDX_CHECK(_range, _indxb, _indxe) \
	_it = _range->iterator(&_range, _indxb) \
	for (ssize_t _indx = _itb; _indx < _indxe; ++_indx, ++_it)
#endif // VECTOR_FOREACH_INDX

/**
 * @def   VECTOR_FOREACH_INDX_R
 * @brief Reverse iterate over vector using indices
 *
 * @param _range  	Vector pointer (VECTOR **)
 * @param _rindxb	Begin reverse index
 * @param _rindxe   End reverse index
 * @param _it     	Iterator variable name
 */
#ifndef VECTOR_FOREACH_INDX_R
#define VECTOR_FOREACH_INDX_R(_range, _rindxb, _rindxe, _it) \
	VECTOR_RANGE_CHECK(_range) \
	VECTOR_RANGE_INDX_CHECK(_range, _rindxe, _rindxb) \
	_it = _range->iterator(&_range, _rindxb); \
	for (ssize_t _indx = _rindxb; _indx < _rindxe; --_indx, --_it)
#endif // VECTOR_FOREACH_INDX_R


#endif // __VECTOR_RANGE_H__
