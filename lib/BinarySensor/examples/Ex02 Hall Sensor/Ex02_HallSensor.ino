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
 * This examples shows a simple use-case for the binary sensor by lighting an LED
 * when a magnet approaches a hall sensor or reed switch.
 */

ArduForge::BinarySensor HallSensor;
const int8_t HallPin = 2;
const int8_t LedPin = 3;

void setup(void){

    // Start communication with the hall sensor. We don't want pullup active in this example.
    HallSensor.begin(HallPin, false);
    pinMode(LedPin, OUTPUT);
    digitalWrite(LedPin, LOW);
}//setup

void loop(void){
    // when the hall sensor detects a magnetic field with the correct polarity
    // the signal is pulled to ground and we read LOW.
    if(HallSensor.read() == LOW){
        digitalWrite(LedPin, HIGH);
    }      
    else
    {
        digitalWrite(LedPin, LOW);
    }
    
    delay(25);
}//loop

