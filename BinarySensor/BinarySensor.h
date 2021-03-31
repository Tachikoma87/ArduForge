/*****************************************************************************\
*                                                                           *
* File(s): BinarySensor.h and BinarySensor.cpp                              *
*                                                                           *
* Content: Class to handle binary sensors (e.g. switches, reed sensors)     *
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

#ifndef ARDUFORGE_BINARYSENSOR_H
#define ARDUFORGE_BINARYSENSOR_H 

#include <inttypes.h>

namespace ArduForge{

    /**
     * \brief This class reads a binary sensor and debounces its input.
     * 
     * Binary sensors such as switches or reed sensors can have a LOW or HIGH state but can deliver a noisy signal when they switch states.
     * Therefore the signal needs to be debounced. This class debounces the signal and returns LOW or HIGH
     * only if the signal is stable (-1 in case of an unstable signal). A signal is considered stable if 
     * it does not change a certain number times with a delay between each read.
     * \image html BinarySensorWiring.png
     */
    class BinarySensor{
    public:
        /**
         * \brief Constructor
         */
        BinarySensor(void);
        
        /**
         * \brief Destructor
         */
        ~BinarySensor(void);
        
        /**
         * \brief Initializes communication with the sensor (set configuration details).
         * \param[in] Pin Digital pin where to read from.
         * \param[in] Pullup Use pullup resistor?
         * \param[in] StableReadingsThreshold Defines number of equal readings so a signal can be considered stable.
         */
        void begin(int8_t Pin, bool Pullup = true, int8_t StableReadingsThreshold = 4);

        /**
         * \brief Ends communication with the sensor.
         */
        void end(void);

        /**
         * \brief Reads the sensor's state (debounced).
         * \param[in] Delay Delay in milliseconds between each read.
         */
        int8_t read(const int8_t Delay = 5);

    protected:
        int8_t m_SensorPin; ///< Digital pin to which the binary sensor is connected.
        int8_t m_StableReadingThreshold; ///< Number of equal readings that defines a stable signal.
    };//BinarySensor

}//name space

#endif