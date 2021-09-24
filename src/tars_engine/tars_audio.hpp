#ifndef TARS_AUDIO_HPP_INCLUDED
#define TARS_AUDIO_HPP_INCLUDED

#include "tars_state.hpp"
#include "portaudio.h"
#include <boost/circular_buffer.hpp>

typedef struct
{
    bool  voiceEvent; // Set to true when voice event happens.
    int   timer;      // Increments if voiceEvent is true.
    int   numSamples; // Timer counts to this value before completing.
    boost::circular_buffer<short> recordedSamples; // short *recordedSamples; // Save into this.
} tarsAudioData;

namespace tars_audio {

    void run(tarsAudioData *userdata);

}

static int recordAudioCallback(const void *inputBuffer, 
                            void *outputBuffer,
                            unsigned long frameCount, // (frames per buffer).
                            const PaStreamCallbackTimeInfo *timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);

static int playAudioCallback(const void *inputBuffer, 
                            void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);


class TARS_Audio
{
public:
    TARS_Audio(tarsAudioData *userdata);
    virtual ~TARS_Audio();

    void startListening();
    void stopListening();
    void startSpeaking();
    void record();
    void shutdown(PaError err);

private:
    PaStream      *stream;
    tarsAudioData *userdata;

    void init();

};

static int recordAudioCallback(const void *inputBuffer, 
                        void *outputBuffer,
                        unsigned long frameCount, // (frames per buffer).
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData);

static int playAudioCallback( const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData );

#endif
