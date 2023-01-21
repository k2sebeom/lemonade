cmake_minimum_required(VERSION 3.11)

project(
  CppTemplate
  VERSION 0.1
  DESCRIPTION "Example Project for Cpp Compile Structure"
  LANGUAGES CXX
)

set(TARGET_NAME program)
set(PROJ_SOURCE_DIR src)
set(PROJ_SOURCES
    ${PROJ_SOURCE_DIR}/main.cpp
    ${PROJ_SOURCE_DIR}/bar.cpp
)
set(PROJ_LIBRARIES_DIR
    libs
)

set(PROJ_LIBRARIES
    foo
)

###########
# Project #
###########
add_executable(${TARGET_NAME} ${PROJ_SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

foreach(LIBRARY ${PROJ_LIBRARIES})
    add_subdirectory("${PROJ_LIBRARIES_DIR}/${LIBRARY}")
endforeach(LIBRARY)
target_link_libraries(${TARGET_NAME} ${PROJ_LIBRARIES})
