/*****************************************************************************\
*                                                                           *
* File(s): BatteryIndicatorNiMh.h and BatteryIndicatorNiMh.cpp              *
*                                                                           *
* Content: A battery indicator class that measures the voltage and          *
*          remaining capacity of a Nickle Metal Hydride (NiMh) rechargeable.*
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
#ifndef ARDUFORGE_BATTERYINDICATORNIMH_H
#define ARDUFORGE_BATTERYINDICATORNIMH_H

#include <Arduino.h>
#include <inttypes.h>

namespace ArduForge{
    /**
     * \brief Measures voltage and remaining charge of a common NiMh rechargeable battery.
     * 
     * The voltage of connected batteries is measured at an analog pin and translated to a relative battery charge. A default rechargeable battery will provide an average voltage of about 1.2V. We will assume that a battery is fully charged (100%) at 1.3V and fully depleted (0%) at 1.05V. Typical Cut Off voltage for NiMh cells is 1.0V. Draining charge below this lower bound value may destroy the battery, so don't do that. If we define 1.05V as fully depleted, we introduce a small security margin.
     * \remarks Battery measurement is not an exact science. Remaining charge highly depends on quality and age of the batteries and can vary considerably between manufacturers. Make sure to provide the correct voltage your boards operates with in BatteryIndicatorNiMH::begin. Even if it is a 5V pin, it is unlikely that it will output 5V exactly. I measured 4.98V on an Uno and 4.71V on a Nano. These voltage differences have a considerable impact on measurement accuracy, if the wrong voltage is assumed.
     * 
     */
    class BatteryIndicatorNiMh{
    public:
        /**
         * \brief Constructor
         */
        BatteryIndicatorNiMh(void);

        /**
         * \brief Destructor
         */
        ~BatteryIndicatorNiMh(void);

        /**
         * \brief Sets up the battery measurement.
         * 
         * \param[in] BatterPin Analog pin for the batterie's anode
         * \param[in] BatteryCount Number of batteries that are connected in series.
         * \param[in] ReferenceVoltage Absolute voltage the board provides to its pins.
         */
        void begin(uint8_t BatteryPin, uint8_t BatteryCount = 1, float ReferenceVoltage = 4.98f);

        /**
         * \brief Ends communication with the battery.
         */
        void end(void);

        /**
         * \brief Measures the voltage, the battery cell provides.
         * 
         * \return Voltage the connected batteries provide.
         */
        float read(void);

        /**
         * \brief Computes the remaining charge of the battery in percent.
         * 
         * \return Remaining charge of the batteries in percent [0, 100].
         */
        uint8_t charge(void);

    private:       
        float m_RefVoltage;     ///< Absolute voltage the board operates with.
        uint8_t m_BatteryPin;   ///< Analog pin the battery is connected to.
        uint8_t m_BatteryCount; ///< Number of batteries that are connected in series.

    };//BatteryIndicatorNiMh
}


#endif 