
#include <stdio.h>
// #include <stdlib.h>

#include <thread>
#include <sstream>
// #include <chrono>
// #include <atomic>

#include "tars_audio.hpp"
#include "tars_intent.hpp"
// #include "tars_state.hpp"
// #include "tars_intent.hpp"

// enum State {LISTENING, AUDIO_EVENT, SPEAKING};
// std::atomic<State> state;


// void test() {
//     printf("::Listening::\n");

//     std::this_thread::sleep_for(std::chrono::milliseconds(3000));

//     state.store(AUDIO_EVENT, std::memory_order_release);
// }



int main()
{

    paSharedData *userdata;
    
    std::thread tAudio (tars_audio::listen, userdata);


    TARS_setState(LISTENING);

    while (TARS_getState() != AUDIO_EVENT)
    {
        std::this_thread::yield();
    }
    printf("t1d:Audio event!\n");

    tAudio.join();

    short *audio = userdata->recordedSamples;

    char *binary;
    for (int i=0; i<userdata->maxFrameIndex; i++ )
    {
        ss << audio[i];
    }
    std::string s = ss.str();
    // printf("data: %s\n", s.c_str());

    TARS_Intent intent = TARS_getIntent(s);

    // for( int i=0; i < 600; i++)
    // {
    //     printf("sample: %d\n", userdata->recordedSamples[i]);
    // }


    return 0;
}
