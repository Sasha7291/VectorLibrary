/*
 * vector_template.c
 *
 *  Created on: 24 февр. 2026 г.
 *      Author: Terentev
 */

#include "vector_config.h" // только для тестирования, потом убрать


#ifdef VECTOR_NO_DYNAMIC_ALLOC
#include "static_vector_template.с"
#else // VECTOR_NO_DYNAMIC_ALLOC
#include "dynamic_vector_template.с"
#endif // VECTOR_NO_DYNAMIC_ALLOC
