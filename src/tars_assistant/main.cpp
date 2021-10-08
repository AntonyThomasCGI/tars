
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

    tarsAudioData *userdata = new tarsAudioData;

    std::thread tAudio (tars_audio::run, userdata);


    while (true)
    {
        TARS_setState(LISTENING);

        while (TARS_getState() != AUDIO_EVENT)
        {
            std::this_thread::yield();
        }
        printf("t1d:Audio event!\n");

        // Get iterator of audio samples from beginning.
        // boost::circular_buffer<short>::iterator sampleIt = userdata->recordedSamples.begin();

        // std::string stringSamples;
        
        std::size_t binaryLen = (userdata->numSamples) * sizeof(short);

        // for ( boost::circular_buffer<short>::iterator it = userdata->recordedSamples.begin();
        //       it != userdata->recordedSamples.end(); 
        //       it++ )
        // {
        //     short sample = *it;
        //     std::cout << sample << std::endl;
        //     stringSamples.append((char*)&sample);
        // }

        // auto ostr = std::ostringstream();

        // ostr << std::setw(sizeof(short));

        // for ( boost::circular_buffer<short>::iterator it = userdata->recordedSamples.begin();
        //       it != userdata->recordedSamples.end(); 
        //       it++ )
        // {
        //     short sample = *it;
        //     ostr << (char*)&sample;
        // }

        short *recordedSamples;


        recordedSamples = (short *) malloc( userdata->numSamples * sizeof(short) ); /* From now on, recordedSamples is initialised. */
        if( recordedSamples == NULL )
        {
            fprintf(stderr, "Could not allocate record array.\n"); // TODO?
        }

        for( int i=0; i<userdata->numSamples; i++ ) recordedSamples[i] = userdata->recordedSamples[i];
        


        // // Char pointer to memory address of iterator gets the raw binary data of samples.
        const char *binarySamples = reinterpret_cast<const char *>(&recordedSamples[0]);  
        // printf("here2\n");
        // // Calc the binary length.
        // printf("here3\n");


        TARS_Intent intent = TARS_getIntent(binarySamples, binaryLen);

        userdata->timer = 0;
        userdata->voiceEvent = false;
    }

    TARS_setState(SHUT_DOWN);


    tAudio.join();
    delete userdata;

    return 0;
}
