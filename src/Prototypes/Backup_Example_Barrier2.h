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
            Serial.print(Lightbarrier.noise());
            Serial.print("\n");
            //Lightbarrier.startDetection();
            LastTrigger = timestamp();
        }break;
        case ArduForge::OpticalLightBarrierESP32::BMSG_BARRIER_TRIGGERED:{
            Serial.print("Barrier triggered\n");
            Lightbarrier.stopDetection();
            LastTrigger = timestamp();
            /*if(BT.hasClient()){
                BT.print("Barrier triggered!\n");
            }*/

            //Serial.print("Stopping detection for 5 seconds...\n");
            //Lightbarrier.startDetection();
            //Serial.print("Resuming duty ... \n");
        }break;
        case ArduForge::OpticalLightBarrierESP32::BMSG_IDLING:{
            //vTaskDelay(50/portTICK_PERIOD_MS);    
        }break;
        default: break;
    }//Msg
}

void setup() {
    Serial.begin(115200);
    //BT.begin("Barrier-Test-Device");
   
    Serial.print("PSRam: ");
    Serial.print(ESP.getFreePsram());
    Serial.print(" bytes\n");

    Lightbarrier.begin(barrierCallback, nullptr, 25.0f, 0);
    Lightbarrier.calibrate(); 

   /* esp_pm_config_esp32_t pm_Config;
    pm_Config.light_sleep_enable = false;
    pm_Config.max_freq_mhz = 240;
    pm_Config.min_freq_mhz = 80;
    esp_err_t Rval = esp_pm_configure(&pm_Config);
    if(Rval == ESP_OK){
        Serial.print("PM successfully set\n");  
    }else if(Rval == ESP_ERR_INVALID_ARG){
        Serial.print("Invalid arguments for pm\n");
    }else if(Rval == ESP_ERR_NOT_SUPPORTED){
        Serial.print("Power mode not supported\n");
    }*/
}//setup



int64_t LastFPSPrint = 0;

void loop() {

    if(timestamp() - LastFPSPrint > 5000){
        Serial.print("FPS: ");
        Serial.print(Lightbarrier.fps());
        Serial.print("\n");
        LastFPSPrint = timestamp();

       /* if(BT.hasClient()){
            BT.print(Lightbarrier.fps());
            BT.print("\n");
        }*/
    }

    
    if(Lightbarrier.state() == ArduForge::OpticalLightBarrierESP32::STATE_NONE && timestamp() - LastTrigger > 4000){
        Serial.print("Lightbarrier has nothing to do ...\n");
        Lightbarrier.startDetection();
        Serial.print("Timestamp: "); Serial.print((int32_t)timestamp());
        Serial.print("\n");
    }

    delay(500);

}//loop