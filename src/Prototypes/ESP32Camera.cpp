#include "ESP32Camera.h"

#ifdef ESP32

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM  -1
#define XCLK_GPIO_NUM   0
#define SIOD_GPIO_NUM   26
#define SIOC_GPIO_NUM   27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

#ifdef DEBUG
#define ON_DEBUG(Statement) Statement
#else
#define ON_DEBUG(Statement)
#endif

namespace ArduForge{
    ESP32Camera::ESP32Camera(void){

    }//Constructor

    ESP32Camera::~ESP32Camera(void){

    }//Destructor

    bool ESP32Camera::begin(framesize_t Framesize, pixformat_t PixelFormat, uint8_t JpegQuality){
        ON_DEBUG(Serial.print("Initializing ESP32 Camera..."));
        
        camera_config_t Config;
        Config.ledc_channel = LEDC_CHANNEL_0;
        Config.ledc_timer = LEDC_TIMER_0;
        Config.pin_d0 = Y2_GPIO_NUM;
        Config.pin_d1 = Y3_GPIO_NUM;
        Config.pin_d2 = Y4_GPIO_NUM;
        Config.pin_d3 = Y5_GPIO_NUM;
        Config.pin_d4 = Y6_GPIO_NUM;
        Config.pin_d5 = Y7_GPIO_NUM;
        Config.pin_d6 = Y8_GPIO_NUM;
        Config.pin_d7 = Y9_GPIO_NUM;
        Config.pin_xclk = XCLK_GPIO_NUM;
        Config.pin_pclk = PCLK_GPIO_NUM;
        Config.pin_vsync = VSYNC_GPIO_NUM;
        Config.pin_href = HREF_GPIO_NUM;
        Config.pin_sscb_sda = SIOD_GPIO_NUM;
        Config.pin_sscb_scl = SIOC_GPIO_NUM;
        Config.pin_pwdn = PWDN_GPIO_NUM;
        Config.pin_reset = RESET_GPIO_NUM;
        Config.xclk_freq_hz = (PixelFormat == PIXFORMAT_JPEG) ? 10000000 : 20000000;
        Config.pixel_format = PixelFormat;

        Config.frame_size = Framesize;
        Config.jpeg_quality = JpegQuality;
        Config.fb_count = (PixelFormat == PIXFORMAT_JPEG) ? 2 : 1;


        bool Rval = true;
        if(ESP_OK != esp_camera_init(&Config)){
            ON_DEBUG(Serial.print("failed\n"));
            Rval = false;
        }else{
            ON_DEBUG(Serial.print("done\n"));
        }

        return Rval;
    }//begin

    void ESP32Camera::end(void){
        ON_DEBUG(Serial.print("Closing ESP32 Camera..."));
        if(ESP_OK != esp_camera_deinit()){
            ON_DEBUG(Serial.print("failed...\n"));
        }else{
            ON_DEBUG(Serial.print("done\n"));
        }
        
    }//end

}//name-space

#endif //ESP32