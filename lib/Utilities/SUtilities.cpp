#include "SUtilities.h"
#include <Arduino.h>

extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;

namespace ArduForge{

    int16_t SUtilities::freeMemory() {
        int free_memory;

        if((int)__brkval == 0)
            free_memory = ((int)&free_memory) - ((int)&__bss_end);
        else
            free_memory = ((int)&free_memory) - ((int)__brkval);

        return free_memory;
    }//freeMemory

}//name space

