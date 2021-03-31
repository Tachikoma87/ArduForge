#include "OpticalLightBarrierESP32.h"

#ifdef ESP32

#ifdef DEBUG
#define ON_DEBUG(Statement) Statement
#else
#define ON_DEBUG(Statement)
#endif

namespace ArduForge{

    OpticalLightBarrierESP32::BarrierLine::BarrierLine(void){
        LineSize = 0;
        Magnitude = 0;
        pBuffer = nullptr;
    }//Constructor

    OpticalLightBarrierESP32::BarrierLine::~BarrierLine(void){
        clear();
    }//Destructor

    void OpticalLightBarrierESP32::BarrierLine::init(uint8_t Magnitude, uint16_t LineSize){
        clear();
        this->Magnitude = Magnitude;
        this->LineSize = LineSize;

        pBuffer = new uint8_t[(Magnitude*2+1)*LineSize];
        memset(pBuffer, 0, sizeof(uint8_t)*(Magnitude*2 + 1) * LineSize);
    }//initialize

    void OpticalLightBarrierESP32::BarrierLine::clear(void){
        Magnitude = 0;
        LineSize = 0;
        delete[] pBuffer;
        pBuffer = nullptr;
    }//clear

    void OpticalLightBarrierESP32::BarrierLine::extractData(camera_fb_t *pFB){
        if(nullptr == pFB) return;

        uint32_t PixelCount = pFB->width * (2 * Magnitude + 1);
        uint16_t Index = (pFB->height/2 - Magnitude)*pFB->width;
        memcpy(&pBuffer[0], &pFB->buf[Index], sizeof(uint8_t)*PixelCount);

    }//retrieveData

    void OpticalLightBarrierESP32::BarrierLine::augmentCaptureLine(uint8_t LineMagnitude, uint32_t Width, uint32_t Height, uint8_t *pImgData){
        uint32_t PixelCount = Width * (2 * LineMagnitude + 1);
        uint32_t Index = (Height/2 - LineMagnitude)*Width;

        ON_DEBUG(Serial.printf("Augmenting line %d Pixels from %d Index\n", PixelCount, Index));

        for(uint32_t i=0; i < PixelCount; ++i) pImgData[Index + i] = 0;
    }//augmentCaptureLine


    float OpticalLightBarrierESP32::BarrierLine::computeDeviationLinear(BarrierLine *pLeft, BarrierLine *pRight){
        uint32_t TotalDeviation = 0;
        uint32_t PixelCount = (pLeft->Magnitude * 2 +1) * pLeft->LineSize;
        for(uint16_t i=0; i < PixelCount; ++i){
            TotalDeviation += abs(pLeft->pBuffer[i] - pRight->pBuffer[i]);
        }

        return (float)TotalDeviation / (float)PixelCount;
    }//computeDeviationLienar

    float OpticalLightBarrierESP32::BarrierLine::computeDeviationQuadratic(BarrierLine *pLeft, BarrierLine *pRight){
        uint32_t TotalDeviation = 0;
        uint32_t PixelCount = (pLeft->Magnitude * 2 +1) * pLeft->LineSize;
        for(uint16_t i=0; i < PixelCount; ++i){
            TotalDeviation += powf(pLeft->pBuffer[i] - pRight->pBuffer[i], 2.0f);
        }

        return (float)TotalDeviation / (float)PixelCount;
    }//computeDeviationQuadratic

    
    void OpticalLightBarrierESP32::cameraThread(void *pParam){
        if(nullptr == pParam){
            ON_DEBUG(Serial.print("Null pointer error at capture thread. Unable to start capture thread.\n"));
            return;
        }

        OpticalLightBarrierESP32 *pParent = (OpticalLightBarrierESP32*)pParam;

        ESP32Camera Camera;
        if(!Camera.begin(FRAMESIZE_CIF, PIXFORMAT_GRAYSCALE)){
            ON_DEBUG(Serial.print("Failed initializing ESP32 Camera!\n"));
            pParent->m_Callback(BMSG_INITIALIZATION_FAILED, pParent->m_pUserCBParam);
            return;
        }

        BarrierLine Line1;
        BarrierLine Line2;
        uint8_t LineToActualize = 0;

        // fps counter
        uint32_t FrameCount = 0;
        uint32_t LastFPSPrint = 0;

        Line1.init(pParent->m_LineMagnitude, 400);
        Line2.init(pParent->m_LineMagnitude, 400);

        camera_fb_t *pFB = nullptr;

        pParent->m_Callback(BMSG_INITIALIZATION_FINISHED, pParent->m_pUserCBParam);

        while(!pParent->m_StopThread){

            switch(pParent->m_ActiveState){
                case STATE_CALIBRATION:{
                    float Noise = 0.0f;
                    uint16_t FrameCount = 0;
                    uint32_t CalibrationStart = millis();

                    pFB = esp_camera_fb_get();
                    Line1.extractData(pFB);
                    esp_camera_fb_return(pFB);;
                    LineToActualize = 1;

                    while(millis() - CalibrationStart < pParent->m_CalibrationDuration){
                        pFB = esp_camera_fb_get();
                        (LineToActualize == 0) ? Line1.extractData(pFB) : Line2.extractData(pFB);
                        LineToActualize = (LineToActualize + 1) % 2;
                        esp_camera_fb_return(pFB);

                        // we will ignore frames within the first 500 ms to give the sensor some time to calibrate itself
                        if(millis() - CalibrationStart > 500){
                            float Temp = BarrierLine::computeDeviationQuadratic(&Line1, &Line2);
                            if(Temp > Noise) Noise = Temp;
                            FrameCount++;
                        }               
                    }//while[calibration time]
                    
                    pParent->m_Noise = Noise;
                    //calibration ready, enter idle state
                    pParent->m_ActiveState = STATE_NONE;
                    pParent->m_Callback(BMSG_CALIBRATION_FINISHED, pParent->m_pUserCBParam);

                    ON_DEBUG(Serial.print("Calibration finished. Noise is "););
                    ON_DEBUG(Serial.print(pParent->m_Noise));
                    ON_DEBUG(Serial.print("\n"));

                }break;
                case STATE_START_DETECION:{
                    // set both lines to current capture values
                    pFB = esp_camera_fb_get();
                    Line1.extractData(pFB);
                    Line2.extractData(pFB);
                    esp_camera_fb_return(pFB);
                    // enter detection state
                    pParent->m_ActiveState = STATE_DETECTION;

                    sensor_t *pSensor = esp_camera_sensor_get();
                    pSensor->set_whitebal(pSensor, 0);
                    pSensor->set_awb_gain(pSensor, 0);
                    pSensor->set_aec2(pSensor, 0);
                    pSensor->set_exposure_ctrl(pSensor, 0);
                    pSensor->set_gain_ctrl(pSensor, 0);

                }break;
                case STATE_STOP_DETECTION:{
                    sensor_t *pSensor = esp_camera_sensor_get();
                    pSensor->set_whitebal(pSensor, 1);
                    pSensor->set_awb_gain(pSensor, 1);
                    pSensor->set_aec2(pSensor, 1);
                    pSensor->set_exposure_ctrl(pSensor, 1);
                    pSensor->set_gain_ctrl(pSensor, 1);
                    pParent->m_ActiveState = STATE_NONE;
                }break;
                case STATE_DETECTION:{
                    // retrieve frame and update one line
                    pFB = esp_camera_fb_get();
                    (LineToActualize == 0) ? Line1.extractData(pFB) : Line2.extractData(pFB);
                    esp_camera_fb_return(pFB);
                    LineToActualize = (LineToActualize + 1) % 2;
                    
                    float Deviation = BarrierLine::computeDeviationQuadratic(&Line1, &Line2);
                    //float Deviation = 0.0f;
                    if(Deviation > pParent->m_Noise + 10.0f * pParent->m_Sensitivity){
                        pParent->m_Callback(BMSG_BARRIER_TRIGGERED, pParent->m_pUserCBParam);
                        ON_DEBUG(Serial.print("Barrier triggered with value: "));
                        ON_DEBUG(Serial.print(Deviation));
                        ON_DEBUG(Serial.print("\n"));
                    }

                    FrameCount++;
                    if(millis() - LastFPSPrint > 1000){
                        pParent->m_FPS = FrameCount;
                        LastFPSPrint = millis();
                        FrameCount = 0;
                    }

                }break;
                default:{
                    // idle if nothing else to do
                    delay(10);
                }break;
            }
        }//while[thread running]

        // free camera
        Camera.end();

        // delete this task
        vTaskDelete(nullptr);
    }//captureThread

    OpticalLightBarrierESP32::OpticalLightBarrierESP32(void){
        m_Callback = nullptr; 

        m_hCameraThread = nullptr;   
        m_StopThread = false;              

        m_Sensitivity = 1.4f;           
        m_Noise = 0.0f;                  
        m_CalibrationDuration = 1500; 
        m_ActiveState = STATE_NONE;     
        m_FPS = 0;                
    }//Constructor

    OpticalLightBarrierESP32::~OpticalLightBarrierESP32(void){
        end();
    }//Destructor

    void OpticalLightBarrierESP32::begin(BarrierCallbackFunc BarrierCB, void *pUserCBParam, float Sensitivity, uint8_t CameraThreadCoreID){
        m_Callback = BarrierCB;
        m_Sensitivity = Sensitivity;
        m_ActiveState = STATE_NONE;
        m_pUserCBParam = pUserCBParam;

        m_StopThread = false;
        m_LineMagnitude = 5;

        // start camera capture thread
        xTaskCreatePinnedToCore(cameraThread, "Camera_Thread", 10000, this, 1, &m_hCameraThread, CameraThreadCoreID);
        
    }//begin

    uint32_t OpticalLightBarrierESP32::imageSize(void)const{
        camera_fb_t *pFB = esp_camera_fb_get();
        uint32_t Rval = 0;
        if(nullptr != pFB) Rval = pFB->len;
        esp_camera_fb_return(pFB);
        return Rval;
    }//imageSize

    void OpticalLightBarrierESP32::captureImage(uint32_t *pWidth, uint32_t *pHeight, uint8_t *pImgData, bool AugmentCaptureLine){
        camera_fb_t *pFB = esp_camera_fb_get();
        if(nullptr != pFB){
            (*pWidth) = pFB->width;
            (*pHeight) = pFB->height;
            memcpy(pImgData, pFB->buf, pFB->len);
            //for(int i=0; i < pFB->len; ++i) pImgData[i] = pFB->buf[i];
            if(AugmentCaptureLine) BarrierLine::augmentCaptureLine(m_LineMagnitude, pFB->width, pFB->height, pImgData);
        }

        esp_camera_fb_return(pFB);
    }//captureImage

    void OpticalLightBarrierESP32::end(void){
        m_StopThread = true;
        m_hCameraThread = nullptr;
    }//end

    void OpticalLightBarrierESP32::calibrate(uint16_t Duration){
        m_CalibrationDuration = Duration;
        m_ActiveState = STATE_CALIBRATION;
    }

    void OpticalLightBarrierESP32::startDetection(void){
        m_ActiveState = STATE_START_DETECION;
    }//startDetection

    void OpticalLightBarrierESP32::stopDetection(void){
        m_ActiveState = STATE_STOP_DETECTION;
    }//stopDetection

    float OpticalLightBarrierESP32::sensitivity(void)const{
        return m_Sensitivity;
    }//sensitivity

    void OpticalLightBarrierESP32::sensitivity(float Sensitivity){
        m_Sensitivity = Sensitivity;
    }//sensitivity

    uint8_t OpticalLightBarrierESP32::fps(void)const{
        return m_FPS;
    }//fps

    OpticalLightBarrierESP32::ActiveState OpticalLightBarrierESP32::state(void)const{
        return m_ActiveState;
    }//ActiveState

    float OpticalLightBarrierESP32::noise(void)const{
        return m_Noise;
    }//noise

}//name-space


#endif