/*****************************************************************************\
*                                                                           *
* File(s): Ex01_LedControl.ino                                              *
*                                                                           *
* Content: This example shows how to use the RGBLed class to create some    *
*          lighting effects with a common cathode RGB Led.                  *
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
#include <inttypes.h>
/**
 * This example shows how to use the RGBLed class to create some lighting effects with a common cathode RGB Led.
 * Use the wiring provided with the docs and the sketch will work without changes. 
 */

#include <Arduino.h>
#include "Prototypes/OpticalLightBarrierESP32.h"
ArduForge::OpticalLightBarrierESP32 Lightbarrier;



void barrierCallback(ArduForge::OpticalLightBarrierESP32::BarrierMsg Msg, void *pUserParam){
    switch(Msg){
        case ArduForge::OpticalLightBarrierESP32::BMSG_CALIBRATION_FINISHED:{
            Serial.print("Calibration finished with noise value: "); 
            Serial.print(Lightbarrier.noise());
            Serial.print("\n");
            Lightbarrier.startDetection();
        }break;
        case ArduForge::OpticalLightBarrierESP32::BMSG_BARRIER_TRIGGERED:{
            //Serial.print("Barrier triggered\n");
        }break;
        default: break;
    }//Msg
}

void setup() {
    Serial.begin(115200);
   
   Serial.print("PSRam: ");
    Serial.print(ESP.getFreePsram());
    Serial.print(" bytes\n");

    Lightbarrier.begin(barrierCallback, nullptr, 1.3f, 0);
    Lightbarrier.calibrate(); 
}//setup


void loop() {

    Serial.print("FPS: ");
    Serial.print(Lightbarrier.fps());
    Serial.print("\n");

    delay(5000);

}//loop