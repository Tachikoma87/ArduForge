#include <Arduino.h>
//#include <Wire.h>

#include "Prototypes/RotaryEncoder.h"
#include "Prototypes/RotaryEncoder.cpp"

using namespace ArduForge;

RotaryEncoder Enc;

void setup(void){
   
    //Serial.begin(115200);
    Enc.begin(2, 3, 4);
}//setup

int32_t Counter = 0;

void loop(void){
    
    Enc.update();
   

}//loop

