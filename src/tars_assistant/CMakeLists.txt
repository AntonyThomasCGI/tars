
add_executable(${PROJECT_NAME} main.cpp)

add_library(
  tars-tasks
  tars_tasks.hpp
  tars_tasks.cpp
)


target_link_libraries(tars-tasks
                    PRIVATE tars-engine
)
# target_include_directories(tars-brain PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")




target_link_libraries(${PROJECT_NAME} 
                  PRIVATE tars-engine
                  PRIVATE tars-tasks
                  # PRIVATE tars-brain
                  # PRIVATE portaudio
                  # PRIVATE jsoncpp_lib
                  # ${CURL_LIBRARIES}
)
