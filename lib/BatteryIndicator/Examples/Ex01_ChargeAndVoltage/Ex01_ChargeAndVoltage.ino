/*****************************************************************************\
*                                                                           *
* File(s): Ex01_ChargeAndVoltage.ino                                        *
*                                                                           *
* Content: This example shows how to measure the voltage and remaining      *
*          charge in percent.                                               *
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
#include <BatteryIndicator.h>
/**
 * This example shows how to measure the voltage and remaining relative charge of a common NiMh rechargeable battery using an Arduino.
 * To test connect the anode (+) of the battery to an analog pin and the cathode (-) to ground. 
 * You can also test several batteries in series, but only up to 3 for an Arduino since 4 batteries can exceed the 5V limit and
 * may fry your board (or just trigger a fuse if you are lucky). Check out the second example to see how you can measure high voltage batteries.
 */

ArduForge::BatteryIndicator Battery;

const uint8_t AnalogReadPin = 0; ///< Id of the analog pin the anode is connected to.
const uint8_t BatteryCount = 1; ///< Number of batteries that are in series.

void setup() {
    // start serial communication
    Serial.begin(115200);
    // setup battery measurements
    Battery.begin(AnalogReadPin, BatteryCount, 0, 0, ArduForge::BatteryIndicator::TYPE_NIMH);
}//setup


void loop() {
    // print status of the battery
    // values will be erratic if no battery is connected
    Serial.print("Battery at ");
    Serial.print(Battery.charge());
    Serial.print("% (");
    Serial.print(Battery.read());
    Serial.print("V)\n");

    delay(2000);
}//loop