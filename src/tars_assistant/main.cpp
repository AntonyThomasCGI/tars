
#include <stdio.h>
#include <iostream>
// #include <stdlib.h>

#include <thread>
#include <bitset>
#include <string>
#include <sstream>
#include <fstream>
// #include <iostream>
// #include <memory>
// #include <chrono>
// #include <atomic>

#include "tars_audio.hpp"
#include "tars_intent.hpp"





int main()
{

    paSharedData *userdata = new paSharedData;
    
    std::thread tAudio (tars_audio::listen, userdata);
    // std::thread tAudio (test, userdata);

    TARS_setState(LISTENING);

    while (TARS_getState() != AUDIO_EVENT)
    {
        std::this_thread::yield();
    }
    printf("t1d:Audio event!\n");

    tAudio.join();

    userdata->frameIndex = 0;

    printf("max: %d\n", userdata->maxFrameIndex);

    const char *binarySamples = reinterpret_cast<const char *>(&userdata->recordedSamples[0]);
    size_t binaryLen = (userdata->maxFrameIndex + 1) * sizeof(short);


    TARS_Intent intent = TARS_getIntent(binarySamples, binaryLen);

    delete userdata;




    return 0;
}
