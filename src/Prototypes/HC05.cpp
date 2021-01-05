#include <Arduino.h>
#include "HC05.h"



namespace ArduForge{

    HC05::HC05(void){
        m_RXPin = -1;
        m_TXPin = -1;
        m_StatePin = -1;
        m_EnPin = -1;
    }//Constructor

    HC05::~HC05(void){

    }//Destructor

    bool HC05::begin(int8_t RXPin, int8_t TXPin, int8_t EnPin, int8_t StatePin, int16_t Baud){
        m_RXPin = RXPin;
        m_TXPin = TXPin;
        m_EnPin = EnPin;
        m_StatePin = StatePin;

        // create software serial and start communication with specified baud rate
        m_pCom = new SoftwareSerial(m_TXPin, m_RXPin);
        m_pCom->begin(Baud);
        delay(1000);
        // m_pCom->write("AT+RENEW\r\n");
        // m_pCom->write("AT+RESET\r\n");
        delay(2000);
        //m_pCom->write("AT+ORGL\r\n");
        //m_pCom->write("AT+ROLE0\r\n");
        m_pCom->write("AT+NAMEtarget\r\n");
        //m_pCom->write("AT+PASS553344\r\n");
        m_pCom->write("AT+ROLE0\r\n");
        delay(50);
        Serial.print(m_pCom->readString());
        //m_pCom->write("AT+NAME=bluinooo\r\n");
        // delay(50);
        // while(m_pCom->available()) m_pCom->read();
    }//begin 

     void HC05::end(void){
        if(nullptr != m_pCom) delete m_pCom;
        m_pCom = nullptr; 

        m_RXPin = -1;
        m_TXPin = -1;
        m_StatePin = -1;
        m_EnPin = -1;
    }//end

    bool HC05::check(void){
        if(nullptr != m_pCom){
            m_pCom->write("AT\r\n");
            delay(50);
            String Answer = m_pCom->readString();
            return (Answer.length() > 1 && Answer[0] == 'O' && Answer[1] == 'K') ? true : false;
        }


        return false;
    }//check

    String HC05::help(void){
        String Rval;
        if(nullptr != m_pCom){
            m_pCom->write("AT+HELP?\r\n");
            Rval = m_pCom->readString();
        }
        return Rval;
    }//help

    String HC05::version(void){
        String Rval;
        if(nullptr != m_pCom){
            m_pCom->write("AT+VERSION?\r\n");
            delay(20);
            Rval = parseAnswer(m_pCom->readString());
        }
        return Rval;
    }//version 

    String HC05::name(void){
        String Rval;
        if(nullptr != m_pCom){
            m_pCom->write("AT+NAME?\r\n");
            delay(50);
            Rval = m_pCom->readString();
        }
        return Rval;
    }//name 

    String HC05::address(void){
        String Rval;
        if(nullptr != m_pCom){
            m_pCom->write("AT+ADDR?\r\n");
            delay(20);
            Rval = parseAnswer(m_pCom->readString());
        }
        return Rval;
    }//address 

    String HC05::pswd(void){
        String Rval;
        if(nullptr != m_pCom){
            m_pCom->write("AT+PASS?\r\n");
            delay(20);
            Rval = parseAnswer(m_pCom->readString());
        }
        return Rval;
    }//pin

    HC05::Role HC05::role(void){
        Role Rval = Role::SLAVE;
        if(nullptr != m_pCom){
            m_pCom->write("AT+ROLE?\r\n");
            delay(20);
            String Answer = parseAnswer(m_pCom->readString());
            Serial.println(Answer);
            if(Answer[0] == '0') Rval = Role::SLAVE;
            else if(Answer[0] == '1') Rval = Role::MASTER;
            else Rval = Role::SLAVE_ECHO;
        }
        return Rval;
    }//role

    HC05::State HC05::state(void){
        HC05::State Rval;
        if(m_pCom != nullptr){
            m_pCom->write("AT+STATE?\r\n");
            delay(50);
            String Answer = m_pCom->readString();
            Serial.print(Answer);
        }
    }//state    

    int16_t HC05::available(void){
        if(nullptr != m_pCom) return m_pCom->available();
    }//available 

    uint8_t HC05::read(void){
        return (nullptr != m_pCom) ? m_pCom->read() : 0;
    }//read

    void HC05::write(uint8_t Msg){
        if(nullptr != m_pCom) m_pCom->write(Msg);
    }//write

    String HC05::parseAnswer(String Answer){
        String Rval;
        for(int8_t i=0; i < Answer.length(); ++i){
            if(Answer[i] == '='){
                Rval = String(&Answer.c_str()[i+1]);
                break;
            }
        }//for[Answer]
        return Rval;
    }//parseAnswer

}//name space