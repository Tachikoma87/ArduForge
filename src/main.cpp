/*****************************************************************************\
*                                                                           *
* File(s):                                               *
*                                                                           *
* Content:      *
*                         *
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



/**
 * This example sketch shows how to use the rotary encoder class using an interrupt attached to the clock signal.
 */



void setup() {
  // put your setup codes here, to run once
  Serial.begin(115200);


}

void loop() {
 
  // In this example we can use delay since the interrupt will ensure that we don't miss a signal from the encoder.
  delay(1000);
}//loop