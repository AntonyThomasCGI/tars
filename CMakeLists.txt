cmake_minimum_required(VERSION 3.21)
project(tars VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
  portaudio
  GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
  GIT_TAG        147dd722548358763a8b649b3e4b41dfffbcfbb6 # v19.7.0
)
  
FetchContent_MakeAvailable(portaudio)


# FetchContent_Decalre(
#   rapidjson
#   GIT_REPOSITORY https://github.com/Tencent/rapidjson/.git
#   GIT_TAG        f54b0e47a08782a6131cc3d60f94d038fa6e0a51 # v1.1.0
# )

# FetchContent_MakeAvailable(rapidjson)



message(STATUS "Looking for CURL")
include(FindCURL)
find_package(CURL REQUIRED)

if(CURL_FOUND)
  message(STATUS "Found CURL version: ${CURL_VERSION_STRING}")
  message(STATUS "Using CURL include dir(s): ${CURL_INCLUDE_DIRS}")
  message(STATUS "Using CURL lib(s): ${CURL_LIBRARIES}")
else()
  message(FATAL_ERROR "Could not find CURL")
endif()


include_directories(${CURL_INCLUDE_DIRS})

add_subdirectory(src/tars_engine)
add_subdirectory(src/tars_assistant)
