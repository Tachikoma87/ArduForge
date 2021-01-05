/*****************************************************************************\
*                                                                           *
* File(s): HCSR04.h and HCSR04.cpp                                          *
*                                                                           *
* Content: Class to use HC-SR04 ultrasonic sensor to measure distance.      *
*                                                                           *
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

#ifndef ARDUFORGE_HCSR04_H
#define ARDUFORGE_HCSR04_H 

namespace ArduForge{
    class HCSR04{
    public:
        HCSR04(void);
        ~HCSR04(void);

        void begin(int8_t TriggerPin, int8_t EchoPin);
        void end(void);

        float read(void);
    protected:
        int8_t m_Trigger;
        int8_t m_Echo;
    };//HCSR04

}//name space


#endif