
#include <stdio.h>
#include <sstream>

#include "tars_tasks.hpp"


int humourSetting = 100;


void tasks::helloWorld(TARS_Intent *taskData)
{
    printf("TARS says: Hello world!\n");
};

void tasks::getHumourSetting(TARS_Intent *taskData)
{
    printf("TARS says: My humour setting is %d%%\n", humourSetting);
};

void tasks::setHumourSetting(TARS_Intent *taskData)
{
    int newValue;
    for (TARS_Entity &entity : taskData->entities)
    {
        if (entity.type == "number")
        {
            std::stringstream ss(entity.value);
            ss >> newValue;
        }
    }
    if (newValue)
    {
        printf("TARS says: In humour setter, new value: %d!\n", newValue);
        humourSetting = newValue;
    } else {
        printf("TARS says: In humour setter, NO new value!\n");
    }
};
