/*****************************************************************************\
*                                                                           *
* File(s): RotaryEncoder.h and RotaryEncoder.cpp                            *
*                                                                           *
* Content: Class to track the state of an incremental rotary encoder.       *
*                                                                           *
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

#ifndef ARDUFORGE_ROTARYENCODER_H
#define ARDUFORGE_ROTARYENCODER_H 

#include <inttypes.h>

namespace ArduForge{
    
    /**
     * \brief This class tracks the position of an incremental rotary encoder.
     * 
     * The state of the rotary encoder changes positively or negatively based on the direction the shaft is turned. Many devices also feature a switch that is activated by pushing the shaft. The switch pin can be configured to use a pullup input. The RotaryEncoder::update method has to be called every time the clock signal changes. Use polling or interrupt (see examples).
     * \image html RotaryEncoderWiring.png
     * \remarks There are many types of rotary encoder. This class is suited to handle KY-040 devices and comparable.
     */
    class RotaryEncoder{
    public:
        static const uint8_t DIR_CW = 0; ///< Clock wise direction
        static const uint8_t DIR_CCW = 1; ///< Counter clock wise direction

        /**
         * \brief Constructor
         */
        RotaryEncoder(void);

        /**
         * \brief Destructor
         */
        ~RotaryEncoder(void);

        /**
         * \brief Starts communication with the rotary encoder.
         * \param[in] ClkPin Clock pin
         * \param]in] DtPin DtPin
         * \param[in] SwitchPin Switch pin.
         * \param[in] Pullup Specify whether to use a pullup input for the switch.
         */
        void begin(uint8_t ClkPin, uint8_t DtPin, uint8_t SwitchPin, bool Pullup = true);

        /**
         * \brief Ends communication with the device.
         */
        void end(void);

        /**
         *  \brief Updates state and position of the encoder. Has to be called every time the clock signal changes.
         * 
         * Attaching an interrupt to the clock signal and calling this method when the interrupt occurs is the preferred approach to not miss any signals.
         */
        void update(void);

        /**
         * \brief Returns the current switch state. 
         * 
         * \return True if switch is pressed, false otherwise.
         * \remarks Switch is not debounced.
         */
        bool switchState(void)const;

        /**
         * \brief Returns the direction the shaft was last turned.
         * 
         * \return DIR_CW or DIR_CCW
         */
        uint8_t direction(void)const;

        /**
         * \brief Returns the current position.
         * 
         * \return Current position.
         */
        int32_t position(void)const;

        /**
         * \brief Resets the position tracking to 0.
         * 
         * \return Last position before the reset.
         */
        int32_t resetPosition(void);

    private:
        uint8_t m_Clk; ///< Clock pin
        uint8_t m_Dt; ///< DT pin
        uint8_t m_Switch; ///< Switch pin.
        int8_t m_LastState; ///< Last state of the clock signal.

        int32_t m_RotaryPosition; ///< Current position. 
        uint8_t m_Dir; ///< Direction the encoder is turning. Can be DIR_CW or DIR_CCW.
        bool m_SwitchState; ///< State of the switch. True if pressed, false otherwise.
    };//RotaryEncoder

}//name space


#endif