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

        void begin(int8_t ClkPin, int8_t DtPin, int8_t ButtonPin);
        void end(void);
        void update(void);

        bool buttonState(void)const;
        int8_t direction(void)const;
        int32_t position(void)const;
        int32_t resetPosition(void);

    private:
        int8_t m_Clk;
        int8_t m_Dt;
        int8_t m_Button;
        int8_t m_LastState;

        int32_t m_RotaryPosition;
        int8_t m_Dir; // 0 for CW and 1 for CCW
        bool m_ButtonState;
    };//RotaryEncoder

}//name space


#endif