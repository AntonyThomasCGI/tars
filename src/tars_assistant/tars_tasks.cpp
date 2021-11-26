
#include <stdio.h>

#include "tars_tasks.hpp"


void tasks::nullTask(std::string *params)
{
    printf("TARS says: Null task called.\n");
};

void tasks::helloWorld(std::string *params)
{
    printf("TARS says: Hello world!\n");
};

void tasks::getHumourSetting(std::string *params)
{
    printf("TARS says: My humour setting is 100%%\n");
}

