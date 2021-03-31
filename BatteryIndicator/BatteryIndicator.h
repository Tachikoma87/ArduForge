/*****************************************************************************\
*                                                                           *
* File(s): BatteryIndicator.h and BatteryIndicator.cpp                      *
*                                                                           *
* Content: A battery indicator class that measures the voltage and          *
*          remaining capacity of different types of rechargeable batteries. *
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
#ifndef ARDUFORGE_BATTERYINDICATOR_H
#define ARDUFORGE_BATTERYINDICATOR_H

#include <Arduino.h>
#include <inttypes.h>

namespace ArduForge{
    /**
     * \brief Measures voltage and remaining charge of various types of rechargeable batteries.
     * 
     * The voltage of connected batteries is measured at an analog pin and translated to a relative battery charge base on cell type and number of cells in series. If the voltage to measure exceeds the boards maximum specifications, you can use a voltage divider.
     * \image html BatteryIndicator.png
     * \remarks Battery measurement is not an exact science. Remaining charge highly depends on quality and age of the batteries and can vary considerably between manufacturers. Make sure to provide the correct voltage your boards operates with in BatteryIndicatorNiMH::begin. Even if it is a 5V pin, it is unlikely that it will output 5V exactly. I measured 4.98V on an Uno and 4.71V on a Nano. These voltage differences have a considerable impact on measurement accuracy.
     * 
     */
    class BatteryIndicator{
    public:
        /**
         * \brief Battery type enumerate.
         */
        enum BatteryType:uint8_t{
            TYPE_NIMH, ///< Nickle Metal Hydride with voltage range from 1.0 to 1.4.
            TYPE_LIPO, ///< Lithium Polymer with voltage range from 3.2 to 4.2.
        }; //BatteryType

        /**
         * \brief Constructor
         */
        BatteryIndicator(void);

        /**
         * \brief Destructor
         */
        ~BatteryIndicator(void);

        /**
         * \brief Sets up the battery measurement.
         * 
         * \param[in] BatteryPin Analog pin to read battery voltage.
         * \param[in] BatteryCount Number of batteries that are connected in series.
         * \param[in] R1 Resistor value of the first Resistor in the voltage divider.
         * \param[in] R2 Resistor value of the second Resistor in the voltage divider.
         * \param[in] Type Type of connected batteries. 
         * \param[in] ReferenceVoltage Absolute voltage the board operates on.
         */
        void begin(uint8_t BatteryPin, uint8_t BatteryCount = 1, uint32_t R1 = 47000, uint32_t R2 = 22000, BatteryType Type = TYPE_LIPO, float ReferenceVoltage = 4.98f);

        /**
         * \brief Ends communication with the battery.
         */
        void end(void);

        /**
         * \brief Measures the voltage at the pin.
         * \param[in] SampleCount Number of consecutives reads. Result will be average of all samples. Execution time of functions increases be 1ms for each sample.
         * \return Voltage the connected batteries provide.
         */
        float read(uint8_t SampleCount = 1);

        /**
         * \brief Computes the remaining charge of the battery in percent.
         * \param[in] SampleCount Number of consecutives reads. Result will be average of all samples.
         * \return Remaining charge of the batteries in percent [0, 100].
         */
        uint8_t charge(uint8_t SampleCount = 1);

        /**
         * \brief Returns battery type.
         * \return Battery type.
         */
        BatteryType type(void)const;

        /**
         * \brief Tells the class to user different bit resolution than default one (12 Bit on ESP32 and 10 Bit on Arduino).
         * \param[in] BitCount Resolution in bits of the ADC.
         * \remarks Only relevant for ESP32 and if default resolution was changed.
         */
        void analogResolution(uint8_t BitCount);

    private:       
        float m_RefVoltage;     ///< Absolute voltage the board operates with.
        float m_R1;             ///< Resistor value 1.
        float m_R2;             ///< Resistor value 2.
        float m_VoltageMin;     ///< Minimum voltage of a single cell (dependents on type)
        float m_VoltageMax;     ///< Maximum voltage of a single cell (dependents on type)
        uint8_t m_BatteryPin;   ///< Analog pin the battery is connected to.
        uint8_t m_BatteryCount; ///< Number of batteries that are connected in series.
        BatteryType m_Type;     ///< Battery type.
        float m_Resolution;     ///< Analog resolution. Required for converting reading to voltage value.

    };//BatteryIndicator
}


#endif 