

add_executable(${PROJECT_NAME} main.cpp)


add_library(
  tars-brain
  tars_brain.hpp
  tars_brain.cpp
  tars_tasks.hpp
  tars_tasks.cpp
)

target_link_libraries(tars-brain
                    PUBLIC tars-engine
)


target_link_libraries(${PROJECT_NAME} 
                      PUBLIC tars-brain 
                      PUBLIC tars-engine
                      PRIVATE portaudio
                      PRIVATE jsoncpp_lib
                      ${CURL_LIBRARIES}
                    )
