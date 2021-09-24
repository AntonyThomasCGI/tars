
add_library(tars-engine
tars_audio.cpp
tars_audio.hpp
tars_state.cpp
tars_state.hpp
tars_intent.cpp
tars_intent.hpp 
)

target_include_directories (tars-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(tars-engine 
                      PRIVATE portaudio
                    #   PRIVATE rapidjson
                      ${CURL_LIBRARIES} 
                      ${Boost_LIBRARIES})
