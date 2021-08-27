cmake_minimum_required(VERSION 3.21)
project(tars VERSION 1.0.0)

include(FetchContent)

FetchContent_Declare(
  portaudio
  GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
  GIT_TAG        147dd722548358763a8b649b3e4b41dfffbcfbb6 # v19.7.0
)
FetchContent_MakeAvailable(portaudio)

add_executable(tars src/main.cpp)
target_link_libraries(tars PRIVATE portaudio)
