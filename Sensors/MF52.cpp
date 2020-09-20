#include <Arduino.h>
#include "MF52.h"

namespace ArduForge{

    float MF52::celsiusToKelvin(float Temp){
        return Temp + 273.15;
    }//toKelvin

    float MF52::kelvinToCelsius(float Temp){
        return Temp - 273.15;
    }//kelvinToCelsius

    float MF52::celsiusToFahrenheit(float Temp){
        return (Temp * 9.0/5.0) + 32.0;
    }//celsiusToFahrenheit

    float MF52::fahrenheitToCelsius(float Temp){
        return (Temp - 32.0) * 5.0/9.0;
    }//fahrenheitToCelsius

    MF52::MF52(void){
        m_ReadPin = -1;
        m_R2 = 0;
        m_Beta = 0;
        m_R25 = 0;
        m_CalibrationOffset = 0;
    }//Constructor

    MF52::~MF52(void){
        end();
    }//Destructor

    void MF52::begin(int8_t ReadPin, int32_t R2, int16_t Beta, int32_t R25){
        m_ReadPin = ReadPin;
        m_R2 = R2;
        m_Beta = Beta;
        m_R25 = R25;
    }//begin 

    void MF52::end(void){
        m_ReadPin = -1;
        m_R2 = 0;
        m_Beta = 0;
        m_R25 = 0;
        m_CalibrationOffset = 0;
    }//end

    bool MF52::isConnected(void)const{
        return (m_ReadPin >= 0);
    }//isConnected

    float MF52::read(int8_t SampleCount, int8_t Delay)const{
        int16_t Reading = analogRead(m_ReadPin);
        for(int8_t i=1; i < SampleCount; ++i){
                Reading += analogRead(m_ReadPin);
                delay(Delay);
        }//for[readings]
        // compute MF52's current resistance 
        // we offset the second resistor with the calibration value       
        const int32_t CalibR2 = m_R2 + m_CalibrationOffset;
        const float Resistance = (float)((1023*CalibR2)/(Reading/SampleCount) - CalibR2);
        // simplified Steinhart-Hart equation (we only require Beta and R25)
        const float InvTemp = log(Resistance/m_R25)/m_Beta + 1.0/298.15;
        return 1.0/InvTemp - 273.15;
    }//read

    void MF52::calibrationOffset(int16_t Offset){
        m_CalibrationOffset = Offset;
    }//calibrationOffset
}//name-space

