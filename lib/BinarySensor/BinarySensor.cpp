#include <Arduino.h>
#include "BinarySensor.h"

namespace ArduForge{

    BinarySensor::BinarySensor(void){
        m_SensorPin = -1;
    }//Constructor 

    BinarySensor::~BinarySensor(void){
        end();
    }//Destructor
    
    void BinarySensor::begin(int8_t Pin, bool Pullup, int8_t StableReadingsThreshold){
        m_StableReadingThreshold = StableReadingsThreshold;
        m_SensorPin = Pin;
        (Pullup) ? pinMode(m_SensorPin, INPUT_PULLUP) : pinMode(m_SensorPin, INPUT);
    }//begin 

    void BinarySensor::end(void){
        m_SensorPin = -1;
    }//end

    int8_t BinarySensor::read(const int8_t Delay){
        int8_t Rval = digitalRead(m_SensorPin);
        if(Delay < 0) return Rval;
        int8_t MaxRepetitions = m_StableReadingThreshold * 3;
        int8_t StableReadings = 0;

        while(StableReadings < m_StableReadingThreshold && MaxRepetitions > 0){
            if(digitalRead(m_SensorPin) == Rval){
                StableReadings++;
            }else{
                Rval = digitalRead(m_SensorPin);
                StableReadings = 0;
            }
            MaxRepetitions--;
            delay(Delay);
        }//while[debounce]

        return (StableReadings < m_StableReadingThreshold) ? -1 : Rval;
    }//read

}//name space