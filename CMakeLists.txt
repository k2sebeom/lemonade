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
    ${PROJ_INCLUDE_DIR}/ExternalPlugin.h
    ${PROJ_INCLUDE_DIR}/ade_plugins/Reverb.h
    ${PROJ_INCLUDE_DIR}/ade_plugins/Chorus.h
    ${PROJ_INCLUDE_DIR}/ade_plugins/Compressor.h
    ${PROJ_INCLUDE_DIR}/ade_plugins/Limiter.h
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
    juce_graphics
    juce_gui_basics
    juce_gui_extra
)

add_definitions(
    # -DDEBUG=1
    # -D_DEBUG=1
    -DJUCE_MAC=1
    -DJUCE_PLUGINHOST_VST3=1
    -DJUCE_MODAL_LOOPS_PERMITTED=1
    -DJUCE_STANDALONE_APPLICATION=1
    -DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
    -DJUCE_DISABLE_JUCE_VERSION_PRINTING=1
    -DJUCE_WEB_BROWSER=0
    -DJUCE_USE_CURL=0
)

foreach(MODULE ${JUCE_MODULES})
    add_definitions(-DJUCE_MODULE_AVAILABLE_${MODULE}=1)
endforeach(MODULE)

# Build JUCE FrameWork
foreach(LIBRARY ${PROJ_LIBRARIES})
    add_subdirectory("${PROJ_LIBRARIES_DIR}/${LIBRARY}")
endforeach(LIBRARY)

###########
# Project #
###########
add_library(${TARGET_NAME} STATIC ${PROJ_SOURCES})

target_include_directories(${TARGET_NAME} PUBLIC
    ${PROJ_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/libs/JUCE/modules/
    ${CMAKE_SOURCE_DIR}/libs/JUCE/modules/juce_audio_processors/format_types/VST3_SDK/
)

# Link JUCE Modules
target_link_libraries(${TARGET_NAME} PUBLIC ${JUCE_MODULES})

############
# Examples #
############
set(PROJ_EXAMPLE_DIR examples)
set(PROJ_EXAMPLE_LIST
    reverb
    vst
)
foreach(EXAMPLE ${PROJ_EXAMPLE_LIST})
    add_subdirectory("${PROJ_EXAMPLE_DIR}/${EXAMPLE}")
endforeach(EXAMPLE)
