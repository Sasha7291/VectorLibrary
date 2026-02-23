# ==================== VectorLibraryConfig.cmake ====================
get_filename_component(VECTOR_LIB_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(VECTOR_LIB_ROOT_DIR "${VECTOR_LIB_CMAKE_DIR}/.." ABSOLUTE)

# ==================== VERSION ====================
set(VECTOR_PROJECT_NAME     			"VectorLibrary")
string(TOLOWER ${VECTOR_PROJECT_NAME} 	VECTOR_PROJECT_NAMESPACE)
set(${VECTOR_PROJECT_NAME}_VERSION 		1.0.1)

# ==================== PATHS ====================
set(${VECTOR_PROJECT_NAME}_INCLUDE_DIR 
    "${VECTOR_LIB_ROOT_DIR}/include/${VECTOR_PROJECT_NAMESPACE}"
)

find_library(${VECTOR_PROJECT_NAME}_LIBRARY
    NAMES 
        ${VECTOR_PROJECT_NAMESPACE} 
        ${VECTOR_PROJECT_NAME} 
        vector
    PATHS 
        "${VECTOR_LIB_ROOT_DIR}/lib"
        "${VECTOR_LIB_ROOT_DIR}/lib64"
    NO_DEFAULT_PATH
)

# ==================== TARGET ====================
if(NOT TARGET ${VECTOR_PROJECT_NAME}::${VECTOR_PROJECT_NAMESPACE})
    add_library(${VECTOR_PROJECT_NAME}::${VECTOR_PROJECT_NAMESPACE} UNKNOWN IMPORTED)
    
    set_target_properties(${VECTOR_PROJECT_NAME}::${VECTOR_PROJECT_NAMESPACE} PROPERTIES
        IMPORTED_LOCATION               "${${VECTOR_PROJECT_NAME}_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES   "${${VECTOR_PROJECT_NAME}_INCLUDE_DIR}"
    )
endif()

# ==================== VARIABLES find_package ====================
set(${VECTOR_PROJECT_NAME}_FOUND 		TRUE)
set(${VECTOR_PROJECT_NAME}_INCLUDE_DIRS "${${VECTOR_PROJECT_NAME}_INCLUDE_DIR}")
set(${VECTOR_PROJECT_NAME}_LIBRARIES    "${${VECTOR_PROJECT_NAME}_LIBRARY}")
set(${VECTOR_PROJECT_NAME}_LIBRARY_DIRS "${VECTOR_LIB_ROOT_DIR}/lib")

# ==================== CHECKINGS ====================
if(NOT "${${VECTOR_PROJECT_NAME}_LIBRARY}" STREQUAL "")
    set(${VECTOR_PROJECT_NAME}_FOUND FALSE)
    message(WARNING "${VECTOR_PROJECT_NAME} library not found")
endif()

if(NOT EXISTS "${${VECTOR_PROJECT_NAME}_INCLUDE_DIR}/vector.h")
    set(${VECTOR_PROJECT_NAME}_FOUND FALSE)
    message(WARNING 
        "${VECTOR_PROJECT_NAME} headers not found at ${${VECTOR_PROJECT_NAME}_INCLUDE_DIR}"
    )
endif()

# ==================== MESSAGE ====================
if(${VECTOR_PROJECT_NAME}_FOUND AND NOT ${VECTOR_PROJECT_NAME}_FIND_QUIETLY)
    message(STATUS 
        "Found ${VECTOR_PROJECT_NAME}: ${${VECTOR_PROJECT_NAME}_LIBRARY} "
        "(version ${${VECTOR_PROJECT_NAME}_VERSION})"
    )
endif()