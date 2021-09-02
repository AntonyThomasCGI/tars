// #include <stdio.h>
// #include <stdlib.h>
// #include "portaudio.h"
// #include <curl/curl.h>

// #define NUM_SECONDS (3)
// #define SAMPLE_RATE (44100)
// #define FRAMES_PER_BUFFER (512) // chunk size - this many frames per call to audio callback function.
// #define VOICE_THRESHOLD 0.3f // TODO base this on the first few seconds of mic input? assume quite? hmm

// #if 1
// #define PA_SAMPLE_TYPE  paFloat32
// typedef float SAMPLE;
// #define SAMPLE_SILENCE  (0.0f)
// #define PRINTF_S_FORMAT "%.8f"
// #elif 1
// #define PA_SAMPLE_TYPE  paInt16
// typedef short SAMPLE;
// #define SAMPLE_SILENCE  (0)
// #define PRINTF_S_FORMAT "%d"
// #elif 0
// #define PA_SAMPLE_TYPE  paInt8
// typedef char SAMPLE;
// #define SAMPLE_SILENCE  (0)
// #define PRINTF_S_FORMAT "%d"
// #else
// #define PA_SAMPLE_TYPE  paUInt8
// typedef unsigned char SAMPLE;
// #define SAMPLE_SILENCE  (128)
// #define PRINTF_S_FORMAT "%d"
// #endif

// typedef struct
// {
//     bool    idle; // Idle until sample above VOICE_THRESHOLD is hit.
//     int     frameIndex; // Current index.
//     int     maxFrameIndex; // Last index.
//     SAMPLE  *recordedSamples; // Save into this. // TODO? FIFO, looping array of 2 sec
// } 
// paUserData;

// // typedef struct
// // {
// //     int          frameIndex;  /* Index into sample array. */
// //     int          maxFrameIndex;
// //     SAMPLE      *recordedSamples;
// // }
// // paTestData;

// // static int recordCallback( const void *inputBuffer, void *outputBuffer,
// //                            unsigned long framesPerBuffer,
// //                            const PaStreamCallbackTimeInfo* timeInfo,
// //                            PaStreamCallbackFlags statusFlags,
// //                            void *userData )
// // {
// //     paTestData *data = (paTestData*)userData;
// //     const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
// //     SAMPLE *wptr = &data->recordedSamples[data->frameIndex];
// //     long framesToCalc;
// //     long i;
// //     int finished;
// //     unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

// //     (void) outputBuffer; /* Prevent unused variable warnings. */
// //     (void) timeInfo;
// //     (void) statusFlags;
// //     (void) userData;

// //     if( framesLeft < framesPerBuffer )
// //     {
// //         framesToCalc = framesLeft;
// //         finished = paComplete;
// //     }
// //     else
// //     {
// //         framesToCalc = framesPerBuffer;
// //         finished = paContinue;
// //     }

// //     if( inputBuffer == NULL )
// //     {
// //         for( i=0; i<framesToCalc; i++ )
// //         {
// //             *wptr++ = SAMPLE_SILENCE;  /* left */
// //         }
// //     }
// //     else
// //     {
// //         for( i=0; i<framesToCalc; i++ )
// //         {
// //             *wptr++ = *rptr++;  /* left */
// //         }
// //     }
// //     data->frameIndex += framesToCalc;
// //     return finished;
// // }

// static int recordAudioCallback( const void *inputBuffer, 
//                                 void *outputBuffer,
//                                 unsigned long frameCount, // (frames per buffer).
//                                 const PaStreamCallbackTimeInfo *timeInfo,
//                                 PaStreamCallbackFlags statusFlags,
//                                 void *userData ) 
// {
//     paUserData *data = (paUserData*)userData;
//     const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
//     SAMPLE *wptr = &data->recordedSamples[data->frameIndex];
//     int finished;

//     (void) outputBuffer;
//     (void) timeInfo;
//     (void) statusFlags;
//     (void) userData;

//     if (data->idle)
//     {
//         for( int i=0; i<frameCount; i++ )
//         {
//             float sample = *rptr++;
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

// static int playCallback( const void *inputBuffer, void *outputBuffer,
//                          unsigned long framesPerBuffer,
//                          const PaStreamCallbackTimeInfo* timeInfo,
//                          PaStreamCallbackFlags statusFlags,
//                          void *userData )
// {
//     paUserData *data = (paUserData*)userData;
//     SAMPLE *rptr = &data->recordedSamples[data->frameIndex];
//     SAMPLE *wptr = (SAMPLE*)outputBuffer;
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
//             SAMPLE sample = *rptr++;
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
//             SAMPLE sample = *rptr++;
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
//             SAMPLE sample = *rptr++;
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

// int main() {
//     // PaStreamParameters  inputParameters, 
//     //                     outputParameters;
//     // const PaDeviceInfo  *inputDevice;
//     // PaStream            *stream;
//     // PaError             err = paNoError;
//     // paUserData          data;
//     // int                 totalFrames;
//     // int                 numBytes;

//     // data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; // Record for a few seconds.
//     // data.frameIndex = 0;
//     // data.idle = true;
//     // numBytes = totalFrames * sizeof(SAMPLE);
//     // data.recordedSamples = (SAMPLE *) malloc( numBytes ); // From now on, recordedSamples is initialised.
//     // if (data.recordedSamples == NULL) {
//     //     printf("Could not allocate record array.\n");
//     //     goto done;
//     // }
//     // for( int i=0; i<totalFrames; i++ ) data.recordedSamples[i] = 0;

//     // err = Pa_Initialize();
//     // if( err != paNoError ) goto done;
    
//     // inputParameters.device = Pa_GetDefaultInputDevice(); // Gets the input device index.
//     // if (inputParameters.device == paNoDevice) {
//     //     fprintf(stderr,"Error: No default input device.\n");
//     //     goto done;
//     // }
//     // inputDevice = Pa_GetDeviceInfo(inputParameters.device); // Use input device index to get name.
//     // if (inputDevice == NULL) {
//     //     fprintf(stderr,"Error: Could not get input device info.\n");
//     //     goto done;
//     // }

//     // printf("Using input device: %s\n", inputDevice->name);

//     // inputParameters.channelCount = 1;
//     // inputParameters.sampleFormat = PA_SAMPLE_TYPE;
//     // inputParameters.suggestedLatency = inputDevice->defaultLowInputLatency;
//     // inputParameters.hostApiSpecificStreamInfo = NULL;

//     // err = Pa_OpenStream(
//     //     &stream,
//     //     &inputParameters,
//     //     NULL, // No output.
//     //     SAMPLE_RATE,
//     //     FRAMES_PER_BUFFER,
//     //     paClipOff, // Turn clipping off.
//     //     recordAudioCallback,
//     //     &data );
//     // if( err != paNoError ) goto done;

//     // err = Pa_StartStream( stream );
//     // if(err != paNoError ) goto done;

//     // printf("\nlistening...\n"); fflush(stdout);

//     // while(( err = Pa_IsStreamActive(stream) ) == 1 )
//     // {
//     //     Pa_Sleep(1000);
//     // }
//     // if( err < 0 ) goto done;

//     // err = Pa_CloseStream( stream );
//     // if( err != paNoError ) goto done;


//     PaStreamParameters  inputParameters,
//                         outputParameters;
//     PaStream*           stream;
//     PaError             err = paNoError;
//     paUserData          data;
//     int                 i;
//     int                 totalFrames;
//     int                 numSamples;
//     int                 numBytes;
//     SAMPLE              max, val;
//     double              average;

//     data.maxFrameIndex = totalFrames = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
//     data.frameIndex = 0;
//     data.idle = true;
//     numSamples = totalFrames;
//     numBytes = numSamples * sizeof(SAMPLE);
//     data.recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
//     if( data.recordedSamples == NULL )
//     {
//         printf("Could not allocate record array.\n");
//         goto done;
//     }
//     for( i=0; i<numSamples; i++ ) data.recordedSamples[i] = 0;

//     err = Pa_Initialize();
//     if( err != paNoError ) goto done;

//     inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
//     if (inputParameters.device == paNoDevice) {
//         fprintf(stderr,"Error: No default input device.\n");
//         goto done;
//     }
//     inputParameters.channelCount = 1;                    /* stereo input */
//     inputParameters.sampleFormat = PA_SAMPLE_TYPE;
//     inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
//     inputParameters.hostApiSpecificStreamInfo = NULL;

//     /* Record some audio. -------------------------------------------- */
//     err = Pa_OpenStream(
//               &stream,
//               &inputParameters,
//               NULL,                  /* &outputParameters, */
//               SAMPLE_RATE,
//               FRAMES_PER_BUFFER,
//               paClipOff,      /* we won't output out of range samples so don't bother clipping them */
//               recordAudioCallback,
//               &data );
//     if( err != paNoError ) goto done;

//     err = Pa_StartStream( stream );
//     if( err != paNoError ) goto done;
//     printf("\nlistening...\n"); fflush(stdout);

//     while( ( err = Pa_IsStreamActive( stream ) ) == 1 )
//     {
//         Pa_Sleep(1000);
//     }
//     if( err < 0 ) goto done;

//     err = Pa_CloseStream( stream );
//     if( err != paNoError ) goto done;


//     /************/
//     data.frameIndex = 0;

//     outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
//     if (outputParameters.device == paNoDevice) {
//         fprintf(stderr,"Error: No default output device.\n");
//         goto done;
//     }
//     outputParameters.channelCount = 1;                    
//     outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
//     outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
//     outputParameters.hostApiSpecificStreamInfo = NULL;

//     printf("=== Ok got audio, playing back. ===\n"); fflush(stdout);
//     err = Pa_OpenStream(
//               &stream,
//               NULL, /* no input */
//               &outputParameters,
//               SAMPLE_RATE,
//               FRAMES_PER_BUFFER,
//               paClipOff,      /* we won't output out of range samples so don't bother clipping them */
//               playCallback,
//               &data );
//     if( err != paNoError ) goto done;

//     if( stream )
//     {
//         err = Pa_StartStream( stream );
//         if( err != paNoError ) goto done;

//         printf("Waiting for playback to finish.\n"); fflush(stdout);

//         while( ( err = Pa_IsStreamActive( stream ) ) == 1 ) Pa_Sleep(100);
//         if( err < 0 ) goto done;

//         err = Pa_CloseStream( stream );
//         if( err != paNoError ) goto done;
//     }

// done:
//     Pa_Terminate();
//     printf("Done.\n");
//     if( err != paNoError )
//     {
//         fprintf( stderr, "An error occurred while using the portaudio stream\n" );
//         fprintf( stderr, "Error number: %d\n", err );
//         fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
//         err = 1;          /* Always return 0 or 1, but no other return codes. */
//     }
//     return err;
// }
