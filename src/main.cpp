#include <Arduino.h>
#include "Prototypes/CC1101.h"

ArduForge::CC1101 Transceiver;

const bool Master = true;
const uint8_t SlaveAddr = 0x09;
const uint8_t MasterAddr = 0x07;

uint16_t Number = 0;
uint8_t Msg = 0;

struct MsgStruct{
  char Text[16];

};

void setup() {
  // put your setup codes here, to run once
  Serial.begin(115200);

  if(Master){
    Transceiver.begin(MasterAddr, 11, 12, 10, 13, 2, 255, 64);
    Serial.print("Master ready on Channel "); Serial.print(Transceiver.channel()); Serial.println();
    Transceiver.outputPowerLevel(0);
  }else{
    Transceiver.begin(SlaveAddr, 11, 12, 10, 13, 2, 255, 64);
    Transceiver.receiveMode();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  MsgStruct M;
  if(Master){
    sprintf(M.Text, "Number: %d", Number);
    Transceiver.sendData(SlaveAddr, (uint8_t*)M.Text, sizeof(MsgStruct::Text), false);
    Serial.print("Send: "); Serial.print(Number); Serial.println();
    Number++;
    delay(2500);
  }else{
    if(Transceiver.dataAvailable() > 0){
      uint8_t From, To;
      int8_t RSSI;
      uint8_t ByteCount = Transceiver.receiveData((uint8_t*)M.Text, &To, &From, &RSSI);
      Transceiver.receiveMode();

      Serial.print("Received Msg ("); Serial.print(ByteCount); Serial.print(" bytes): ");  Serial.print(M.Text); 
      Serial.print(" | From "); Serial.print(From); Serial.print(" to "); Serial.print(To);
      Serial.print(" with RSSI "); Serial.print(RSSI); Serial.print(" db");
      Serial.println();
      delay(1);
    }
  }

}