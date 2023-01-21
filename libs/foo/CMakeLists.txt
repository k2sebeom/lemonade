add_library(foo STATIC src/foo.cpp)

target_include_directories(foo PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/foo
)

target_compile_options(foo PRIVATE -Wall -Werror)
