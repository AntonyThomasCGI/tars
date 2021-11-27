
add_library(tars-engine STATIC
        tars_audio.cpp
        tars_audio.hpp
        tars_brain.hpp
        tars_brain.cpp
        tars_intent.cpp
        tars_intent.hpp 
        tars_state.cpp
        tars_state.hpp
        tars_tasks.hpp
        tars_tasks.cpp
)

target_include_directories (tars-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(tars-engine
                      PRIVATE portaudio
                      PRIVATE jsoncpp_lib
                      ${CURL_LIBRARIES} 
                      ${Boost_LIBRARIES})
