/*****************************************************************************\
*                                                                           *
* File(s): Ex01_Switch.ino                                                  *
*                                                                           *
* Content: Example sketch for the binary sensor which lights an LED when    *
*          the switch is pressed.                                           *
*                                                                           *
*                                                                           *
* Author(s): Tom Uhlmann                                                    *
*                                                                           *
*                                                                           *
* The file(s) mentioned above are provided as is under the terms of the     *
* MIT License without any warranty or guaranty to work properly.            *
* For additional license, copyright and contact/support issues see the      *
* supplied documentation.                                                   *
*                                                                           *
\****************************************************************************/
#include <Arduino.h>
#include <BinarySensor.h>

/**
 * This examples shows a simple usecase for the binary sensor by lighting an LED
 * when the switch is pressed.
 * 
 */

ArduForge::BinarySensor Switch;
const int8_t SwitchPin = 2;
const int8_t LedPin = 3;

void setup(void){

    // start communication with the switch and set pullup resistor (default signal is HIGH)
    Switch.begin(SwitchPin, true);
    pinMode(LedPin, OUTPUT);
    digitalWrite(LedPin, LOW);
}//setup

void loop(void){
    // when the button is pressed the switch's signal is pulled to ground
    if(Switch.read() == LOW){
        digitalWrite(LedPin, HIGH);
    }      
    else
    {
        digitalWrite(LedPin, LOW);
    }
    
    delay(25);
}//loop

