/**
 * @file      template.h
 * @brief     Macros for template function naming
 * @author    Sasha729
 * @version   1.0.1
 * @date      22.02.2026
 *
 * @details   This file provides macros for generating unique names
 *            for template functions based on type parameters.
 */

#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__


/**
 * @def   CAT
 * @brief Concatenate two tokens with an underscore
 *
 * @param X First token
 * @param Y Second token
 * @return  X##_##Y
 */
#define CAT(X, Y) X##_##Y

/**
 * @def   TEMPLATE
 * @brief Generate a template name from base name and type
 *
 * @param X Base name
 * @param Y Type identifier
 * @return  CAT(X, Y)
 */
#define TEMPLATE(X, Y) CAT(X, Y)


#endif // __TEMPLATE_H__ //
