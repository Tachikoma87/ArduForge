/*****************************************************************************\
*                                                                           *
* File(s): Ex02_Interrupt.ino                                               *
*                                                                           *
* Content: Example sketch that shows how to use the RotaryEncoder class     *
*          using an attached interrupt for the clock signal.                *
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
#include <RotaryEncoder.h>

ArduForge::RotaryEncoder Encoder;
/**
 * This example sketch shows how to use the rotary encoder class using an interrupt attached to the clock signal.
 */

bool Print = false;

// This is the interrupt function that will be called every time the clock signal changes
void updateEncoder(void){
    Encoder.update(); // clock signal changed so update the encoder
    Print = true; // print the changed state in next main loop run
}

void setup() {
    // put your setup codes here, to run once
    Serial.begin(115200);

    Encoder.begin(2, 3, 4);
    // We attach an interrupt to the clock signal (digital pin 2) and update on every flank change (rising and falling).
    // Make sure you attach the clock signal to a pin which support an interrupt. If you don't use an Arduino, a different pin might be required.
    attachInterrupt(digitalPinToInterrupt(2), updateEncoder, CHANGE);
    Serial.print("Rotary encoder initialized and ready.\n"); 
}

void loop() {
 
    // If the encoder's button is pressed we will reset the position.
    // We check the switch only once every second. This means there is a chance to miss a switch press. 
    // If the button press needs to be reliable you will have to attach the switch pin to an interrupt too.
    if(Encoder.switchState()){
        Encoder.resetPosition();
        Print = true;
    }

    if(Print){
        Serial.print("Encoder position: "); Serial.print(Encoder.position()); 
        Serial.print(" | Turning direction: "); 
        if(Encoder.direction() == ArduForge::RotaryEncoder::DIR_CW){
            Serial.print(" clock wise");
        }else{
            Serial.print(" counter clock wise");
        }
        Serial.println();

        Print = false;
    }

    // In this example we can use delay since the interrupt will ensure that we don't miss a signal from the encoder.
    delay(1000);
}//loop