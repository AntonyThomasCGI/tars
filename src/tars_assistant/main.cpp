
#include <stdio.h>
#include <iostream>

#include "tars_brain.hpp"
#include "tars_tasks.hpp"


int main()
{
    TARS_Brain tarsBrain;
    
    tarsBrain.AddTaskHandler("hello_world", tasks::helloWorld);
    tarsBrain.AddTaskHandler("humour_getter", tasks::getHumourSetting);
    tarsBrain.AddTaskHandler("humour_setter", tasks::setHumourSetting);

    tarsBrain.Listen(); // Blocking.

    return 0;
}
