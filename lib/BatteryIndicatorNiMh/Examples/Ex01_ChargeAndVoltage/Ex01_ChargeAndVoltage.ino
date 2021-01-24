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
#include <BatteryIndicatorNiMh.h>
/**
 * This example shows how to measure the voltage and remaining relative charge of a common NiMh rechargeable battery.
 * To test connect the anode (+) of the battery to an analog pin and the cathode (-) to ground. 
 * You can also test several batteries in series, but only up to 3 for an Arduino since 4 batteries can exceed the 5V limit and
 * may fry your board (or just trigger a fuse if you are lucky).
 * To get exact measurements you have to determine the voltage your board provides. Use a multimeter and measure voltage between 5V (or 3.3V) pin and ground.
 */

ArduForge::BatteryIndicatorNiMh Battery;

const uint8_t AnalogReadPin = 0; ///< Id of the analog pin the anode is connected to.
const uint8_t BatteryCount = 1; ///< Number of batteries that are in series.
const float ReferenceVoltage = 4.98; ///< Voltage that is actually provided by your board.

void setup() {
    // start serial communication
    Serial.begin(115200);
    // setup battery measurements
    Battery.begin(AnalogReadPin, BatteryCount, ReferenceVoltage);
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