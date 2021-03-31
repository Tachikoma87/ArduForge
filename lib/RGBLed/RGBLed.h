/*****************************************************************************\
*                                                                           *
* File(s): RGBLed.h and RGBLed.cpp                                          *
*                                                                           *
* Content: Class to interface with a standard RGB Led with common           *
*          cathode.                                                         *
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
#ifndef ARDUFORGE_RGBLED_H
#define ARDUFORGE_RGBLED_H

#include <inttypes.h>

namespace ArduForge{

    /**
     * \brief A simple and lightweight class to interface with a common 5mm RGB Led with common cathode.
     * 
     * The color of the RGB Led is defined by the strength of each individual power. You have to use the PWM pins on your microcontroller to get 
     * varying colors. If you just want to turn each color on and off you can use any digital pin. Make sure to specify in the RGBLed::begin function,
     * that you don't want to use PWM.
     * 
     * \image html RGBLedWiring.png
     */
    class RGBLed{
    public:
        /**
         * \brief Constructor
         */
        RGBLed(void);
        
        /**
         * \brief Destructor
         */
        ~RGBLed(void);

        /**
         * \brief Set the pin configuration and PWM feature.
         * \param[in] RedPin Pin to the LED's red channel
         * \param[in] GreenPin Pin to the LED's green channel
         * \param[in] BluePin Pin to the LED's blue channel.
         * \param[in] PWM Use PWM feature. All pins must be PWM pins if this is true.
         * \remarks If you set PMW to false you can turn each color only on and off.
         */
        void begin(uint8_t RedPin, uint8_t GreenPin, uint8_t BluePin, bool PWM = true);

        /**
         * \brief End communication with the RGB Led.
         */
        void end(void);

        /**
         * \brief Set an RGB color.
         * \param[in] Red Red channel strength [0,255].
         * \param[in] Green Green channel strength [0,255].
         * \param[in] Blue Blue channel strength [0, 255].
         * \remarks If PWM feature is off 0 will result in turning channel off, all other values will turn channel on.
         */
        void color(uint8_t Red, uint8_t Green, uint8_t Blue);

        /**
         * \brief Set red channel strength.
         * \param[in] Value Channel strength [0,255].
         */
        void red(uint8_t Value);

        /**
         * \brief Set green channel strength.
         * \param[in] Value Channel strength [0,255]
         */
        void green(uint8_t Value);

        /**
         * \brief Set blue channel strength.
         * \param[in] Value Channel strength.
         */
        void blue(uint8_t Value);
    private:
        uint8_t m_RedPin;   ///< Pin of LED's red channel
        uint8_t m_GreenPin; ///< Pin of LED's green channel
        uint8_t m_BluePin;  ///< Pin of LED's blue channel
        bool m_PWM;         ///< Pulse width modulation feature switch
        #ifdef ESP32
        static const uint8_t PWMChannelR = 0; ///< PWM channel for red channel.
        static const uint8_t PWMChannelG = 1; ///< PWM channel for green channel.
        static const uint8_t PWMChannelB = 2; ////< PWM channel for blue channel.
        #endif
    };//RGBLed


}//name space


#endif