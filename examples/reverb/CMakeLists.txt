###########
# Project #
###########

include(FetchContent)

FetchContent_Declare(
  SndFile
  GIT_REPOSITORY "https://github.com/libsndfile/libsndfile.git"
  GIT_TAG "1.1.0"
)

FetchContent_MakeAvailable(SndFile)

add_executable(reverb main.cpp)

target_link_libraries(reverb lemonade sndfile)
