/*****************************************************************************\
*                                                                           *
* File(s): MF52.h and MF52.cpp                                              *
*                                                                           *
* Content: Class to interact with an MF52 NTC Thermistor by using a basic   *
*          voltage divider circuit.                                         *
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
#ifndef ARDUFORGE_MF52_H
#define ARDUFORGE_MF52_H

#include <inttypes.h>

namespace ArduForge{
    /**
     * \brief Class to interface an MF52 thermistor device in a voltage divider circuit.
     * 
     * Handles "communication" with an MF52 thermistor by using a basic voltage divider. You only need the thermistor's beta value
     * and the resistance at room temperature (R25). Use the part number or the datasheet 
     * (http://radio-hobby.org/uploads/datasheets/mf/mf52[1].pdf) to acquire the necessary information.
     * It is assumed that supply and reference voltage are the same.
    */
    class MF52{
        public:
            /**
             * \brief Converts a temperature value from degree Celsius to Kelvin.
             * \param[in] Temp Temperature value to convert.
             * \return Temperature value in Celsius.
            */
            static float celsiusToKelvin(float Temp);

            /**
             * \brief Converts a temperature value form degree Kelvin to Celsius.
             * \param[in] Temp Temperature value to convert.
             * \return Temperature value in Kelvin.
            */
            static float kelvinToCelsius(float Temp);

            /**
             * \brief Converts a temperature value from degree Fahrenheit to Celsius.
             * \param[in] Temp Temperature value to convert.
             * \return Temperature value in Fahrenheit.
            */
            static float celsiusToFahrenheit(float Temp);

            /**
             * \brief Converts a temperature value from degree Celsius to Fahrenheit.
             * \param[in] Temp Temperature value to convert.
             * \return Temperature value in Celsius.
            */
            static float fahrenheitToCelsius(float Temp);

            /**
             * \brief Constructor. Initializes default values. 
            */
            MF52(void);
            /**
             * \brief Destructor. Calls end(). 
            */
            ~MF52(void);

            /**
             * \brief Initializes the thermistor by defining the circuit and device properties. 
             * \param[in] ReadPin Analog pin to read voltage from.
             * \param[in] R2 Resistance value of resistor \f$ R_2\f$.
             * \param[in] Beta MF52's \f$\beta\f$ value. See datasheet of your thermistor.
             * \param[in] R25 MF52's resistance at room temperature (25°C)
            */
            void begin(int8_t ReadPin, int32_t R2 = 10000, int16_t Beta = 3435, int32_t R25 = 10000); 

            /**
             * \brief Ends "communication" with the device. Resets to default values. 
            */
            void end(void);

            /**
             * \brief Checks whether the device was initialized.
             * \return true if begin was called, false if invalid values are specified
             * \note There is actually no way to check whether the thermistor (i.e. the analog pin) provides valid data. Check you circuit if values go berserk.
            */
            bool isConnected(void)const;

            /**
             * \brief Reads the analog port and returns temperature in °C. Multiple successive sample may be taken to increase stability.
             * \param[in] SampleCount Number of samples to take.
             * \param[in] Delay Delay in milliseconds between each sample.
             * \return Temperature in °C.
            */
            float read(int8_t SampleCount = 1, int8_t Delay = 10)const;

            /**
             * \brief Sets the calibration offset for the circuit.
             * \param[in] Offset Calibration offset in Ohm.
             * 
             * The circuit's response can quite vary depending on the thermistor and resistor quality you use. Provide an offset to mitigate this problem.
             * You can find an appropriate value by calibration. 
            */
            void calibrationOffset(int16_t Offset);
        private:
            int8_t m_ReadPin; ///< Analog Pin to read value V0 from.
            int32_t m_R2; ///< Second resistor of the voltage divider.
            int16_t m_Beta; ///< Beta value of the Thermistor.
            int32_t m_R25; ///< Resistance of the Thermistor at Room Temperature (25°C).
            int16_t m_CalibrationOffset; ///< Calibration offset in Ohm. Use to mitigate hardware tolerance. 
    };//MF52

}//name-space

#endif