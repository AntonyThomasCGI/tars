#ifndef TARS_BRAIN_HPP_INCLUDED
#define TARS_BRAIN_HPP_INCLUDED

// #include <deque>
// #include <string>
#include <map>

#include "tars_intent.hpp"

#include <cstddef>
#include <string>


// const std::map<std::string, TaskFunction> taskMapping = {
//     {"null_task",      tasks::nullTask},
//     {"hello_world",    tasks::helloWorld},
//     {"humour_setting", tasks::getHumourSetting},
// };

class TARS_Brain
{
    public:
        typedef void (*TaskHandler)(TARS_Intent *taskData);

    private:
        std::map<std::string, TaskHandler> taskMapping = {};

        TaskHandler taskFromIntent(TARS_Intent intent);
    public:
        TARS_Brain();
        virtual ~TARS_Brain();

        // std::deque<TARS_Task> recentTasks;

        void HandleNewIntent(TARS_Intent intent);

        void AddTaskHandler(std::string task, TaskHandler f);

        void RemoveTaskHandler(std::string task);

        void Listen();
};

#endif
