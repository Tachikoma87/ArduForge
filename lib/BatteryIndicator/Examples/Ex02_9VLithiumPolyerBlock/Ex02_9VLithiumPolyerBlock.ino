/*****************************************************************************\
*                                                                           *
* File(s): Ex02_9VLithiumPolymerBlock.ino                                   *
*                                                                           *
* Content: This example shows how to measure voltage and remaining          *
*          charge in percent of lithium polymer 9V block.                   *
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
 * This example shows how to measure the voltage and remaining relative charge of a dual Lithium Polymer battery used in rechargeable 9V blocks.
 * Since the 6.4 to 8.2 volts of the battery exceeds board limits by far, we have to use a voltage divider circuit - 47k and 22k will work for 5V as well as 3.3V boards. 
 * To get exact measurements you have to determine the voltage your board actually operates with. Usually an Arduino runs at 5V and an ESP32 at 3.3V but some boards may differ. An Arduino Nano (China rebuild) may run at 4.75V. Use a multimeter and measure voltage between 5V (or 3.3V) pin and ground.
 */

// Global battery instance
ArduForge::BatteryIndicator Battery;

#ifdef ESP32
const uint8_t AnalogReadPin = 33; ///< Analog pin to read the battery voltage.
const float RefVoltage = 3.3f; ///< ESP32 boards operate on 3.3V
const uint8_t Samples = 20; ///< The ESP32 as an analog-to-digital converter (ADC) which is no particularly exact. 20 samples per reading provide a sufficiently stable result.  
#else
const uint8_t AnalogReadPin = 0; ///< Analog pin to read the battery voltage.
const float RefVoltage = 5.0f; ///< Arduino boards operate on 5.0V
const uint8_t Samples = 1; ///< Arduino ADC are very precise. One sample will result in a reliable reading already.
#endif
const uint8_t BatteryCount = 2; ///< Number of batteries that are in series. In 9V blocks two Lithium Polymer cells are in series.
const uint32_t R1 = 47000; ///< Resistance value of first resistor in voltage divider.
const uint32_t R2 = 22000; ///< Resistance value of second resistor in voltage divider.

void setup() {
    // start serial communication
    Serial.begin(115200);
    // setup battery measurements
    Battery.begin(AnalogReadPin, BatteryCount, R1, R2, ArduForge::BatteryIndicator::TYPE_LIPO, RefVoltage);
}//setup


void loop() {
    // print status of the battery
    // Reading will be erratic if nothing is connected
    Serial.print("Battery at ");
    Serial.print(Battery.charge(Samples));
    Serial.print("% (");
    Serial.print(Battery.read(Samples));
    Serial.print("V)\n");

    delay(2000);
}//loop