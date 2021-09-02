cmake_minimum_required(VERSION 3.21)
project(tars VERSION 1.0.0)

include(FetchContent)

FetchContent_Declare(
  portaudio
  GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
  GIT_TAG        147dd722548358763a8b649b3e4b41dfffbcfbb6 # v19.7.0
)
  
FetchContent_MakeAvailable(portaudio)

# FetchContent_Declare(
#   curlpp
#   GIT_REPOSITORY https://github.com/jpbarrette/curlpp.git
#   GIT_TAG        592552a165cc569dac7674cb7fc9de3dc829906f # v0.8.1
# )

# # message(STATUS "ok:")

# # FetchContent_GetProperties(curlpp)
# # if(NOT curlpp_POPULATED)
# #   FetchContent_Populate(curlpp)
# #   add_subdirectory(${curlpp_SOURCE_DIR} ${curlpp_BINARY_DIR})
# # endif()

# # message(STATUS ${curlpp_SOURCE_DIR})
# # message(STATUS ${curlpp_BINARY_DIR})



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


# add_executable(tars src/main.cpp)
# target_link_libraries(${PROJECT_NAME} 
#                       PRIVATE portaudio
#                       ${CURL_LIBRARIES} )



add_subdirectory(src/tars_engine)
add_subdirectory(src/tars_assistant)
