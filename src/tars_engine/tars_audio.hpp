#ifndef TARS_AUDIO_HPP_INCLUDED
#define TARS_AUDIO_HPP_INCLUDED

#include "tars_state.hpp"
#include "portaudio.h"

typedef struct
{
    bool  idle; // Idle until sample above VOICE_THRESHOLD is hit.
    int   frameIndex; // Current index.
    int   maxFrameIndex; // Last index.
    short *recordedSamples; // Save into this. // TODO? FIFO, looping array of 2 sec
} paSharedData;

namespace tars_audio {

    void listen(paSharedData *userdata);


}
static int recordAudioCallback(const void *inputBuffer, 
                            void *outputBuffer,
                            unsigned long frameCount, // (frames per buffer).
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);

static int playCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData );


class TARS_Audio
{
public:
    TARS_Audio(paSharedData *userdata);
    virtual ~TARS_Audio();

    void startListening();
    void stopListening();
    void record();
    void shutdown(PaError err);

private:
    PaStream*           stream;
    paSharedData        userdata;

    void init(paSharedData *userdata);
};

#endif
