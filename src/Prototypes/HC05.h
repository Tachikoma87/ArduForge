/*****************************************************************************\
*                                                                           *
* File(s): HC05.h and HC05.cpp                                              *
*                                                                           *
* Content: This class handles communication with and via a HC05 BTLE module.*
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

#ifndef ARDUFORGE_HC05_H
#define ARDUFORGE_HC05_H 

#include <SoftwareSerial.h>
#include <inttypes.h>

namespace ArduForge{
    class HC05{
    public:
        enum struct State: int8_t{
            UNKNOWN = -1,
            INITIALIZED = 0,
            READY = 1,
            PAIRABLE = 2,
            PAIRED = 3,
            INQUIRING = 4,
            CONNECTING = 5,
            CONNECTED = 6,
            DISCONNECTED = 7,
        };

        enum struct Role: int8_t{
            SLAVE = 0,
            MASTER = 1,
            SLAVE_ECHO = 2,
        };

        HC05(void);
        ~HC05(void);

        bool begin(int8_t RXPin, int8_t TXPin, int8_t EnPin, int8_t StatePin, int16_t Baud = 9600);
        void end(void);

        int16_t available(void);
        uint8_t read(void);
        void write(uint8_t Msg);
        template<typename T>
        void print(T Msg){
            if(nullptr != m_pCom){
                m_pCom->print(Msg);
            }
        }//print


        String version(void);
        String name(void);
        String address(void);
        String pswd(void);
        Role role(void);
        State state(void);
        String help(void);


        bool check(void);

    protected:

        String parseAnswer(String Answer);

        int8_t m_RXPin;
        int8_t m_TXPin;
        int8_t m_EnPin;
        int8_t m_StatePin;
        SoftwareSerial *m_pCom;
    };//HC05 

}//name space

#endif