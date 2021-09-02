#include "tars_audio.hpp"

#include <stdio.h>
#include <thread>
#include <atomic>
#include <chrono>


TARS_Audio::TARS_Audio(short *recordedSamples)
{
    init(recordedSamples);
}

TARS_Audio::~TARS_Audio()
{
    printf("TODO");
    //TODO
}

void TARS_Audio::init(short *recordedSamples)
{
    err = paNoError;

    userdata.idle = 1;
    userdata.frameIndex = 0;
    userdata.maxFrameIndex = 100;
    userdata.recordedSamples = recordedSamples;
}

void tars_audio::listen() 
{
    while (TARS_getState() != LISTENING)
    {
        std::this_thread::yield();
    }
    printf("t2:Listening\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    printf("t2:Found Audio event.\n");

    TARS_setState(AUDIO_EVENT);
}
