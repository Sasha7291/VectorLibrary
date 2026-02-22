/**
 * @file      vector.h
 * @brief     Main vector library header
 * @author    Sasha729
 * @version   1.0.1
 * @date      22.02.2026
 *
 * @details   This is the main header file for the vector library.
 *            It initializes vector types and includes the generated declarations.
 *
 * @note      To use this library, include this file in your project.
 * @see       vector_initialize_type.h
 * @see       vector_decl.h
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__


#include "vector_initialize_type.h"


VECTOR_INITIALIZE_TYPE(int)
VECTOR_INITIALIZE_TYPE(char)
VECTOR_INITIALIZE_TYPE(float)


#include "vector_decl.h"


#endif // __VECTOR_H__
