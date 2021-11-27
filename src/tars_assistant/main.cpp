
#include <stdio.h>
#include <iostream>
// #include <stdlib.h>

#include <thread>
#include <bitset>
#include <sstream>
#include <fstream>
// #include <iostream>
// #include <memory>
// #include <chrono>
// #include <atomic>

#include <tars_audio.hpp>
// #include <tars_intent.hpp>
#include <tars_brain.hpp>


int main()
{

    tarsAudioData *userdata = new tarsAudioData;
    TARS_Brain tarsBrain;

    std::thread tAudio (tars_audio::run, userdata);


    while (true)
    {
        TARS_setState(LISTENING);

        while (TARS_getState() != AUDIO_EVENT)
        {
            std::this_thread::yield();
        }
        printf("t1:Recieved audio event!\n");

        std::size_t binaryLen = (userdata->numSamples) * sizeof(short);

        short *recordedSamples;

        recordedSamples = (short *) malloc( userdata->numSamples * sizeof(short) ); /* From now on, recordedSamples is initialised. */
        if( recordedSamples == NULL )
        {
            fprintf(stderr, "Could not allocate record array.\n"); // TODO?
        }

        for( int i=0; i<userdata->numSamples; i++ ) recordedSamples[i] = userdata->recordedSamples[i];

        // Char pointer to memory address of iterator gets the raw binary data of samples.
        const char *binarySamples = reinterpret_cast<const char *>(&recordedSamples[0]);  

        TARS_Intent intent = TARS_getIntent(binarySamples, binaryLen);

        printf("t1:Speech->Text: %s\n", intent.text.c_str());
        printf("t1:Intent: %s\n", intent.intent.c_str());
        printf("t1:Confidence: %.2f\n", intent.confidence);

        tarsBrain.handleNewIntent(intent);

        userdata->timer = 0;
        userdata->voiceEvent = false;
    }

    TARS_setState(SHUT_DOWN);


    tAudio.join();
    delete userdata;

    return 0;
}
