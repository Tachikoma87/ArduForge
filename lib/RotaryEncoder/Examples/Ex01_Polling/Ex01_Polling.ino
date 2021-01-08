/*****************************************************************************\
*                                                                           *
* File(s): Ex01_Polling.ino                                                 *
*                                                                           *
* Content: Example sketch that shows how to use the RotaryEncoder class     *
*          with a simple polling approach.                                  *
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
 * This example sketch shows how to use the rotary encoder class using a simple polling approach.
 */

unsigned long Timeout = 0; // save milliseconds to realize a timeout

void setup() {
    // put your setup codes here, to run once
    Serial.begin(115200);

    Encoder.begin(2, 3, 4);
    Serial.print("Rotary encoder initialized and ready.\n"); 
}

void loop() {
    // update the state of the encoder (polling)
    Encoder.update();

    // if the encoder's button is pressed we will reset the position
    if(Encoder.switchState()){
        Encoder.resetPosition();
    }

    // every other second we will print the current state to the serial monitor
    if(millis() - Timeout > 1000){
        Serial.print("Encoder position: "); Serial.print(Encoder.position()); 
        Serial.print(" | Turning direction: "); 
        if(Encoder.direction() == ArduForge::RotaryEncoder::DIR_CW){
            Serial.print(" clock wise");
        }else{
            Serial.print(" counter clock wise");
        }
        Serial.println();

        Timeout = millis();
    }

}//loop