

add_executable(tars main.cpp)


target_link_libraries(tars 
                      PUBLIC tars-engine
                      PRIVATE portaudio
                      ${CURL_LIBRARIES} 
                    )
