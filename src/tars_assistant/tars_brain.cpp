

#include <stdio.h>

#include "tars_brain.hpp"


TARS_Brain::TARS_Brain()
{

};

TARS_Brain::~TARS_Brain()
{

};

void TARS_Brain::handleNewIntent(TARS_Intent intent)
{
    TARS_Task task = this->taskFromIntent(intent);

    printf("Calling task function...\n\n");
    task.function(task.params);
};


TARS_Task TARS_Brain::taskFromIntent(TARS_Intent intent)
{
    /**/
    if (intent.confidence < 0.9)
    {
        printf("Confidence value is: %.02f Returning null task.\n", intent.confidence);
        TARS_Task task = {tasks::nullTask, NULL};
        return task;
    }
    if ( taskMapping.find(intent.intent) == taskMapping.end() ) {
        fprintf(stderr, "//Error: Task not found!\n");
        TARS_Task task = {tasks::nullTask, NULL};
        return task;
    } else {
        TARS_Task task = {taskMapping.at(intent.intent), NULL};
        return task;
    }
};
