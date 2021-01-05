#include <arduino.h>
#include "Melody.h"

namespace ArduForge{
    Melody::Melody(void){
        m_SpeakerPin = -1;
    }//Constructor

    Melody::~Melody(void){

    }//Destructor

    void Melody::begin(int8_t SpeakerPin){
        m_SpeakerPin = SpeakerPin;
    }//begin

    void Melody::end(void){
        m_SpeakerPin = -1;
    }//end

    void Melody::play(Note *pNotes, int8_t *pDurations, int32_t NoteCount){

        for(int i=0; i< NoteCount; ++i){
            int16_t Duration = 1000/pDurations[i];
            tone(m_SpeakerPin, (int16_t)pNotes[i], Duration);
            delay(Duration + 30);
            noTone(8);
        }//for[notes]

    }//play


}//name space