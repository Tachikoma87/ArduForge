/*****************************************************************************\
*                                                                           *
* File(s):                                            *
*                                                                           *
* Content:   *
*                                            *
*                                                                           *
*                                                                           *
* Author(s): Tom Uhlmann                                                    *
*                                                                           *
*                                                                           *
* The file(s) mentioned above are provided as is under the terms of the     *
* MIT License without any warranty or guaranty to work properly.            *
* For additional license, copyright and contact/support issues see the      *
* supplied documentation.                                                   *
*                                                                           *
\****************************************************************************/
#include <Arduino.h>
#include <CC1101.h>

#include "pins_arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


/**
 * The example sketch shows how to use the ArduForge::HM10 class for the interaction with an HM-10 BTLE device. 
 * To test the code you can install the Serial Bluetooth Terminal app on your smart phone or tablet to establish a bluetooth connection to your HM-10 module.
 * \ToDo Overhaul this example and use Interrupt for signal reception (attach interrupt to Gdo0 pin)
 */

const bool Master = true;
const uint8_t MasterAddr = 0x10;
const uint8_t SlaveAddr = 0x20;

const uint8_t Gdo0Pin = 2;
const uint8_t Gdo2Pin = 0;
const uint8_t CsnPin = SS;
const uint8_t MosiPin = MOSI;
const uint8_t MisoPin = MISO;
const uint8_t SckPin = SCK;

uint32_t PingsSend = 0;
uint32_t PingsReceived = 0;

uint8_t Receiver;
uint8_t Sender;
int8_t RSSI;
bool WaitForSignal = false;
bool PongReceived = false;
uint32_t LastPing = 0;

ArduForge::CC1101 Transceiver;
Adafruit_SSD1306 Display(128, 64, &Wire, -1, -1);

uint32_t Millis = 0;
const uint8_t SDaPin = SDA;

struct Msg{
    uint8_t MsgData[4];
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Transceiver.begin(MasterAddr, Gdo0Pin, ArduForge::CC1101::F_433, Gdo2Pin, CsnPin, MosiPin, MisoPin, SckPin );
  if(Master) Transceiver.begin(MasterAddr, Gdo0Pin, ArduForge::CC1101::F_433, Gdo2Pin, CsnPin, MosiPin, MisoPin, SckPin);
  else Transceiver.begin(SlaveAddr, Gdo0Pin);

  Serial.print("Init transceiver"); Serial.print(" Version: "); Serial.print(Transceiver.version());
  Serial.println();
 
 Transceiver.outputPowerLevel(12);
 Transceiver.receiveMode();
 Serial.print("Transceiver ready\n");
  Serial.print("Initializing display... ");
  Wire.begin(SDA, SCL);

  if(!Display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.print("Display initialization failed\n");
  }
  Display.display();

}

void masterRoutine(void){
  if(Transceiver.dataAvailable() > 0){   
    Transceiver.receiveData((uint8_t*)&Millis, &Receiver, &Sender, &RSSI); 
    uint32_t Latency = millis() - Millis;
    Serial.print("Received Pong from "); 
    Serial.print(Sender);
    Serial.print(" with Latency of ");
    Serial.print(Latency); 
    Serial.print(" ms (RSSI: "); Serial.print(RSSI); Serial.print(")\n");
    Transceiver.receiveMode();
    PongReceived = true;
    PingsReceived++;

    // print to display
    Display.clearDisplay();
    Display.setTextColor(SSD1306_WHITE);
    Display.setCursor(0, 10);
    Display.print("Pong: "); Display.print(Latency); Display.print(" ms");
    Display.setCursor(0, 22);
    Display.print("RSSI: "); Display.print(RSSI); Display.print(" dBm");
    Display.setCursor(0, 34);
    Display.print("Ping statistic: "); 
    Display.setCursor(0, 46);
    Display.print(PingsReceived); Display.print("/"); Display.print(PingsSend);
    Display.display();

  }
  
  if(!WaitForSignal){
    // send ping with milliseconds
    Serial.print("Sending ping ... ");
    PingsSend++;
    Millis = millis();
    LastPing = Millis;
    Transceiver.sendData(SlaveAddr, (uint8_t*)&Millis, sizeof(uint32_t));
    WaitForSignal = true;   
    PongReceived = false;
    Transceiver.receiveMode();
  }

  // after 0.5 Seconds of the last ping we send out another one
  if( (millis() - LastPing) > 1500){
    if(!PongReceived) Serial.println("got nothing");
    WaitForSignal = false;
  }
}//masteRoutine

void slaveRoutine(void){
  // if we receiver a ping we just send the message back
  if(Transceiver.dataAvailable() > 0){  
    Serial.print("Sent back Ping\n");
    Serial.print("Data bytes: "); Serial.println(Transceiver.dataAvailable());
    Transceiver.receiveData((uint8_t*)&Millis, &Receiver, &Sender, &RSSI);
    Transceiver.sendData(MasterAddr, (uint8_t*)&Millis, sizeof(uint32_t));
    Transceiver.receiveMode();
  }
}//slaveRoutine

void loop() {
  // put your main code here, to run repeatedly:
  if(Master){
    masterRoutine();
    
    //delay(1);
  } 
  else{
    slaveRoutine();
    //delay(1);
  } 

  
}