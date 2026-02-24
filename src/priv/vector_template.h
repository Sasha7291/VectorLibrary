/*
 * vector_template.h
 *
 *  Created on: 24 февр. 2026 г.
 *      Author: Terentev
 */

#ifndef __VECTOR_TEMPLATE_H__
#define __VECTOR_TEMPLATE_H__

#include "vector_config.h" // только для тестирования, потом убрать


#ifdef VECTOR_NO_DYNAMIC_ALLOC
#include "static_vector_template.h"
#else // VECTOR_NO_DYNAMIC_ALLOC
#include "dynamic_vector_template.h"
#endif // VECTOR_NO_DYNAMIC_ALLOC


#endif // __VECTOR_TEMPLATE_H__
