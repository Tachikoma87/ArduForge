#include "BatteryIndicator.h"

namespace ArduForge{

    BatteryIndicator::BatteryIndicator(void){
        m_RefVoltage = 0.0f;
        m_R1 = 0;
        m_R2 = 0;
        m_BatteryCount = 0;
        m_BatteryPin = 0xFF;
    }//Constructor

    BatteryIndicator::~BatteryIndicator(void){
        end();
    }//Destructor


    void BatteryIndicator::begin(uint8_t BatteryPin, uint8_t BatteryCount, uint32_t R1, uint32_t R2, BatteryType Type, float ReferenceVoltage){
        m_BatteryPin = BatteryPin;
        m_BatteryCount = BatteryCount;
        m_R1 = (float)R1;
        m_R2 = (float)R2;
        m_RefVoltage = ReferenceVoltage;
        m_Type = Type;

        switch(m_Type){
            case TYPE_NIMH:{
                m_VoltageMin = 1.05f;
                m_VoltageMax = 1.30f;
            }break;
            case TYPE_LIPO:{
                m_VoltageMin = 3.1f;
                m_VoltageMax = 4.1f;
            }break;
            default: break;
        }//switch[type]

        #ifdef ESP32
        analogResolution(12);
        #else
        analogResolution(10);
        #endif

    }//begin

    void BatteryIndicator::analogResolution(uint8_t BitCount){
        m_Resolution = (float)(1 << BitCount);
        m_Resolution -= 1.0f;
    }//analogResolution

    void BatteryIndicator::end(void){

    }//end

    float BatteryIndicator::read(uint8_t SampleCount){
        if(SampleCount < 1) return 0.0f;

        uint32_t Raw = analogRead(m_BatteryPin);
        for(uint8_t i=1; i < SampleCount; ++i){
            delay(1);
            Raw += analogRead(m_BatteryPin);
        } 
        Raw /= SampleCount;

        // read voltage at voltage divider
        float Reading = m_RefVoltage * Raw/m_Resolution;
        // calculate original voltage V from Reading U1: U1 = (V*R2) / (R1 + R2) <=> V = (U1 * (R1+R2)) / R2
        if(m_R1 < 0.001f || m_R2 < 0.001f) return Reading;
        return Reading * (m_R1+m_R2) / m_R2;
    }//read

    uint8_t BatteryIndicator::charge(uint8_t SampleCount){
        const float Voltage = read(SampleCount) / (float)m_BatteryCount;
        if(Voltage < m_VoltageMin) return 0;
        // Mapping from [VoltageMin , VoltageMax] -> [0,100]
        uint8_t Charge = (uint8_t) ((Voltage - m_VoltageMin)/(m_VoltageMax - m_VoltageMin) * 100.0f);
        if(Charge < 0) Charge = 0;
        else if(Charge > 100) Charge = 100;
        return Charge;
    }//charge

    BatteryIndicator::BatteryType BatteryIndicator::type(void)const{
        return m_Type;
    }//type


}//name-space