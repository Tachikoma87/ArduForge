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
#include <sys/time.h>
/**
 * This example shows how to use the RGBLed class to create some lighting effects with a common cathode RGB Led.
 * Use the wiring provided with the docs and the sketch will work without changes. 
 */

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <esp_pm.h>
#include "Prototypes/OpticalLightBarrierESP32.h"
ArduForge::OpticalLightBarrierESP32 Lightbarrier;
BluetoothSerial BT;

int64_t timestamp(){
    struct timeval tv_now;
    gettimeofday(&tv_now, nullptr);
    int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
    int64_t time_ms = time_us/1000;
    return time_ms;
}

int64_t LastTrigger = 0;

void barrierCallback(ArduForge::OpticalLightBarrierESP32::BarrierMsg Msg, void *pUserParam){
    switch(Msg){
        case ArduForge::OpticalLightBarrierESP32::BMSG_CALIBRATION_FINISHED:{
            Serial.print("Calibration finished with noise value: "); 
            Serial.print(Lightbarrier.noise(0));
            Serial.print("\n");
            //Lightbarrier.startDetection();
            LastTrigger = timestamp();
        }break;
        case ArduForge::OpticalLightBarrierESP32::BMSG_BARRIER_TRIGGERED:{
            for(uint8_t i=0; i < Lightbarrier.measureLineCount(); ++i){
                if(!Lightbarrier.isLineTriggered(i)) return;
                else{
                    Serial.print("Line ");
                    Serial.print(i);
                    Serial.print(" triggered\n");
                }
            }
            Serial.print("All measure lines triggered! Stropping detection!\n");
            Lightbarrier.stopDetection();
            LastTrigger = timestamp();
            if(BT.hasClient()){
                BT.print("Barrier triggered!\n");
            }

            //Serial.print("Stopping detection for 5 seconds...\n");
            //Lightbarrier.startDetection();
            //Serial.print("Resuming duty ... \n");
        }break;
        case ArduForge::OpticalLightBarrierESP32::BMSG_IDLING:{ 
            //Serial.print("Sleeping\n");
            //esp_sleep_enable_timer_wakeup(15*1000);
            //esp_light_sleep_start();
            delay(50);
        }break;
        default: break;
    }//Msg
}

void setup() {
    Serial.begin(115200);
    //BT.begin("Barrier-Test-Device");
   
    /*Serial.print("PSRam: ");
    Serial.print(ESP.getFreePsram());
    Serial.print(" bytes\n");
*/

    const bool Grayscale = false;
    const bool LinearError = false;
    const bool Filter = false;
    Lightbarrier.begin(barrierCallback, nullptr, 15.0, 0, Grayscale, LinearError, Filter);
    Lightbarrier.setCalibrationParams(false, 1500);

    //Lightbarrier.clearMeasureLines();
    //Lightbarrier.addMeasureLine(0, 50);
    //Lightbarrier.addMeasureLine(250, 300);
}//setup



int64_t LastFPSPrint = 0;

void loop() {

    if(timestamp() - LastFPSPrint > 5000){
        Serial.print("FPS: ");
        Serial.print(Lightbarrier.fps());
        Serial.print("\n");
        LastFPSPrint = timestamp();

        if(BT.hasClient()){
            BT.print(Lightbarrier.fps());
            BT.print("\n");
        }
    }

    
    if(Lightbarrier.state() == ArduForge::OpticalLightBarrierESP32::STATE_NONE && timestamp() - LastTrigger > 5000){
        Serial.print("Lightbarrier has nothing to do ...\n");
        Lightbarrier.startDetection();
        Serial.print("Timestamp: "); Serial.print((int32_t)timestamp());
        Serial.print("\n");
    }

    delay(500);

}//loop