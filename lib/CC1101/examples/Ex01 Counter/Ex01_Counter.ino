/*****************************************************************************\
*                                                                           *
* File(s): Ex01_Counter.ino                                                 *
*                                                                           *
* Content: Example sketch that shows how to use the CC1101 class to         *
*          transmit a counter number which is converted to a string first.  *
*                                                                           *
*                                                                           *
* Author(s): Tom Uhlmann                                                    *
*                                                                           *
*                                                                           *
* The file(s) mentioned above are provided as is under the terms of the     *
* FreeBSD License without any warranty or guaranty to work properly.        *
* For additional license, copyright and contact/support issues see the      *
* supplied documentation.                                                   *
*                                                                           *
\****************************************************************************/
#include <Arduino.h>
#include <CC1101.h>

ArduForge::CC1101 Transceiver;
/**
 * The master device will count a single 16 bit unsigned integer (uint16_t) from 0 upwards, converts the value to a string, and then transmits the string to the slave. The slave device * will receive the string and print it to Serial along with some connection information.
 * You will need two devices. To test the code compile and upload it for the master and then for the slave device. 
 */

const bool Master = true; // change this to compile either the master or the slave sketch

const uint8_t MasterAddr = 0x01; // master device address
const uint8_t SlaveAddr = 0x07; // slave device address
const uint8_t Channel = 10; // channel to use

uint16_t Number = 0; // Number to count

// We use this structure to send and receive data
struct MsgStruct{
  char Text[16];
};

void setup() {
  // put your setup codes here, to run once
  Serial.begin(115200);

  if(Master){
    Transceiver.begin(MasterAddr, 2);
    Transceiver.channel(Channel);
    Serial.print("Master ready on Channel "); Serial.print(Transceiver.channel()); Serial.print(" with address "); Serial.print(Transceiver.deviceAddress()); Serial.println();
    Transceiver.outputPowerLevel(0); // you can amplify the signal if you have a bad connection, or reduce it to save some energy
  }else{
    Transceiver.begin(SlaveAddr, 2);
    Transceiver.channel(Channel);
    Serial.print("Slave ready on Channel "); Serial.print(Transceiver.channel()); Serial.print(" with address "); Serial.print(Transceiver.deviceAddress()); Serial.println();
    Transceiver.receiveMode();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  MsgStruct M;
  if(Master){
    // convert number to text and send to slave
    sprintf(M.Text, "Number: %d", Number);
    Transceiver.sendData(SlaveAddr, (uint8_t*)M.Text, sizeof(MsgStruct::Text));
    Serial.print("Send: "); Serial.print(Number); Serial.println();
    Number++;
    delay(500); // wait half a second before transmitting the next number
  }else{
    // if we have received data, we can read it from the device
    if(Transceiver.dataAvailable() > 0){
      uint8_t From, To; // Sender and Receiver addresses
      int8_t RSSI; // received signal strength indicator (RSSI) in dBm
      uint8_t PayloadSize = Transceiver.receiveData((uint8_t*)M.Text, &To, &From, &RSSI);
      // put the device in receive mode again after reading the data
      Transceiver.receiveMode();

      Serial.print("Received Msg ("); Serial.print(PayloadSize); Serial.print(" bytes): ");  Serial.print(M.Text); 
      Serial.print(" | From "); Serial.print(From); Serial.print(" to "); Serial.print(To);
      Serial.print(" with RSSI "); Serial.print(RSSI); Serial.print(" dBm");
      Serial.println();
      delay(1);
    }
  }

}//loop