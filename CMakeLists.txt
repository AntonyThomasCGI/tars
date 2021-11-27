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
  
# FetchContent_MakeAvailable(portaudio)


FetchContent_Declare(
  jsoncpp
  GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
  GIT_TAG        9059f5cad030ba11d37818847443a53918c327b1 # v1.9.4
)

FetchContent_MakeAvailable(portaudio jsoncpp)


message(STATUS "the var is: ${CURL_ROOT}")

message(STATUS"Looking for CURL")
find_package(CURL 7.64.1 REQUIRED)
if(CURL_FOUND)
  message(STATUS "Found CURL version: ${CURL_VERSION_STRING}")
  message(STATUS "Using CURL include dir(s): ${CURL_INCLUDE_DIRS}")
  message(STATUS "Using CURL lib(s): ${CURL_LIBRARIES}")
else()
  message(FATAL_ERROR "Could not find CURL")
endif()


message(STATUS "Looking for boost")
find_package(Boost 1.76.0 REQUIRED)
if(Boost_FOUND)
  message(STATUS "Found Boost version: ${Boost_VERSION_STRING}")
  message(STATUS "Using Boost include dir(s): ${Boost_INCLUDE_DIRS}")
  message(STATUS "Using Boost lib(s): ${Boost_LIBRARIES}")
else()
  message(FATAL_ERROR "Could not find Boost")
endif()


include_directories(${CURL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

add_subdirectory(src/tars_engine)
add_subdirectory(src/tars_assistant)
