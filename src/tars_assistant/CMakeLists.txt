

# add_library(
#   tars-brain STATIC
#   tars_brain.hpp
#   tars_brain.cpp
#   tars_tasks.hpp
#   tars_tasks.cpp
# )


# target_link_libraries(tars-brain
#                     PRIVATE tars-engine
# )
# target_include_directories(tars-brain PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")


add_executable(${PROJECT_NAME} main.cpp)


target_link_libraries(${PROJECT_NAME} 
                      PRIVATE tars-engine
                      # PRIVATE tars-brain
                      PRIVATE portaudio
                      PRIVATE jsoncpp_lib
                      ${CURL_LIBRARIES}
                    )
