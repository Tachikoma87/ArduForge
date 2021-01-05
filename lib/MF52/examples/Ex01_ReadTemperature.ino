/*****************************************************************************\
*                                                                           *
* File(s): BasicReadTemperature.ino                                         *
*                                                                           *
* Content: Example sketch that shows basic usage of the MF52 Thermistor     *
*          library.                                                         *
*                                                                           *
*                                                                           *
* Author(s): Tom Uhlmann                                                    *
*                                                                           *
*                                                                           *
* The file(s) mentioned above are provided as is under the terms of the     *
* FreeBSD License without any warranty or guaranty to work properly.        *
* For additional license, copyright and contact/support issues see the      *
* supplied documentation.                                                   *
*                                                                           *
\****************************************************************************/
#include <Arduino.h>
#include <MF52.h>

ArduForge::MF52 Thermistor;
/*
* This example sketch shows how to use an MF52 thermistor with ArduForge.
* We are going to provide the voltage divider only a short amount of time with power through digital pin 2.
* By supplying constant current (i.e. connecting to 5V pin) the thermistor is dissipating
* heat causing a self-heating effect, hence providing us with inaccurate measurements regarding the surrounding medium.
* I experienced more than 0.5°C of self-heating effect. Try yourself, the sketch also works by connecting to a constant voltage source.
* Connect the Arduino an thermistor according to the provided schematic.
*/
const int8_t SupplyPin = 2; // We use digital pin 2 to supply power.
const int8_t ReadPin = 0; // We use analog pin 0 to read the data.

void setup(void){
    // initialize serial connection so we can print our measurements
    Serial.begin(115200);
    const int32_t R2 = 10000; ///< Second resistor of the voltage divider
    const int16_t Beta = 3425; ///< Beta value of the thermistor (see datasheet and/or part number)
    const int32_t R25 = 10000; ///< Thermistor's resistance at room temperature (see datasheet and/or part number)
    Thermistor.begin(ReadPin, R2, Beta, R25);
    pinMode(SupplyPin, OUTPUT);
}//setup

void loop(void){
    // provide the circuitry with power, read temperature, turn of power
    digitalWrite(SupplyPin, HIGH);
    const float TempCelsius = Thermistor.read();
    const float TempFahrenheit = ArduForge::MF52::celsiusToFahrenheit(TempCelsius);
    digitalWrite(SupplyPin, LOW);
    Serial.print("The room temperature is "); 
    Serial.print(TempCelsius); Serial.print("°C (");
    Serial.print(TempFahrenheit); Serial.print("°F)");
    Serial.println(".");
    delay(2000);
}//loop