/*****************************************************************************\
*                                                                           *
* File(s): Ex01_BasicUsage.ino                                              *
*                                                                           *
* Content: This simple example sketch shows how to initialize an HC-SR04    *
*          ultrasonic distance sensor and retrieve readings.                *
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
#include <HCSR04.h>

// define distance sensor instance
ArduForge::HCSR04 DSensor;
/**
 * The example sketch shows how to measure distance with an HC-SR04 ultrasonic distance sensor. 
 * Note that reliability of the measurement highly depends on the (sound) reflectance of the targeted surface. While walls, ceilings, and desktops usually work fine, clothing absorbs most of the sound impulse which distorts measurements or makes distant measurement impossible. Just give it a whirl and target your desk, your ceiling, a sheet of paper, and yourself.
 */

void setup() {
    // initialize serial connection
    Serial.begin(115200);

    // start up the HCSR04 sensor
    DSensor.begin(4, 5);
}

void loop() {
    // get distance measurement (in centimeter)
    const float Distance = DSensor.read();
    if(Distance < 0.0f){
        // if sensor reading is less than 0 the measurement failed
        Serial.print("No obstacle detected!");
        Serial.println();
    }else{
        // we got a valid measurement, so print it to serial
        Serial.print("Distance to target: "); 
        Serial.print(Distance);
        Serial.print(" cm");
        // output distance measurement in Imperial Units too
        Serial.print(" ("); Serial.print(ArduForge::HCSR04::centimeterToInch<float>(Distance)); Serial.print("\")");
        Serial.println();
    }

    delay(1000);
}//loop