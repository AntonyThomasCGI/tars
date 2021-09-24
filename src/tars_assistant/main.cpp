
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

    TARS_setState(LISTENING);

    while (TARS_getState() != AUDIO_EVENT)
    {
        std::this_thread::yield();
    }
    printf("t1d:Audio event!\n");

    // Get iterator of audio samples from beginning.
    // boost::circular_buffer<short>::iterator sampleIt = userdata->recordedSamples.begin();

    std::string stringSamples;
    
    size_t binaryLen = (userdata->numSamples) * sizeof(short);

    for ( boost::circular_buffer<short>::iterator it = userdata->recordedSamples.begin();
          it != userdata->recordedSamples.end(); 
          it++ )
    {
        short sample = *it;
        std::cout << sample << std::endl;
        stringSamples.append((char*)&sample);
    }
    
    
    // printf("here1\n");
    // // Char pointer to memory address of iterator gets the raw binary data of samples.
    // const char *binarySamples = reinterpret_cast<const char *>(&sampleIt);
    // printf("here2\n");
    // // Calc the binary length.
    // printf("here3\n");

    std::cout << stringSamples << std::endl;


    TARS_Intent intent = TARS_getIntent(stringSamples.c_str(), binaryLen);


    tAudio.join();
    delete userdata;

    return 0;
}
