cmake_minimum_required(VERSION 3.11)

project(
  LemonAde
  VERSION 0.1.0
  DESCRIPTION "Simple Audio Processing in C++ with JUCE"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME lemonade)
set(PROJ_SOURCE_DIR src)
set(PROJ_INCLUDE_DIR include)

set(PROJ_SOURCES
    ${PROJ_SOURCE_DIR}/lemonade.cc
    ${PROJ_INCLUDE_DIR}/lemonade.h
    ${PROJ_INCLUDE_DIR}/ade_plugins/Reverb.h
)

set(PROJ_LIBRARIES_DIR
    libs
)

set(PROJ_LIBRARIES
    JUCE
)

set(JUCE_MODULES 
    juce_core
    juce_dsp
    juce_audio_basics
    juce_audio_formats
    juce_audio_processors
    juce_data_structures
    juce_events
)

add_definitions(
    -DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
    -DDEBUG=1
    -D_DEBUGE=1
    -DMACOS=1
)

foreach(MODULE ${JUCE_MODULES})
    add_definitions(-DJUCE_MODULE_AVAILABLE_${MODULE}=1)
endforeach(MODULE)

###########
# Project #
###########
add_library(${TARGET_NAME} ${PROJ_SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC ${PROJ_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/libs/JUCE/modules)

foreach(LIBRARY ${PROJ_LIBRARIES})
    add_subdirectory("${PROJ_LIBRARIES_DIR}/${LIBRARY}")
endforeach(LIBRARY)

# Link JUCE Modules
target_link_libraries(${TARGET_NAME} ${JUCE_MODULES})


############
# Examples #
############
set(PROJ_EXAMPLE_DIR examples)
set(PROJ_EXAMPLE_LIST
    reverb
)
foreach(EXAMPLE ${PROJ_EXAMPLE_LIST})
    add_subdirectory("${PROJ_EXAMPLE_DIR}/${EXAMPLE}")
endforeach(EXAMPLE)
