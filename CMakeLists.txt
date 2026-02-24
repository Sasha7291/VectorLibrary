cmake_minimum_required(VERSION 3.21)

if(PROJECT_IS_TOP_LEVEL)
    project(VectorLibrary VERSION 1.0.1 LANGUAGES C)
	
    set(VECTOR_PROJECT_NAME 	${CMAKE_PROJECT_NAME})
	set(VECTOR_PROJECT_VERSION 	${CMAKE_PROJECT_VERSION})
else()
    set(VECTOR_PROJECT_NAME 	"VectorLibrary")
	set(VECTOR_PROJECT_VERSION 	"1.0.1")
endif()

# ==================== CONFIGURATION ====================
set(CMAKE_C_STANDARD 			90)
set(CMAKE_C_STANDARD_REQUIRED 	ON)
set(CMAKE_C_EXTENSIONS 			OFF)

include(GNUInstallDirs)

option(VECTOR_NO_DYNAMIC_ALLOC 		"Use static memory allocation only" 									OFF)
option(VECTOR_MAX_N_VECTORS 		"Maximum number of vectors (required VECTOR_NO_DYNAMIC_ALLOC=ON)" 		10)
option(VECTOR_STATIC_BUFFER_SIZE 	"Size of static buffer in bytes (required VECTOR_NO_DYNAMIC_ALLOC=ON)" 	1024)
option(VECTOR_8BIT_SIZE 			"Use uint8_t for size (for tiny systems)" 								OFF)
option(VECTOR_16BIT_SIZE			"Use uint16_t for size (for small systems)" 							OFF)
option(VECTOR_USE_PACKED_STRUCT 	"Use packed structs to save RAM" 										OFF)
option(VECTOR_CHECK_ON      		"Enable runtime checks"                                					OFF)
option(VECTOR_LITE          		"Build lite version (without function pointers)"      					OFF)
option(VECTOR_USE_INLINE 			"Force inline functions for speed" 										OFF)
option(VECTOR_OPTIMIZE_SIZE 		"Optimize for size (smaller growth factor)"           					OFF)
option(VECTOR_SMALL_MEMORY  		"Optimize for small memory (smaller initial capacity)" 					OFF)
option(VECTOR_USE_CUSTOM_ALLOCATOR 	"Use custom allocator functions (required VECTOR_NO_DYNAMIC_ALLOC=OFF)" OFF)
option(VECTOR_RUN_GENERATOR 		"Run vector_gen.bat to generate files"                					ON)

# ==================== NAMESPACE CONFIGURATION ====================
string(TOLOWER ${VECTOR_PROJECT_NAME} VECTOR_PROJECT_NAMESPACE)

# ==================== DIRECTORIES =======================
set(VECTOR_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(VECTOR_DOC_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/doc)
set(VECTOR_GEN_DIR     ${VECTOR_SOURCE_DIR}/gen)
set(VECTOR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/${VECTOR_PROJECT_NAMESPACE})
set(VECTOR_LOG_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/log)
set(VECTOR_PRIV_DIR    ${VECTOR_SOURCE_DIR}/priv)
set(VECTOR_SCRIPT_DIR  ${VECTOR_SOURCE_DIR}/script)

# ==================== GENERATE FILES ====================
if(VECTOR_RUN_GENERATOR)
	find_package(Python3 COMPONENTS Interpreter)
    
    if(Python3_FOUND)
        set(VECTOR_GEN_SCRIPT ${VECTOR_SCRIPT_DIR}/vector_gen.py)
        set(VECTOR_GEN_COMMAND ${Python3_EXECUTABLE} ${VECTOR_GEN_SCRIPT})
    else()
        if(WIN32)
            set(VECTOR_GEN_SCRIPT ${VECTOR_SCRIPT_DIR}/vector_gen.bat)
            set(VECTOR_GEN_COMMAND ${VECTOR_GEN_SCRIPT})
        else()
            find_program(BASH bash)
            if(BASH)
                set(VECTOR_GEN_SCRIPT ${VECTOR_SCRIPT_DIR}/vector_gen.sh)
                set(VECTOR_GEN_COMMAND ${BASH} ${VECTOR_GEN_SCRIPT})
            endif()
        endif()
    endif()
    
    if(EXISTS ${VECTOR_GEN_SCRIPT})
        message(STATUS "Running vector generator: ${VECTOR_GEN_SCRIPT}")
        
        file(MAKE_DIRECTORY ${VECTOR_LOG_DIR})
        file(MAKE_DIRECTORY ${VECTOR_DOC_DIR})
        file(MAKE_DIRECTORY ${VECTOR_GEN_DIR})
        
        set(VECTOR_GENERATED_FILES
            ${VECTOR_GEN_DIR}/vector_aliases.h
            ${VECTOR_GEN_DIR}/vector_decl.h
            ${VECTOR_GEN_DIR}/vector_impl.c
            ${VECTOR_DOC_DIR}/vector_types.txt
            ${VECTOR_LOG_DIR}/vector_gen.txt
        )
        
        add_custom_command(
            OUTPUT ${VECTOR_GENERATED_FILES}
            COMMAND ${VECTOR_GEN_COMMAND}
            WORKING_DIRECTORY ${VECTOR_SCRIPT_DIR}
            DEPENDS ${VECTOR_SOURCE_DIR}/vector.h
            COMMENT "Generating vector type files using vector_gen.bat"
            VERBATIM
        )
        
        add_custom_target(
            generate_vector_files ALL
            DEPENDS ${VECTOR_GENERATED_FILES}
            COMMENT "Generating vector files"
        )
        
        set_source_files_properties(
            ${VECTOR_GENERATED_FILES}
            PROPERTIES
            GENERATED TRUE
        )
        
        message(STATUS "  Generated files will be in: ${VECTOR_GEN_DIR}")
    else()
        message(WARNING "Generator script not found: ${VECTOR_GEN_SCRIPT}")
        message(WARNING "Please ensure vector_gen.bat exists in ${VECTOR_SCRIPT_DIR}")
    endif()
endif()

# ==================== LIBRARY SOURCES ====================
set(VECTOR_SOURCES
    ${VECTOR_SOURCE_DIR}/vector.c
    ${VECTOR_PRIV_DIR}/vector_template.c
    ${VECTOR_GEN_DIR}/vector_impl.c
)

set(VECTOR_HEADERS
    ${VECTOR_SOURCE_DIR}/vector.h
    ${VECTOR_SOURCE_DIR}/vector_error.h
    ${VECTOR_SOURCE_DIR}/vector_range.h
    ${VECTOR_SOURCE_DIR}/vector_size.h
)

set(VECTOR_GEN_HEADERS
    ${VECTOR_GEN_DIR}/vector_aliases.h
    ${VECTOR_GEN_DIR}/vector_decl.h
)

set(VECTOR_PRIV_HEADERS
    ${VECTOR_PRIV_DIR}/template.h
    ${VECTOR_PRIV_DIR}/vector_initialize_type.h
    ${VECTOR_PRIV_DIR}/vector_template.h
)

# ==================== CREATE LIBRARY ====================
add_library(${VECTOR_PROJECT_NAME} STATIC ${VECTOR_SOURCES})

set_target_properties(${VECTOR_PROJECT_NAME} PROPERTIES
    OUTPUT_NAME ${VECTOR_PROJECT_NAMESPACE}
    PREFIX "lib"
)

if(TARGET generate_vector_files)
    add_dependencies(${VECTOR_PROJECT_NAME} generate_vector_files)
endif()

# ==================== INCLUDE DIRECTORIES ====================
target_include_directories(${VECTOR_PROJECT_NAME}
    PUBLIC
        ${VECTOR_SOURCE_DIR}
        ${VECTOR_GEN_DIR}
        ${VECTOR_PRIV_DIR}
)

# ==================== COMPILE DEFINITIONS ====================
target_compile_definitions(${VECTOR_PROJECT_NAME}
    PRIVATE
        $<$<BOOL:${VECTOR_CHECK_ON}>:VECTOR_CHECK_ON>
        $<$<BOOL:${VECTOR_LITE}>:VECTOR_LITE>
        $<$<BOOL:${VECTOR_OPTIMIZE_SIZE}>:VECTOR_OPTIMIZE_SIZE>
        $<$<BOOL:${VECTOR_SMALL_MEMORY}>:VECTOR_SMALL_MEMORY>
)

# ==================== INSTALL ====================
set(VECTOR_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}/${VECTOR_PROJECT_NAMESPACE})

install(TARGETS ${VECTOR_PROJECT_NAME}
    EXPORT ${VECTOR_PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${VECTOR_HEADERS}
    DESTINATION ${VECTOR_INSTALL_INCLUDEDIR}
)

install(FILES ${VECTOR_GEN_HEADERS}
    DESTINATION ${VECTOR_INSTALL_INCLUDEDIR}/gen
)

install(FILES ${VECTOR_PRIV_HEADERS}
    DESTINATION ${VECTOR_INSTALL_INCLUDEDIR}/priv
)

# ==================== EXPORT ====================
install(EXPORT ${VECTOR_PROJECT_NAME}Targets
    FILE ${VECTOR_PROJECT_NAME}Config.cmake
    NAMESPACE ${VECTOR_PROJECT_NAMESPACE}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${VECTOR_PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${VECTOR_PROJECT_NAME}ConfigVersion.cmake
    VERSION ${VECTOR_PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${VECTOR_PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${VECTOR_PROJECT_NAME}
)

# ==================== UNINSTALL TARGET ====================
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY
    )
    
    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
        COMMENT "Uninstalling ${VECTOR_PROJECT_NAME}"
    )
endif()

# ==================== CUSTOM TARGETS ====================
add_custom_target(clean_gen
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${VECTOR_GEN_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${VECTOR_LOG_DIR}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${VECTOR_DOC_DIR}
    COMMENT "Cleaning generated files"
)

add_custom_target(run_generator
    COMMAND ${VECTOR_GEN_SCRIPT}
    WORKING_DIRECTORY ${VECTOR_SCRIPT_DIR}
    COMMENT "Running vector generator manually"
)

# ==================== PRINT CONFIGURATION ====================
message(STATUS "========================================")
message(STATUS "${VECTOR_PROJECT_NAME} configuration:")
message(STATUS "	Version: 				${VECTOR_PROJECT_VERSION}")
message(STATUS "  	Namespace: 				${VECTOR_PROJECT_NAMESPACE}")
message(STATUS "  	Source dir: 			${VECTOR_SOURCE_DIR}")
message(STATUS "  	Include dir (build): 	${VECTOR_INCLUDE_DIR}")
message(STATUS "  	Include dir (install): 	${VECTOR_INSTALL_INCLUDEDIR}")
message(STATUS "  	Script dir: 			${VECTOR_SCRIPT_DIR}")
message(STATUS "  	Doc dir: 				${VECTOR_DOC_DIR}")
message(STATUS "  	C Standard: 			${CMAKE_C_STANDARD}")
message(STATUS "  	Runtime checks: 		${VECTOR_CHECK_ON}")
message(STATUS "  	Lite version: 			${VECTOR_LITE}")
message(STATUS "  	Optimize for size: 		${VECTOR_OPTIMIZE_SIZE}")
message(STATUS "  	Small memory: 			${VECTOR_SMALL_MEMORY}")
message(STATUS "  	Run generator: 			${VECTOR_RUN_GENERATOR}")
message(STATUS "  	Build tests: 			${VECTOR_BUILD_TESTS}")
message(STATUS "========================================")