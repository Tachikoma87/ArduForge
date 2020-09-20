#include <Arduino.h>
#include "RotaryEncoder.h"

namespace ArduForge{
    RotaryEncoder::RotaryEncoder(void){
        m_Clk = -1;
        m_Dt = -1;
        m_Button = -1;
        m_LastState = 0;

        m_RotaryPosition = 0;
        m_Dir = -1; // 0 for CW and 1 for CCW
        m_ButtonState = false;
    }//Constructor

    RotaryEncoder::~RotaryEncoder(void){

    }//Destructor

    void RotaryEncoder::begin(int8_t ClkPin, int8_t DtPin, int8_t ButtonPin){
        m_Clk = ClkPin;
        m_Dt = DtPin,
        m_Button = ButtonPin;

        pinMode(m_Clk, INPUT);
        pinMode(m_Dt, INPUT);
        pinMode(m_Button, INPUT_PULLUP); // pullup for switch

        m_RotaryPosition = 0;
        m_LastState = digitalRead(m_Clk);

    }//begin 

    void RotaryEncoder::end(void){
        m_Clk = -1;
        m_Dt = -1;
        m_Button = -1;
        m_LastState = 0;

        m_RotaryPosition = 0;
        m_Dir = -1; // 0 for CW and 1 for CCW
        m_ButtonState = false;
    }//end

    void RotaryEncoder::update(void){
        m_ButtonState = (digitalRead(m_Button) == LOW) ? true : false;

        int8_t State = digitalRead(m_Clk);
        if(m_LastState != State){
            const int8_t Dt = digitalRead(m_Dt);

            // determine direction of turn
            if(Dt == State){
                // rotary encoder turned CCW
                m_Dir = DIR_CCW; 
                if(m_LastState == LOW && Dt == HIGH) m_RotaryPosition++;
            } 
            else{
                // rotary encoder turned CW
                m_Dir = DIR_CW;
                if(m_LastState == HIGH && Dt == HIGH) m_RotaryPosition--;
            }
        }   
         m_LastState = State;   
    }//update

    bool RotaryEncoder::buttonState(void)const{
        return m_ButtonState;
    }//buttonState 

    int8_t RotaryEncoder::direction(void)const{
        return m_Dir;
    }//direction 

    int32_t RotaryEncoder::position(void)const{
        return m_RotaryPosition;
    }//position 

    int32_t RotaryEncoder::resetPosition(void){
        m_RotaryPosition = 0;
    }//resetPosition

}//name space