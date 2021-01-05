#include <Arduino.h> 
#include "HCSR04.h"

namespace ArduForge{
    HCSR04::HCSR04(void){
        m_Trigger = -1;
        m_Echo = -1;
    }//Constructor

    HCSR04::~HCSR04(void){
        end();
    }//Destructor

    void HCSR04::begin(int8_t TriggerPin, int8_t EchoPin){
        m_Trigger = TriggerPin;
        m_Echo = EchoPin;

        pinMode(m_Trigger, OUTPUT);
        pinMode(m_Echo, INPUT);
    }//begin 

    void HCSR04::end(void){
        m_Trigger = -1;
        m_Echo = -1;
    }//end

    float HCSR04::read(void){
        digitalWrite(m_Trigger, LOW);
        delayMicroseconds(3);
        digitalWrite(m_Trigger, HIGH); // Trigger impuls for 10us
        delayMicroseconds(10);
        digitalWrite(m_Trigger, LOW);
        // measure time, divide by 2 since sound has to travel forth and back
        const int32_t Time = pulseIn(m_Echo, HIGH)/2;
        // convert time to centimeter and return
        // or return -1 if sensor has not detected any obstances (pulse longer than 20ms)
        return (Time >= 10000) ? -1.0 : Time/29.1; 
    }//read

}//name space