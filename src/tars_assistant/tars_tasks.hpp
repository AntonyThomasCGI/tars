#ifndef TARS_TASKS_HPP_INCLUDED
#define TARS_TASKS_HPP_INCLUDED

#include "tars_intent.hpp"

int humourSetting = 100;

namespace tasks {
    void helloWorld(TARS_Intent *taskData);
    void getHumourSetting(TARS_Intent *taskData);
    void setHumourSetting(TARS_Intent *taskData);
};

#endif
