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
    }//computeDeviationLinear

    float OpticalLightBarrierESP32::BarrierLine::computeDeviationQuadratic(BarrierLine *pLeft, BarrierLine *pRight){
        uint32_t TotalDeviation = 0;
        uint32_t PixelCount = (pLeft->Magnitude * 2 +1) * pLeft->LineSize;
        for(uint16_t i=0; i < PixelCount; ++i){
            TotalDeviation += powf(pLeft->pBuffer[i] - pRight->pBuffer[i], 2.0f);
        }

        return (float)TotalDeviation / (float)PixelCount;
    }//computeDeviationQuadratic

    void OpticalLightBarrierESP32::performCalibration(BarrierLine *pLine1, BarrierLine *pLine2){
        m_ActiveState = STATE_CALIBRATION;
        m_Noise = 0.0f;
        uint8_t FrameCount = 0;
        camera_fb_t *pFrame = nullptr;

        OpticalLightBarrierESP32::cameraAutomatics(true);
        // we ignore the first 10 images (about half a second) to let the camera calibrate itself
        while(FrameCount < 10){
            pFrame = esp_camera_fb_get();
            if(nullptr != pFrame) FrameCount++;
            esp_camera_fb_return(pFrame);
        }
        OpticalLightBarrierESP32::cameraAutomatics(false);

        uint32_t CalibrationStart = millis();
        FrameCount = 0;
 
        pFrame = esp_camera_fb_get();
        pLine1->extractData(pFrame);
        esp_camera_fb_return(pFrame);
        uint8_t LineToActualize = 1;

        while(millis() - CalibrationStart < m_CalibrationDuration){
            pFrame = esp_camera_fb_get();
            if(nullptr == pFrame) continue;
            (LineToActualize == 0) ? pLine1->extractData(pFrame) : pLine2->extractData(pFrame); 
            esp_camera_fb_return(pFrame);
            LineToActualize = (LineToActualize + 1) % 2;

            float Temp = BarrierLine::computeDeviationQuadratic(pLine1, pLine2);
            if(Temp > m_Noise) m_Noise = Temp;                  
        }//while[calibration time]

        m_Callback(BMSG_CALIBRATION_FINISHED, m_pUserCBParam);

    }//performCalibration

    void OpticalLightBarrierESP32::cameraAutomatics(bool Enable){
        sensor_t *pSensor = esp_camera_sensor_get();
        int Val = (Enable) ? 1 : 0;
        pSensor->set_whitebal(pSensor, Val);
        pSensor->set_awb_gain(pSensor, Val);
        pSensor->set_aec2(pSensor, Val);
        pSensor->set_exposure_ctrl(pSensor, Val);
        pSensor->set_gain_ctrl(pSensor, Val);

    }//cameraAutomatics
    
    
    void OpticalLightBarrierESP32::cameraThread(void *pParam){
        if(nullptr == pParam){
            ON_DEBUG(Serial.print("Null pointer error at capture thread. Unable to start capture thread.\n"));
            return;
        }

        OpticalLightBarrierESP32 *pParent = (OpticalLightBarrierESP32*)pParam;

        ESP32Camera Camera;
        BarrierLine Line1;
        BarrierLine Line2;
        uint8_t LineToActualize = 0;

        // fps counter
        uint32_t FrameCount = 0;
        uint32_t LastFPSPrint = 0;

        Line1.init(pParent->m_LineMagnitude, 400);
        Line2.init(pParent->m_LineMagnitude, 400);

        camera_fb_t *pFB = nullptr;

        if(!Camera.begin(FRAMESIZE_CIF, PIXFORMAT_GRAYSCALE)){
            ON_DEBUG(Serial.print("Failed initializing ESP32 Camera!\n"));
            pParent->m_Callback(BMSG_INITIALIZATION_FAILED, pParent->m_pUserCBParam);
            return;
        }
        pParent->m_ActiveState = STATE_CALIBRATION;
        pParent->m_Callback(BMSG_INITIALIZATION_FINISHED, pParent->m_pUserCBParam);

        while(!pParent->m_StopThread){
            switch(pParent->m_ActiveState){   
                case STATE_CALIBRATION:{
                    pParent->performCalibration(&Line1, &Line2);
                    pParent->m_ActiveState = STATE_NONE;
                }break;
                case STATE_START_DETECION:{
                    // set both lines to current capture values
                    if(pParent->m_AutoCalibration) pParent->performCalibration(&Line1, &Line2);

                    pFB = esp_camera_fb_get();
                    Line1.extractData(pFB);
                    Line2.extractData(pFB);
                    esp_camera_fb_return(pFB);
                    // enter detection state
                    pParent->m_ActiveState = STATE_DETECTION;
                    pParent->m_FPS = 0;

                    pParent->m_Callback(BMSG_DETECTION_STARTED, pParent->m_pUserCBParam);
                    
                }break;
                case STATE_STOP_DETECTION:{
                    OpticalLightBarrierESP32::cameraAutomatics(true);
                    pParent->m_ActiveState = STATE_NONE;
                    pParent->m_FPS = 0;
                    pParent->m_Callback(BMSG_DETECTION_STOPPED, pParent->m_pUserCBParam);
                }break;
                case STATE_DETECTION:{
                    // retrieve frame and update one line
                    pFB = esp_camera_fb_get();
                    if(nullptr == pFB){
                        ON_DEBUG(Serial.print("Failed to get frame\n"));
                        break;
                    }
                    (LineToActualize == 0) ? Line1.extractData(pFB) : Line2.extractData(pFB);
                    esp_camera_fb_return(pFB);
                    LineToActualize = (LineToActualize + 1) % 2;
                    
                    float Deviation = BarrierLine::computeDeviationQuadratic(&Line1, &Line2);
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
                    // nothing to do here
                    pParent->m_Callback(BMSG_IDLING, pParent->m_pUserCBParam);
                }break;
            }

            if(!pParent->m_StateQueue.empty()){
                pParent->m_ActiveState = pParent->m_StateQueue.front();
                pParent->m_StateQueue.pop();
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

        m_Sensitivity = 20.0f;           
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
        m_AutoCalibration = true;
        m_CalibrationDuration = 1000;
        m_FPS = 0;

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
            if(AugmentCaptureLine) BarrierLine::augmentCaptureLine(m_LineMagnitude, pFB->width, pFB->height, pImgData);
        }
        esp_camera_fb_return(pFB);
    }//captureImage

    void OpticalLightBarrierESP32::end(void){
        m_StopThread = true;
        m_hCameraThread = nullptr;
    }//end

    void OpticalLightBarrierESP32::calibrate(void){
        m_StateQueue.push(STATE_CALIBRATION);
    }

    void OpticalLightBarrierESP32::startDetection(void){
        m_StateQueue.push(STATE_START_DETECION);
    }//startDetection

    void OpticalLightBarrierESP32::stopDetection(void){
        m_StateQueue.push(STATE_STOP_DETECTION);
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

    void OpticalLightBarrierESP32::setCalibrationParams(bool AutoCalibration, uint16_t Duration){
        m_AutoCalibration = AutoCalibration;
        m_CalibrationDuration = Duration;
    }//autoCalibration

    void OpticalLightBarrierESP32::getCalibrationParams(bool *pAutoCalibration, uint16_t *pCalibrationDuration)const{
        if(nullptr != pCalibrationDuration) (*pCalibrationDuration) = m_CalibrationDuration;
        if(nullptr != pAutoCalibration) (*pAutoCalibration) = m_AutoCalibration;
    }//autoCalibration

}//name-space


#endif