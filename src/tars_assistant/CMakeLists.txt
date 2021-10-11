

add_executable(${PROJECT_NAME} main.cpp)




target_link_libraries(${PROJECT_NAME} 
                      PUBLIC tars-engine
                      PRIVATE portaudio
                      PRIVATE jsoncpp_lib
                      ${CURL_LIBRARIES} 
                    )
