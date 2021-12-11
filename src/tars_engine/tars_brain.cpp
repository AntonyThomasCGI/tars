

#include <stdio.h>
#include <thread>
#include <bitset>
#include <sstream>
#include <fstream>

#include "tars_brain.hpp"
#include "tars_audio.hpp"


void nullTask(TARS_Intent *taskData)
{
    printf("TARS says: Null task called.\n");
};


TARS_Brain::TARS_Brain()
{
    this->AddTaskHandler("null_task", nullTask);
};

TARS_Brain::~TARS_Brain()
{

};

void TARS_Brain::HandleNewIntent(TARS_Intent intent)
{
    TaskHandler task = this->taskFromIntent(intent);

    printf("Calling task function...\n\n");
    task(&intent); // TODO pass whole intent? How do it so user can implement functionality..?
};

void TARS_Brain::AddTaskHandler(std::string task, TARS_Brain::TaskHandler f)
{
    if ( this->taskMapping.find(task) == this->taskMapping.end() ) {
        this->taskMapping[task] = f;
    }
    else {
        fprintf(stderr, "//Error: Task already exists!\n");
    }
};


TARS_Brain::TaskHandler TARS_Brain::taskFromIntent(TARS_Intent intent)
{
    /**/
    if (intent.confidence < 0.9)
    {
        printf("Confidence value is: %.02f Returning null task.\n", intent.confidence);
        TARS_Brain::TaskHandler task = this->taskMapping.at("null_task");
        return task;
    }
    if ( this->taskMapping.find(intent.intent) == this->taskMapping.end() ) {
        fprintf(stderr, "//Error: Task not found!\n");
        TARS_Brain::TaskHandler task = this->taskMapping.at("null_task");
        return task;
    } else {
        TARS_Brain::TaskHandler task = this->taskMapping.at(intent.intent);
        return task;
    }
};


void TARS_Brain::Listen()
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

        this->HandleNewIntent(intent);

        userdata->timer = 0;
        userdata->voiceEvent = false;
    }

    TARS_setState(SHUT_DOWN);


    tAudio.join();
    delete userdata;
};