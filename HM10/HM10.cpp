#include <Arduino.h>
#include "HM10.h"

namespace ArduForge{

    uint32_t HM10::discoverBaud(uint8_t RXPin, uint8_t TXPin){
        uint32_t BaudList[12] = {75, 300, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200};
        uint32_t Rval = 0;
        uint32_t Timeout = 0;
        SoftwareSerial UART(RXPin, TXPin);  
        for(uint8_t i=0; i < 12; ++i){  
            UART.begin(BaudList[i]);    
            UART.write("AT\r\n");
            Timeout = millis();
            while(millis() - Timeout < 500){
                if(UART.available() > 0) Rval = BaudList[i];
            }
            UART.end();
            if(Rval > 0) break;
        }//for[all bauds]

        return Rval;
    }//discoverBaud


    HM10::HM10(void){
        m_RX = 0xFF;
        m_TX = 0xFF;
    }//Constructor

    HM10::~HM10(void){
        end();
    }//Destructor

    void HM10::begin(uint8_t RXPin, uint8_t TXPin, uint8_t StatePin, const char *pName, bool Reset, uint32_t Baud){
        m_RX = RXPin;
        m_TX = TXPin;
        m_State = StatePin;
       
        // create software serial and start communication with specified baud rate
        if(nullptr == m_pCom) m_pCom = new SoftwareSerial(m_RX, m_TX);
        m_pCom->begin(Baud);
        // wait a little bit to establish connection

        pinMode(m_State, INPUT);

        if(!connected() && Reset){
            char Reply[32];
            issueATCommand("AT+RESET\r\n", (uint8_t*)Reply);;
        }
        if(!connected() && nullptr != pName){
            char Name[32]; // maximum number of name is 22 characters, so 32 should be enough
            sprintf(Name, "AT+NAME%s\r\n", pName);
            issueATCommand(Name, (uint8_t*)Name);
        }
        
        // clear any residuals on UART
        delay(50);   
        while(m_pCom->available()) m_pCom->read();
    }//begin 

     void HM10::end(void){
        if(nullptr != m_pCom) {
            m_pCom->end();
            // deleting the object does not seem to be working
            // compiler complains about non-virtual destructor ...
            //delete m_pCom;
        }

        m_RX = 0xFF;
        m_TX = 0xFF;
    }//end

    uint16_t HM10::available(void){
        return (nullptr != m_pCom) ? m_pCom->available() : 0;
    }//available 

    uint8_t HM10::read(void){
        return (nullptr != m_pCom && m_pCom->available() != 0) ? m_pCom->read() : 0;
    }//read

    uint16_t HM10::read(uint8_t *pBuffer, uint16_t ByteCount){
        return (nullptr != m_pCom && nullptr != pBuffer) ? m_pCom->readBytes(pBuffer, ByteCount) : 0;
    }//read

    uint16_t HM10::write(uint8_t *pBuffer, uint16_t ByteCount){
        return (nullptr != m_pCom) ? m_pCom->write(pBuffer, ByteCount) : 0;
    }//write

    
    bool HM10::check(void){
        uint8_t Buffer[8];
        bool Rval = false;
        if(0 < issueATCommand("AT\r\n", Buffer)){
            if(Buffer[0] == 'O' && Buffer[1] == 'K') Rval = true;
        }
        return Rval;
    }//check

    bool HM10::connected(void)const{
        return (digitalRead(m_State) == HIGH);
    }//connected

    int16_t HM10::issueATCommand(const char *pCmd, uint8_t *pAnswerBuffer, uint16_t Timeout){
        if(nullptr == m_pCom  || nullptr == pCmd) return -1;

        // issue the AT command
        m_pCom->write(pCmd);
        // should we wait for answer? if not return
        if(nullptr == pAnswerBuffer) return 0;

        int16_t Rval = 0;
        uint16_t Start = millis();
        while(millis() - Start < Timeout && Rval == 0){
            while(m_pCom->available() != 0){
                pAnswerBuffer[Rval++] = m_pCom->read();
                delay(5);
            } 
            delay(50);
        }
        return Rval;
    }//issueATCommand

    SoftwareSerial *HM10::serialConnection(void){
        return m_pCom;
    }//serialConnection

}//name space