#include "BatteryIndicatorNiMh.h"

namespace ArduForge{
    BatteryIndicatorNiMh::BatteryIndicatorNiMh(void){

    }//Constructor

    BatteryIndicatorNiMh::~BatteryIndicatorNiMh(void){

    }//Destructor

    void BatteryIndicatorNiMh::begin(uint8_t BatteryPin, uint8_t BatteryCount, float ReferenceVoltage){
        m_BatteryPin = BatteryPin;
        m_RefVoltage = ReferenceVoltage;
        m_BatteryCount = BatteryCount;
    }//begin

    void BatteryIndicatorNiMh::end(void){
        m_BatteryPin = 0xFF;
        m_RefVoltage = 0.0f;
    }//end

    float BatteryIndicatorNiMh::read(void){
        const float Reading = (float)analogRead(m_BatteryPin);
        return (Reading/1023.0f) * m_RefVoltage;
    }//read

    uint8_t BatteryIndicatorNiMh::charge(void){
        const float Voltage = read();
        // map from Number of Batteries n * [1.05, 1.3] => [0, 1] <==> Charge = (Voltage - 1.05*n)/(0.25f*n);
        // equation below is equivalent to approach above
        const float Charge = 4.0 * Voltage / m_BatteryCount - 4.2f;
        if(Charge < 0.0) return 0;
        else if(Charge > 1.0) return 100;
        return (uint8_t)(Charge * 100.0);
    }//charge

}//name space