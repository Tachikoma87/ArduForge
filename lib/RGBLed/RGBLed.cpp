#include "RGBLed.h"

namespace ArduForge{
    RGBLed::RGBLed(void){
        m_RedPin = 0xFF;
        m_GreenPin = 0xFF;
        m_BluePin = 0xFF;
    }//Constructor

    RGBLed::~RGBLed(void){
        m_RedPin = 0xFF;
        m_GreenPin = 0xFF;
        m_BluePin = 0xFF;
    }//Destructor

    void RGBLed::begin(uint8_t RedPin, uint8_t GreenPin, uint8_t BluePin, bool PWM){
        m_RedPin = RedPin;
        m_GreenPin = GreenPin;
        m_BluePin = BluePin;
        m_PWM = PWM;

        if(m_RedPin < 0xFF) pinMode(m_RedPin, OUTPUT);
        if(m_GreenPin < 0xFF) pinMode(m_GreenPin, OUTPUT);
        if(m_BluePin < 0xFF) pinMode(m_BluePin, OUTPUT);
        color(0,0,0);
    }//begin

    void RGBLed::end(void){
        m_RedPin = 0xFF;
        m_GreenPin = 0xFF;
        m_BluePin = 0xFF;
    }//end

    void RGBLed::color(uint8_t Red, uint8_t Green, uint8_t Blue){
        red(Red);
        green(Green);
        blue(Blue);
    }//color

    void RGBLed::red(uint8_t Value){
        if(0xFF == m_GreenPin) return;
        if(m_PWM) analogWrite(m_RedPin, Value);
        else digitalWrite(m_RedPin, (Value > 0) ? HIGH : LOW);
    }//red

    void RGBLed::green(uint8_t Value){
        if(0xFF == m_GreenPin) return;
        if(m_PWM) analogWrite(m_GreenPin, Value);
        else digitalWrite(m_GreenPin, (Value > 0) ? HIGH : LOW);
    }//green

    void RGBLed::blue(uint8_t Value){
        if(0xFF == m_BluePin) return;
        if(m_PWM) analogWrite(m_BluePin, Value);
        else digitalWrite(m_BluePin, (Value > 0) ? HIGH : LOW);
    }//blue

}//name space