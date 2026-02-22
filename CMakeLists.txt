cmake_minimum_required(VERSION 3.16)

project(vector LANGUAGES C)

add_library(${PROJECT_NAME} main.c
    script/vector_gen.bat
    src/vector.c src/vector.h
    src/vector_error.h
    src/vector_range.h
    src/vector_size.h
    src/vec/template.h
    src/vec/vector_checker.h
    src/vec/vector_initialize_type.h
    src/vec/vector_template.c src/vec/vector_template.h
    src/gen/vector_aliases.h
    src/gen/vector_decl.h
    src/gen/vector_impl.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    src/gen
    src/vec
)

include(GNUInstallDirs)
install(TARGETS vector
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
