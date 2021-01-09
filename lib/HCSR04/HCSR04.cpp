#include <Arduino.h> 
#include "HCSR04.h"

namespace ArduForge{
    HCSR04::HCSR04(void){
        m_Trigger = 0xFF;
        m_Echo = 0xFF;
    }//Constructor

    HCSR04::~HCSR04(void){
        end();
    }//Destructor

    void HCSR04::begin(uint8_t TriggerPin, uint8_t EchoPin){
        m_Trigger = TriggerPin;
        m_Echo = EchoPin;

        pinMode(m_Trigger, OUTPUT);
        pinMode(m_Echo, INPUT);
    }//begin 

    void HCSR04::end(void){
        m_Trigger = 0xFF;
        m_Echo = 0xFF;
    }//end

    float HCSR04::read(uint8_t Retries){
        float Rval = -1.0f;
        for(uint8_t i = 0; i < Retries; ++i){
            digitalWrite(m_Trigger, LOW);
            delayMicroseconds(3);
            noInterrupts();
            digitalWrite(m_Trigger, HIGH); // Trigger impulse for 10us
            delayMicroseconds(10);
            digitalWrite(m_Trigger, LOW);
            // measure time, divide by 2 since sound has to travel forth and back
            const int32_t Time = pulseIn(m_Echo, HIGH)/2;
            interrupts();
            // convert time to centimeter and return
            // or return -1 if sensor has not detected any obstacles (pulse longer than 20ms)
            if(Time < 10000){
                Rval = Time/29.1;
                break;
            } 
        }
      
        return Rval;
    }//read

}//name space