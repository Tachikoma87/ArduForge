/*****************************************************************************\
*                                                                           *
* File(s): HCSR04.h and HCSR04.cpp                                          *
*                                                                           *
* Content: Class to use HC-SR04 ultrasonic sensor for distance measurement. *
*                                                                           *
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

#ifndef ARDUFORGE_HCSR04_H
#define ARDUFORGE_HCSR04_H 

#include <inttypes.h>

namespace ArduForge{

    /**
     * \brief Implements communication with an HC-SR04 ultrasonic distance measurement device.
     * 
     * The HC-SR04 produces an ultrasonic sound and measures the time it takes for the signal to travel back to the sensor. According to the data sheet range is from 2 to 400 cm with an accuracy or \f$\pm \f$ 3 mm (specifications may differ between vendors). In any case the distance measurement highly depends on the targeted surface. Plain solid surfaces work best, while materials such as clothing can absorb all of the ultrasonic impulse which makes measurement impossible.
     * \image HTML HCSR04Wiring.png
     */
    class HCSR04{
    public:
        /**
         * \brief Constructor
         */
        HCSR04(void);

        /**
         * \brief Destructor
         */
        ~HCSR04(void);

        /**
         * \brief Initializes communication with the device.
         * 
         * \param[in] TriggerPin Trigger pin address
         * \param[in] EchoPin Echo pin address
         */
        void begin(uint8_t TriggerPin, uint8_t EchoPin);

        /**
         * \brief Ends communication with the device
         * 
         */
        void end(void);

        /**
         * \brief Perform a distance measurement.
         * 
         * The distance measurement will be performed and repeated several times (specified by Retries) if measurement fails. The first valid measurement will be returned.
         * \param[in] Retries Maximum number of measurement trials
         * \return Distance in centimeter or -1.0 if measurement fails.
         */
        float read(uint8_t Retries = 5);

        /**
         *  \brief Converts centimeter to inch
         * 
         * \param[in] Value Distance in centimeter
         * \return Distance in inch
         */
        template<typename T>
        static T centimeterToInch(T Value){
            return (T)(Value / 2.54);
        }//cmToInch

        /**
         *  \brief Converts inch to centimeter
         * 
         * \param[in] Value Distance in inch
         * \return Distance in centimeter
         */
        template<typename T>
        static T inchToCentimeter(T Value){
            return (T)(Value * 2.54);
        }//inchToCentimeter

    protected:
        uint8_t m_Trigger; ///< Trigger pin
        uint8_t m_Echo; ///< Echo pin
    };//HCSR04

}//name space

#endif