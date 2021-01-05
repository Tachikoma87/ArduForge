/*****************************************************************************\
*                                                                           *
* File(s): RotaryEncoder.h and RotaryEncoder.cpp                            *
*                                                                           *
* Content: Class to handle input of an incremental rotary encoder.          *
*                                                  *
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

#ifndef ARDUFORGE_ROTARYENCODER_H
#define ARDUFORGE_ROTARYENCODER_H 

#include <inttypes.h>

namespace ArduForge{
    
    class RotaryEncoder{
    public:
        static const int8_t DIR_CW = 0;
        static const int8_t DIR_CCW = 1;

        RotaryEncoder(void);
        ~RotaryEncoder(void);

        void begin(int8_t ClkPin, int8_t DtPin, int8_t SwitchPin);
        void end(void);
        void update(void);

        bool buttonState(void)const;
        int8_t direction(void)const;
        int32_t position(void)const;
        int32_t resetPosition(void);

    private:
        int8_t m_Clk; ///< Clk pin.
        int8_t m_Dt; ///< DT pin.
        int8_t m_Switch; ///< Pin of the button.
        int8_t m_LastState; ///< Last state of the clk signal.

        int32_t m_RotaryPosition; ///< Current position. 
        int8_t m_Dir; ///< Direction the encoder is turing. Can be DIR_CW or DIR_CCW.
        bool m_SwitchState; ///< State of the switch. True if pressed, false otherise.
    };//RotaryEncoder

}//name space


#endif