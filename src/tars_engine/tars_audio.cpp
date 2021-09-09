#include "tars_audio.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <atomic>
#include <chrono>

#define NUM_SECONDS (3)
#define SAMPLE_RATE (16000)
#define FRAMES_PER_BUFFER (512) // chunk size - this many frames per call to audio callback function.
#define VOICE_THRESHOLD (2000) // TODO base this on the first few seconds of mic input? assume quite? hmm

#define PA_SAMPLE_TYPE  paInt16
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"


TARS_Audio::TARS_Audio(paSharedData *userdata)
{
    init(userdata);
}

TARS_Audio::~TARS_Audio()
{
    //TODO gets called twice atm?
    Pa_Terminate();
    // TODO?
}

void TARS_Audio::startListening()
{
    PaError err;
    err = Pa_StartStream( stream );
    if( err != paNoError )
    {
        this->shutdown(err);
    }
    printf("\nlistening...\n"); fflush(stdout);

    while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
    {
        Pa_Sleep(1000);
    }
    if( err < 0 )
    {
        this->shutdown(err);
    }
    err = Pa_CloseStream( stream );
    if( err != paNoError )
    {
        this->shutdown(err);
    }
}

void TARS_Audio::stopListening()
{
    printf("@todo");
}

void TARS_Audio::startSpeaking(paSharedData *userdata)
{
    PaError err;

    PaStreamParameters outputParameters;

    userdata->frameIndex = 0;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        this->shutdown(1); // TODO
    }
    outputParameters.channelCount = 1;                    
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    printf("=== Ok got audio, playing back. ===\n"); fflush(stdout);
    err = Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              playAudioCallback,
              userdata );
    if( err != paNoError )
    {
        this->shutdown(err);
    }

    if( stream )
    {
        err = Pa_StartStream( stream );
        if( err != paNoError )
        {
            this->shutdown(err);
        }

        printf("Waiting for playback to finish.\n"); fflush(stdout);

        while( ( err = Pa_IsStreamActive( stream ) ) == 1 ) Pa_Sleep(100);
        if( err < 0 )
        {
            this->shutdown(err);
        }

        err = Pa_CloseStream( stream );
        if( err != paNoError )
        {
            this->shutdown(err);
        }
    }
}

void TARS_Audio::record()
{
    printf("@todo");
}

void TARS_Audio::shutdown(PaError err)
{
    Pa_Terminate();
    printf("Shutting down TARS_Audio.\n");
    if(err != paNoError)
    {
        fprintf( stderr, "An error occurred while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    }
}

void TARS_Audio::init(paSharedData *userdata)
{
    // TODO probably don't have to have these vars defined in private class scope..
    // just need to open stream for input/output
    // need userdata, stream .. nothing else
    PaStreamParameters  inputParameters,
                        outputParameters;
    PaError             err;
    int                 i;
    int                 totalFrames;
    // int                 numSamples;
    int                 numBytes;
    short               max, val;
    double              average;

    err = paNoError;
    printf("here\n");

    userdata->idle = 1;
    printf("here2\n");
    userdata->frameIndex = 0;
    userdata->maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
    // userdata.recordedSamples = recordedSamples;
    numBytes = totalFrames * sizeof(short);

    userdata->recordedSamples = (short *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
    if( userdata->recordedSamples == NULL )
    {
        fprintf(stderr, "Could not allocate record array.\n"); // TODO?
    }
    for( i=0; i<totalFrames; i++ ) userdata->recordedSamples[i] = 0;

    err = Pa_Initialize();
    if( err != paNoError )
    {
        this->shutdown(err);
        return;
    }

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        this->shutdown(err);
        return;
    }
    inputParameters.channelCount = 1;                    /* mono input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              NULL,                  /* &outputParameters, */
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              recordAudioCallback,
              userdata );
    if( err != paNoError )
    {
        this->shutdown(err);
    };
}

void tars_audio::listen(paSharedData *userdata) 
{
    TARS_Audio tars_audio(userdata);

    while (TARS_getState() != LISTENING)
    {
        std::this_thread::yield();
    }
    printf("t2:Listening\n");
    tars_audio.startListening();

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));


    printf("t2:Playing back audio...\n");

    tars_audio.startSpeaking(userdata);

    printf("t2:setting state.\n");

    TARS_setState(AUDIO_EVENT);

    tars_audio.shutdown(paNoError);
}


// static int recordAudioCallback( const void *inputBuffer, 
//                                 void *outputBuffer,
//                                 unsigned long frameCount, // (frames per buffer).
//                                 const PaStreamCallbackTimeInfo *timeInfo,
//                                 PaStreamCallbackFlags statusFlags,
//                                 void *userData ) 
// {
//     paSharedData *data = (paSharedData*)userData;
//     const short *rptr = (const short*)inputBuffer;
//     short *wptr = &data->recordedSamples[data->frameIndex];
//     int finished;

//     (void) outputBuffer;
//     (void) timeInfo;
//     (void) statusFlags;
//     (void) userData;

//     if (data->idle)
//     {
//         for( int i=0; i<frameCount; i++ )
//         {
//             short sample = *rptr++;
//             // printf("%d\n", sample);
//             if( sample > VOICE_THRESHOLD )
//             {
//                 // idle = False
//                 data->idle = false;
                
//                 printf("Start Recording.\n"); fflush(stdout);

//                 break;
//             }
//         }
//         finished = paContinue; // Not finished if still idle.
//     }
//     else
//     {
//         // calc framesToSample
//         unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;
//         long framesToCalc;
//         int i;
//         if( framesLeft < frameCount )
//         {
//             framesToCalc = framesLeft;
//             finished = paComplete;
//         }
//         else
//         {
//             framesToCalc = frameCount;
//             finished = paContinue;
//         }
//         // sample
//         if( inputBuffer == NULL)
//         {
//             for( i=0; i<framesToCalc; i++ )
//             {
//                 *wptr++ = SAMPLE_SILENCE;
//             }
//         }
//         else
//         {
//             for( i=0; i<framesToCalc; i++ )
//             {
//                 *wptr++ = *rptr++;
//             }
//         }
//         data->frameIndex += framesToCalc;
//     }

//     return finished;
// }
static int recordAudioCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    paSharedData *data = (paSharedData*)userData;
    const short *rptr = (const short*)inputBuffer;
    short *wptr = &data->recordedSamples[data->frameIndex];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) outputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if( framesLeft < framesPerBuffer )
    {
        framesToCalc = framesLeft;
        finished = paComplete;
    }
    else
    {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }

    if( inputBuffer == NULL )
    {
        for( i=0; i<framesToCalc; i++ )
        {
            *wptr++ = SAMPLE_SILENCE;  /* left */
            // if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
        }
    }
    else
    {
        for( i=0; i<framesToCalc; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            // if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
    }
    data->frameIndex += framesToCalc;
    return finished;
}

// static int playCallback( const void *inputBuffer, void *outputBuffer,
//                          unsigned long framesPerBuffer,
//                          const PaStreamCallbackTimeInfo* timeInfo,
//                          PaStreamCallbackFlags statusFlags,
//                          void *userData )
// {
//     paSharedData *data = (paSharedData*)userData;
//     short *rptr = &data->recordedSamples[data->frameIndex];
//     short *wptr = (short*)outputBuffer;
//     unsigned int i;
//     int finished;
//     unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

//     (void) inputBuffer; /* Prevent unused variable warnings. */
//     (void) timeInfo;
//     (void) statusFlags;
//     (void) userData;

//     if( framesLeft < framesPerBuffer )
//     {
//         /* final buffer... */
//         for( i=0; i<framesLeft; i++ )
//         {
//             short sample = *rptr++;
//             int sampleFactor = static_cast<int>(sample*70);
//             if (sampleFactor > 0)
//             {
//                 printf("------------------------------|");
//                 for( int s=0; s<30; s++ )
//                 {
//                     if( s < sampleFactor )
//                     {
//                         printf("#");
//                     }
//                     else
//                     {
//                         printf("-");
//                     }
//                 }
//             }
//             else
//             {
//                 for( int s=-30; s<0; s++ )
//                 {
//                     if( s > sampleFactor )
//                     {
//                         printf("#");
//                     }
//                     else
//                     {
//                         printf("-");
//                     }
//                 }
//                 printf("|------------------------------");
//             }
//             printf("\n");
//             fflush(stdout);
//             *wptr++ = sample;
//         }
//         for( ; i<framesPerBuffer; i++ )
//         {
//             short sample = *rptr++;
//             int sampleFactor = static_cast<int>(sample*70);
//             if (sampleFactor > 0)
//             {
//                 printf("------------------------------|");
//                 for( int s=0; s<30; s++ )
//                 {
//                     if( s < sampleFactor )
//                     {
//                         printf("#");
//                     }
//                     else
//                     {
//                         printf("-");
//                     }
//                 }
//             }
//             else
//             {
//                 for( int s=-30; s<0; s++ )
//                 {
//                     if( s > sampleFactor )
//                     {
//                         printf("#");
//                     }
//                     else
//                     {
//                         printf("-");
//                     }
//                 }
//                 printf("|------------------------------");
//             }
//             printf("\n");
//             fflush(stdout);
//             *wptr++ = sample;
//         }
//         data->frameIndex += framesLeft;
//         finished = paComplete;
//     }
//     else
//     {
//         for( i=0; i<framesPerBuffer; i++ )
//         {
//             short sample = *rptr++;
//             int sampleFactor = static_cast<int>(sample*70);
//             if (sampleFactor > 0)
//             {
//                 printf("------------------------------|");
//                 for( int s=0; s<30; s++ )
//                 {
//                     if( s < sampleFactor )
//                     {
//                         printf("#");
//                     }
//                     else
//                     {
//                         printf("-");
//                     }
//                 }
//             }
//             else
//             {
//                 for( int s=-30; s<0; s++ )
//                 {
//                     if( s > sampleFactor )
//                     {
//                         printf("#");
//                     }
//                     else
//                     {
//                         printf("-");
//                     }
//                 }
//                 printf("|------------------------------");
//             }
//             printf("\n");
//             fflush(stdout);
//             *wptr++ = sample;
//         }
//         data->frameIndex += framesPerBuffer;
//         finished = paContinue;
//     }
//     return finished;
// }

static int playAudioCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    paSharedData *data = (paSharedData*)userData;
    short *rptr = &data->recordedSamples[data->frameIndex];
    short *wptr = (short*)outputBuffer;
    unsigned int i;
    int finished;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    (void) inputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    if( framesLeft < framesPerBuffer )
    {
        /* final buffer... */
        for( i=0; i<framesLeft; i++ )
        {
            *wptr++ = *rptr++;  /* left */
            // if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
        for( ; i<framesPerBuffer; i++ )
        {
            *wptr++ = 0;  /* left */
            // if( NUM_CHANNELS == 2 ) *wptr++ = 0;  /* right */
        }
        data->frameIndex += framesLeft;
        finished = paComplete;
    }
    else
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            short sample = *rptr++;
            *wptr++ = sample;  /* left */
            // if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
        }
        data->frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}