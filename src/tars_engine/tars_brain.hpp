#ifndef TARS_BRAIN_HPP_INCLUDED
#define TARS_BRAIN_HPP_INCLUDED

// #include <deque>
// #include <string>

#include "tars_intent.hpp"
#include "tars_tasks.hpp"

#include <cstddef>
#include <string>


const std::map<std::string, TaskFunction> taskMapping = {
    {"null_task",      tasks::nullTask},
    {"hello_world",    tasks::helloWorld},
    {"humour_setting", tasks::getHumourSetting},
};

class TARS_Brain
{
    public:
        TARS_Brain();
        virtual ~TARS_Brain();

        // std::deque<TARS_Task> recentTasks;
        
        void handleNewIntent(TARS_Intent intent);
    
    private:
        TARS_Task taskFromIntent(TARS_Intent intent);


};

#endif
