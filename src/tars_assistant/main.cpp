
// #include <stdio.h>
// #include <stdlib.h>

#include <thread>
// #include <chrono>
// #include <atomic>

#include "tars_audio.hpp"
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

    std::thread tAudio (tars_audio::listen);

    TARS_setState(LISTENING);

    while (TARS_getState() != AUDIO_EVENT)
    {
        std::this_thread::yield();
    }
    printf("t1:Audio event!\n");

    tAudio.join();

    return 0;
}
