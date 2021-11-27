#ifndef TARS_TASKS_HPP_DEFINITION
#define TARS_TASKS_HPP_DEFINITION

#include <string>
#include <map>

typedef void (*TaskFunction)(std::string *params);

struct TARS_Task
{
    TaskFunction function;
    std::string  *params;

};

namespace tasks {

    void nullTask(std::string *params);
    void helloWorld(std::string *params);
    void getHumourSetting(std::string *params);

};

#endif