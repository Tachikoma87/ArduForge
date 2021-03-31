/*****************************************************************************\
*                                                                           *
* File(s): Ex01_LedControl.ino                                              *
*                                                                           *
* Content: This example shows how to use the RGBLed class to create some    *
*          lighting effects with a common cathode RGB Led.                  *
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
#include <inttypes.h>
#include <RGBLed.h>
/**
 * This example shows how to use the RGBLed class to create some lighting effects with a common cathode RGB Led.
 * Use the wiring provided with the docs and the sketch will work without changes. 
 */

ArduForge::RGBLed ColorLed;
const uint8_t RedPin = 5; // Red channel pin
const uint8_t GreenPin = 6; // Green channel pin
const uint8_t BluePin = 9; // Blue channel pin

void setup() {
    Serial.begin(115200);
    // setup the pins
    ColorLed.begin(RedPin, GreenPin, BluePin);
    Serial.print("RGB Led initialized ...\n");
}//setup


void loop() {
    // fade from 0 to max for Red, green, blue, and white
    Serial.print("Starting RGB-Led Test ...\n");
    Serial.print("Red ... ");
    for(uint8_t i=0; i < 255; ++i){
        ColorLed.color(i, 0, 0);
        delay(20);
    }
    Serial.print("Green ...");
    for(uint8_t i=0; i < 255; ++i){
        ColorLed.color(0, i, 0);
        delay(20);
    }
    Serial.print("Blue ...");
    for(uint8_t i=0; i < 255; ++i){
        ColorLed.color(0, 0, i);
        delay(20);
    }
    Serial.print("White");
    for(uint8_t i=0; i < 255; ++i){
    ColorLed.color(i,i,i);
    delay(20); 
    }
    Serial.println();

    // let's make the led blink a bit in yellow and magenta
    Serial.print("Blinking...");
    Serial.print("Yellow ...");
    for(uint8_t i=0; i < 20; ++i){
        ColorLed.color(255, 200, 0);
        delay(125);
        ColorLed.color(0,0,0);
        delay(125);
    }
    Serial.print("Magenta ...");
    for(uint8_t i=0; i < 20; ++i){
        ColorLed.color(255, 0, 255);
        delay(125);
        ColorLed.color(0,0,0);
        delay(125);
    }
    Serial.println();
 
}//loop