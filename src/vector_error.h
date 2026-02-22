/**
 * @file      vector_error.h
 * @brief     Error codes for vector operations
 * @author    Sasha729
 * @version   1.0.1
 * @date      22.02.2026
 *
 * @details   This file defines error codes used throughout the vector library
 *            to indicate operation success or failure.
 */

#ifndef __VECTOR_ERROR_H__
#define __VECTOR_ERROR_H__


/**
 * @enum   vector_error
 * @brief  Error codes for vector operations
 *
 * @var    VECTOR_ERROR_SUCCESS		Operation completed successfully
 * @var    VECTOR_ERROR_ALLOC		Memory allocation failed
 * @var    VECTOR_ERROR_INDEX  		Index out of bounds
 * @var    VECTOR_ERROR_NULL   		NULL pointer encountered
 * @var    VECTOR_ERROR_CAPACITY  	Capacity operation failed
 * @var    VECTOR_ERROR_SIZE   		Size operation failed
 */
typedef enum vector_error
{
    VECTOR_ERROR_SUCCESS = 0,
    VECTOR_ERROR_ALLOC,
    VECTOR_ERROR_INDEX,
    VECTOR_ERROR_NULL,
    VECTOR_ERROR_CAPACITY,
    VECTOR_ERROR_SIZE
} vector_error_t;


#endif // __VECTOR_ERROR_H__
