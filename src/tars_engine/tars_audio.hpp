#ifndef TARS_AUDIO_HPP_INCLUDED
#define TARS_AUDIO_HPP_INCLUDED

#include "tars_state.hpp"
#include "portaudio.h"

namespace tars_audio {

    void listen();

}

typedef struct
{
    bool  idle; // Idle until sample above VOICE_THRESHOLD is hit.
    int   frameIndex; // Current index.
    int   maxFrameIndex; // Last index.
    short *recordedSamples; // Save into this. // TODO? FIFO, looping array of 2 sec
} paUserData;

class TARS_Audio
{
public:
    TARS_Audio(short *recordedSamples);
    virtual ~TARS_Audio();

private:
    PaStreamParameters  inputParameters,
                        outputParameters;
    PaStream*           stream;
    PaError             err;
    paUserData          userdata;
    int                 i;
    int                 totalFrames;
    int                 numSamples;
    int                 numBytes;
    short               max, val;
    double              average;

    void init(short *recordedSamples);
};

#endif
