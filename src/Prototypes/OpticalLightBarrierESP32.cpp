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

    void OpticalLightBarrierESP32::BarrierLine::init(uint8_t Magnitude, uint16_t LineSize, bool Grayscale){
        clear();
        this->Magnitude = Magnitude;   
        this->Grayscale = Grayscale;
        this->LineSize = LineSize;

        uint16_t BufferSize = (Magnitude*2+1)*LineSize*sizeof(uint8_t);
        if(!Grayscale) BufferSize *= 2;
     
        pBuffer = new uint8_t[BufferSize];
        memset(pBuffer, 0, BufferSize);
        
    }//initialize

    void OpticalLightBarrierESP32::BarrierLine::clear(void){
        Magnitude = 0;
        LineSize = 0;
        delete[] pBuffer;
        pBuffer = nullptr;
    }//clear

    void OpticalLightBarrierESP32::BarrierLine::splitRGB565(uint16_t PixelData, int8_t *pRed, int8_t *pGreen, int8_t *pBlue){
        (*pRed) = (int8_t) ((PixelData & 0b1111100000000000) >> 11);
        (*pGreen) = (int8_t) ((PixelData & 0b0000011111100000) >> 5);
        (*pBlue) = (int8_t)(PixelData & 0b0000000000011111);
        (*pRed)  = (*pRed) << 2;
        (*pGreen) = (*pGreen) << 1;
        (*pBlue) = (*pBlue) << 2;
    }//splitRGB565

    void OpticalLightBarrierESP32::BarrierLine::extractData(camera_fb_t *pFB){
        if(nullptr == pFB) return;

        uint32_t PixelCount = pFB->width * (2 * Magnitude + 1);
        uint16_t Index = (pFB->height/2 - Magnitude)*pFB->width;
        if(Grayscale) memcpy(&pBuffer[0], &pFB->buf[Index], sizeof(uint8_t)*PixelCount);
        else memcpy(&pBuffer[0], &pFB->buf[Index*2], sizeof(uint16_t)*PixelCount);

    }//retrieveData

    void OpticalLightBarrierESP32::BarrierLine::augmentCaptureLine(uint8_t LineMagnitude, uint32_t Width, uint32_t Height, uint8_t *pImgData, bool Grayscale){
        uint32_t PixelCount = Width * (2 * LineMagnitude + 1);
        uint32_t Index = (Height/2 - LineMagnitude)*Width;

        ON_DEBUG(Serial.printf("Augmenting line %d Pixels from %d Index\n", PixelCount, Index));

        for(uint32_t i=0; i < PixelCount; ++i){
            if(Grayscale) pImgData[Index + i] = 0;
            else {
                pImgData[ (Index+i)*3 + 0] = 0;
                pImgData[ (Index+i)*3 + 1] = 0;
                pImgData[ (Index+i)*3 + 2] = 0;
            }
        } 
    }//augmentCaptureLine

    void OpticalLightBarrierESP32::BarrierLine::augmentMeasurementLines(uint8_t LineMagnitude, std::vector<MeasureLine*> *pMeasurementLines, uint32_t Width, uint32_t Height, uint8_t *pImgData, bool Grayscale){

        for(auto i: (*pMeasurementLines)){
            for(uint16_t y = Height/2 - LineMagnitude; y < Height/2 + LineMagnitude; ++y){
                for(uint16_t x = i->Start; x <= i->End; ++x){
                    uint32_t Index = y * Width + x;
                    if(Grayscale) pImgData[Index] = 255;
                    else{
                        pImgData[Index*3 + 0] = 255;
                        pImgData[Index*3 + 1] = 255;
                        pImgData[Index*3 + 2] = 255;
                    }
                }//for[columns]
            }//for[rows]
        }//for[Measurement lines]

    }//augmentMeasurementLines

    uint8_t OpticalLightBarrierESP32::BarrierLine::pixelValue(uint16_t x, uint16_t y){
        uint8_t Rval = 0;
        uint16_t Index = (y * LineSize) + x;
        if(Grayscale){
            Rval = pBuffer[Index];
        }else{
            uint16_t LowByte = pBuffer[Index*2+1];
            uint16_t HighByte = pBuffer[Index*2+0];
            uint16_t Value = (HighByte << 8) | LowByte;
            int8_t R,G,B;
            splitRGB565(Value, &R, &G, &B);
            float Grey = (R/64.0f + G/128.0f + B/64.0f)/3.0f;
            Rval = (uint8_t)(Grey*255.0f);
        }
        
        return Rval;
    }//

    void OpticalLightBarrierESP32::BarrierLine::pixelValue(uint16_t x, uint16_t y, int8_t *pRed, int8_t *pGreen, int8_t *pBlue){
        uint16_t Index = (y * LineSize) + x;
        if(Grayscale){
            (*pRed) = pBuffer[Index];
            (*pGreen) = pBuffer[Index];
            (*pBlue) = pBuffer[Index];
        }else{
            uint16_t LowByte = pBuffer[Index*2+1];
            uint16_t HighByte = pBuffer[Index*2+0];
            uint16_t Value = (HighByte << 8) | LowByte;
            splitRGB565(Value, pRed, pGreen, pBlue);
        }
    }//pixelValue

    float OpticalLightBarrierESP32::BarrierLine::filter3x3(uint16_t x, uint16_t y){
        uint32_t Sum = 0;
        for(uint16_t u = x; u < x + 3; ++u){
            for(uint16_t v = y; v < y + 3; ++v){
                Sum += pixelValue(u,v);
            }
        }
        return (float)(Sum / 9.0f);
    }//filter3x3

    void OpticalLightBarrierESP32::BarrierLine::filter3x3(uint16_t x, uint16_t y, int8_t *pRed, int8_t *pGreen, int8_t *pBlue){

        int8_t R,G,B;   
        int16_t RSum = 0;
        int16_t GSum = 0;
        int16_t BSum = 0; 
        for(uint16_t u = x; u < x + 3; ++u){
            for(uint16_t v = y; v < y + 3; ++v){
                pixelValue(u,v, &R, &G, &B);
                RSum += R;
                GSum += G;
                BSum += B;
            }
        }
        
        (*pRed) = (int8_t)(RSum/9.0f);
        (*pGreen) = (int8_t)(GSum/9.0f);
        (*pBlue) = (int8_t)(BSum/9.0f);

    }//filter3x3


    float OpticalLightBarrierESP32::BarrierLine::computeDeviation(BarrierLine *pLeft, BarrierLine *pRight, bool UseFilter, bool LinearError){
        float TotalDeviation = 0;
        uint16_t Scale = 0;

        if(pLeft->Grayscale){
            uint16_t PixelCount = pLeft->Magnitude * pLeft->LineSize;
            for(uint16_t i=0; i < PixelCount; ++i){
                if(LinearError) TotalDeviation += (float)abs(pLeft->pBuffer[i] - pRight->pBuffer[i]);
                else TotalDeviation += powf((float)abs(pLeft->pBuffer[i] - pRight->pBuffer[i]), 2.0f);
                Scale++;
            }
        }else{
            int8_t Red1,Green1,Blue1, Red2,Green2,Blue2;
            for(uint16_t y = 0; y < pLeft->Magnitude * 2+1; y++){
                for(uint16_t x = 0; x < pLeft->LineSize; ++x){
                    pLeft->pixelValue(x,y, &Red1, &Green1, &Blue1);
                    pRight->pixelValue(x,y, &Red2, &Green2, &Blue2);
                    if(LinearError) TotalDeviation += (float)(abs(Red1 - Red2) + abs(Green1 - Green2) + abs(Blue1 - Blue2));
                    else TotalDeviation += powf((float)(Red1 - Red2), 2.0f) + powf((float)(Green1 - Green2), 2.0f) + powf((float)(Blue1 - Blue2), 2.0f);
                    Scale++;
                }
            }
        }
        
        return TotalDeviation / (float)Scale;
    }//computeDeviation

    float OpticalLightBarrierESP32::BarrierLine::computeDeviation(uint16_t XStart, uint16_t XEnd, BarrierLine *pLeft, BarrierLine *pRight, bool UseFilter, bool LinearError){
        float TotalDeviation = 0;
        uint16_t Scale = 0;

        if(pLeft->Grayscale && UseFilter){
            for(uint16_t y = 0; y < pLeft->Magnitude*2+1 + 3; y += 3){
                for(int x=XStart; x < XEnd + 3; x += 3){       
                    float Left = pLeft->filter3x3(x,y);
                    float Right = pRight->filter3x3(x,y);
                    if(LinearError) TotalDeviation += (float)abs(Left - Right);
                    else TotalDeviation += powf(Left-Right, 2.0f);
                    Scale++;
                }
            }   
        }else if(pLeft->Grayscale){

            for(uint16_t y = 0; y < pLeft->Magnitude*2 +1; ++y){
                for(int x=XStart; x < XEnd; ++x){
                    float LV = pLeft->pixelValue(x,y);
                    float LR = pRight->pixelValue(x,y);
                    if(LinearError) TotalDeviation += (float)abs(LV - LR);
                    else TotalDeviation += powf((float)abs(LV-LR), 2.0f);
                    Scale++;
                }
            }  
        }else if(!pLeft->Grayscale && UseFilter){
            int8_t LRed, LGreen, LBlue;
            int8_t RRed, RGreen, RBlue;
            for(uint16_t y = 0; y < pLeft->Magnitude*2+1 + 3; y += 3){
                for(int x=XStart; x < XEnd + 3; x += 3){       
                    pLeft->filter3x3(x,y, &LRed, &LGreen, &LBlue);
                    pRight->filter3x3(x,y, &RRed, &RGreen, &RBlue);

                    if(LinearError) TotalDeviation += (float)(abs(LRed-RRed) + abs(LGreen - RGreen) + abs(LBlue - RBlue));
                    else TotalDeviation += powf(LRed - RRed, 2.0f) + powf(LGreen-RGreen, 2.0f) + powf(LBlue-RBlue, 2.0f);
                    Scale++;
                }
            }      
        }else if(!pLeft->Grayscale){
            int8_t Red1,Green1,Blue1, Red2,Green2,Blue2;
            for(uint16_t y = 0; y < pLeft->Magnitude * 2+1; y++){
                for(uint16_t x = XStart; x <= XEnd; ++x){
                    pLeft->pixelValue(x,y, &Red1, &Green1, &Blue1);
                    pRight->pixelValue(x,y, &Red2, &Green2, &Blue2);

                    if(LinearError) TotalDeviation += (float)(abs(Red1-Red2) + abs(Green1 - Green2) + abs(Blue1 - Blue2));
                    else TotalDeviation += powf(Green1 - Green2, 2.0f) + powf(Green1 - Green2, 2.0f) + powf(Blue1 - Blue2, 2.0f);
                    Scale++;
                }
            }
        }

        //ON_DEBUG(Serial.print("Scale: "); Serial.print(Scale); Serial.print("\n"));          
        return TotalDeviation / (float)Scale;
    }//computeDeviation

    #pragma endregion

    #pragma region
    OpticalLightBarrierESP32::MeasureLine::MeasureLine(void){
        Start = 0;
        End = 0;
    }//Constructor

    OpticalLightBarrierESP32::MeasureLine::~MeasureLine(void){
        clear();
    }//Destructor

    void OpticalLightBarrierESP32::MeasureLine::init(uint16_t Start, uint16_t End){
        this->Start = Start;
        this->End = End;
        this->Noise = 0.0f;
        this->Triggered = false;
    }//initialize

    void OpticalLightBarrierESP32::MeasureLine::clear(void){
        Start = 0;
        End = 0;
        this->Noise = 0.0f;
        this->Triggered = false;
    }//clear


   
    void OpticalLightBarrierESP32::performCalibration(BarrierLine *pLine1, BarrierLine *pLine2){
        m_ActiveState = STATE_CALIBRATION;

        uint8_t FrameCount = 0;
        camera_fb_t *pFrame = nullptr;

        OpticalLightBarrierESP32::cameraAutomatics(true);
        //we ignore the first 10 images (about half a second) to let the camera calibrate itself
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

            for(auto i: m_MeasureLines){
                float Temp = BarrierLine::computeDeviation(i->Start, i->End, pLine1, pLine2, m_UseFilter, m_LinearError);
                //float Temp = BarrierLine::computeDeviation(pLine1, pLine2, m_UseFilter, m_LinearError);
                if(Temp > i->Noise) i->Noise = Temp;      
            }
                        
        }//while[calibration time]

        ON_DEBUG(Serial.print("Calibration finished with noise level of "); Serial.print(m_MeasureLines[0]->Noise););

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
        
        uint8_t LineToActualize = 0;

        // fps counter
        uint32_t FrameCount = 0;
        uint32_t LastFPSPrint = 0;

        uint32_t FrameHeight = 320;
        framesize_t FrameType = FRAMESIZE_QVGA;
        pixformat_t PixFormat = PIXFORMAT_GRAYSCALE;

        if(!pParent->m_GrayscaleMode){
            FrameType = FRAMESIZE_HQVGA;
            PixFormat = PIXFORMAT_RGB565;
            FrameHeight = 240;
        };

        // initialize basic middle line
        ON_DEBUG(Serial.print("Starting up barrier lines\n"));
        
        BarrierLine Line1;
        BarrierLine Line2;

        Line1.init(pParent->m_LineMagnitude, FrameHeight, pParent->m_GrayscaleMode);
        Line2.init(pParent->m_LineMagnitude, FrameHeight, pParent->m_GrayscaleMode);

        // initialize first barrier (whole line)
        pParent->m_FrameWidth = FrameHeight;
        pParent->addMeasureLine(FrameHeight * 0.1f, FrameHeight * 0.9f);

        
        if(!Camera.begin(FrameType, PixFormat)){
            ON_DEBUG(Serial.print("Failed initializing ESP32 Camera!\n"));
            pParent->m_Callback(BMSG_INITIALIZATION_FAILED, pParent->m_pUserCBParam);
            return;
        }else{
            ON_DEBUG(Serial.print("ESP32 Camera initialized!\n"));
        }
        pParent->m_ActiveState = STATE_NONE;
        pParent->m_NextState = STATE_NONE;
        pParent->m_Callback(BMSG_INITIALIZATION_FINISHED, pParent->m_pUserCBParam);
        pParent->m_MaxDeviation = 0.0f;

        
        while(!pParent->m_StopThread){

            camera_fb_t *pFB = nullptr;

            if(pParent->m_ClearMeasureLines){
                for(auto &i: pParent->m_MeasureLines) delete i;
                pParent->m_MeasureLines.clear();
                pParent->m_ClearMeasureLines = false;
            } 

            switch(pParent->m_ActiveState){   
                case STATE_CALIBRATION:{
                    pParent->performCalibration(&Line1, &Line2); 
                    pParent->m_ActiveState = STATE_NONE;
                }break;
                case STATE_START_DETECION:{
                    // set both lines to current capture values
                    if(pParent->m_AutoCalibration) pParent->performCalibration(&Line1, &Line2);
                    ON_DEBUG(Serial.print("Preparing detection\n"));

                    pFB = esp_camera_fb_get();

                    Line1.extractData(pFB);
                    Line2.extractData(pFB);
                    
                    esp_camera_fb_return(pFB);
                    // enter detection state
                    pParent->m_ActiveState = STATE_DETECTION;
                    pParent->m_FPS = 0;

                    ON_DEBUG(Serial.print("Preparation done\n"); );
                    pParent->m_Callback(BMSG_DETECTION_STARTED, pParent->m_pUserCBParam);
                    ON_DEBUG(Serial.print("Staring detection\n"); );
                    
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


                    // if(Line1.LineSize != pFB->width){
                    //     ON_DEBUG(Serial.print("Frame dimensions do not match\n"); Serial.print(Line1.LineSize); Serial.print(" vs "); Serial.print(pFB->width));
                    // }

                    (LineToActualize == 0) ? Line1.extractData(pFB) : Line2.extractData(pFB);
                    
                    esp_camera_fb_return(pFB);
                    LineToActualize = (LineToActualize + 1) % 2;
                    
                    bool Triggered = false;
                    for(auto i: pParent->m_MeasureLines){
                        float Deviation = BarrierLine::computeDeviation(i->Start, i->End, &(Line1), &(Line2), pParent->m_UseFilter, pParent->m_LinearError);
                        //float Deviation = BarrierLine::computeDeviation(&(Line1), &(Line2), pParent->m_UseFilter, pParent->m_LinearError);
                        if(Deviation > pParent->m_MaxDeviation) pParent->m_MaxDeviation = Deviation;
                        if(Deviation > i->Noise + pParent->m_Sensitivity){         
                            i->Triggered = true;
                            Triggered = true;
                            ON_DEBUG(Serial.print("Barrier triggered with value: "));
                            ON_DEBUG(Serial.print(Deviation));
                            ON_DEBUG(Serial.print("\n"));
                        }else{
                            i->Triggered = false;
                        }
                    }
                    if(Triggered) pParent->m_Callback(BMSG_BARRIER_TRIGGERED, pParent->m_pUserCBParam);
                    
                    pParent->m_FrameCount++;
                    if(millis() - pParent->m_LastFPSPrint > 1000){
                        pParent->m_FPS = pParent->m_FrameCount;
                        pParent->m_LastFPSPrint = millis();
                        pParent->m_FrameCount = 0;

                        ON_DEBUG(Serial.print("Max deviation last second: "); Serial.print(pParent->m_MaxDeviation); Serial.print("\n"));
                        ON_DEBUG(Serial.print("FPS: "); Serial.print(pParent->m_FPS); Serial.print("\n"));
                        pParent->m_MaxDeviation = 0.0f;
                    }

                }break;
                default:{
                    // nothing to do here
                    pParent->m_Callback(BMSG_IDLING, pParent->m_pUserCBParam);
                }break;
                
            }

            if(pParent->m_NextState != STATE_NONE){
                pParent->m_ActiveState = pParent->m_NextState;
                pParent->m_NextState = STATE_NONE;
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
        m_CalibrationDuration = 1500; 
        m_ActiveState = STATE_NONE;     
        m_FPS = 0;                
    }//Constructor

    OpticalLightBarrierESP32::~OpticalLightBarrierESP32(void){
        end();
    }//Destructor

    void OpticalLightBarrierESP32::begin(BarrierCallbackFunc BarrierCB, void *pUserCBParam, float Sensitivity, uint8_t CameraThreadCoreID, bool Grayscale, bool LinearError, bool UseFilter){
        m_Callback = BarrierCB;
        m_Sensitivity = (LinearError) ? Sensitivity : powf(Sensitivity, 2.0f);
        m_ActiveState = STATE_NONE;
        m_pUserCBParam = pUserCBParam;

        m_StopThread = false;
        m_LineMagnitude = 4;
        m_AutoCalibration = true;
        m_CalibrationDuration = 1000;
        m_FPS = 0;

        m_GrayscaleMode = Grayscale;
        m_LinearError = LinearError;
        m_UseFilter = UseFilter;


        // start camera capture thread
        xTaskCreatePinnedToCore(cameraThread, "Camera_Thread", 10000, this, 1, &m_hCameraThread, CameraThreadCoreID);
        
    }//begin

    uint32_t OpticalLightBarrierESP32::imageSize(void)const{
        camera_fb_t *pFB = esp_camera_fb_get();
        uint32_t Rval = 0;
        if(nullptr != pFB){
            Rval = pFB->width * pFB->height;
            Rval *= (m_GrayscaleMode) ? 1 : 3;
        } 
        esp_camera_fb_return(pFB);
        return Rval;
    }//imageSize

    void OpticalLightBarrierESP32::captureImage(uint32_t *pWidth, uint32_t *pHeight, uint8_t *pImgData, bool AugmentCaptureLine){
        camera_fb_t *pFB = esp_camera_fb_get();
        (*pWidth) = pFB->width;
        (*pHeight) = pFB->height;

        if(nullptr != pFB && m_GrayscaleMode){   
            memcpy(pImgData, pFB->buf, pFB->len);      
        }else if(pFB != nullptr){
            int8_t Red, Green, Blue;
            uint16_t LowByte = 0;
            uint16_t HighByte = 0;
            uint16_t Value = 0;
            for(uint32_t i=0; i < pFB->width * pFB->height; ++i){   
                LowByte = pFB->buf[i*2+1];
                HighByte = pFB->buf[i*2+0];
                Value = (HighByte << 8) | LowByte;
                BarrierLine::splitRGB565(Value, &Red, &Green, &Blue);
                pImgData[i*3 + 0] = Red << 1;
                pImgData[i*3 + 1] = Green << 1;
                pImgData[i*3 + 2] = Blue << 1;
            }
        }

        if(AugmentCaptureLine){
            BarrierLine::augmentCaptureLine(m_LineMagnitude, pFB->width, pFB->height, pImgData, m_GrayscaleMode);
            BarrierLine::augmentMeasurementLines(m_LineMagnitude, &m_MeasureLines, pFB->width, pFB->height, pImgData, m_GrayscaleMode);
        } 
        esp_camera_fb_return(pFB);
    }//captureImage

    void OpticalLightBarrierESP32::end(void){
        m_StopThread = true;
        m_hCameraThread = nullptr;
    }//end

    void OpticalLightBarrierESP32::calibrate(void){
        //m_StateQueue.push(STATE_CALIBRATION);
        m_NextState = STATE_CALIBRATION;
    }

    void OpticalLightBarrierESP32::startDetection(void){
        //m_StateQueue.push(STATE_START_DETECION);
        ON_DEBUG(Serial.print("Trying to start detection now\n"));
        m_NextState = STATE_START_DETECION;
    }//startDetection

    void OpticalLightBarrierESP32::stopDetection(void){
        //m_StateQueue.push(STATE_STOP_DETECTION);
        m_NextState = STATE_STOP_DETECTION;
    }//stopDetection

    float OpticalLightBarrierESP32::sensitivity(void)const{
        return m_Sensitivity;
    }//sensitivity

    void OpticalLightBarrierESP32::sensitivity(float Sensitivity){
        m_Sensitivity = (m_LinearError) ? Sensitivity : powf(Sensitivity, 2.0f);
    }//sensitivity)

    uint8_t OpticalLightBarrierESP32::fps(void)const{
        return m_FPS;
    }//fps

    OpticalLightBarrierESP32::ActiveState OpticalLightBarrierESP32::state(void)const{
        return m_ActiveState;
    }//ActiveState

    float OpticalLightBarrierESP32::noise(uint8_t MeasureLineID)const{
        if(MeasureLineID >= m_MeasureLines.size()) return -1.0f;
        return m_MeasureLines[MeasureLineID]->Noise;
    }//noise

    void OpticalLightBarrierESP32::setCalibrationParams(bool AutoCalibration, uint16_t Duration){
        m_AutoCalibration = AutoCalibration;
        m_CalibrationDuration = Duration;
    }//autoCalibration

    void OpticalLightBarrierESP32::getCalibrationParams(bool *pAutoCalibration, uint16_t *pCalibrationDuration)const{
        if(nullptr != pCalibrationDuration) (*pCalibrationDuration) = m_CalibrationDuration;
        if(nullptr != pAutoCalibration) (*pAutoCalibration) = m_AutoCalibration;
    }//autoCalibration

    uint8_t OpticalLightBarrierESP32::measureLineCount(void)const{
        return m_MeasureLines.size();
    }//measureLineCount

    bool OpticalLightBarrierESP32::isLineTriggered(uint8_t MeasureLineID)const{
        if(MeasureLineID >= m_MeasureLines.size()) return false;
        return m_MeasureLines[MeasureLineID]->Triggered;
    }//isLineTriggered

    void OpticalLightBarrierESP32::clearMeasureLines(void){
        m_ClearMeasureLines = true;
        while(m_ClearMeasureLines) delay(50);
    }//clearMeasureLines

    void OpticalLightBarrierESP32::addMeasureLine(uint16_t Start, uint16_t End){
        MeasureLine *pLine = new MeasureLine();
        if(Start > End) return;
        if(Start >= m_FrameWidth) Start = m_FrameWidth - 1;
        if(End >= m_FrameWidth) End = m_FrameWidth - 1;
        pLine->init(Start, End);
        m_MeasureLines.push_back(pLine);
        ON_DEBUG(Serial.print("Added measure line: "); Serial.print(Start); Serial.print(":"); Serial.print(End); Serial.print("\n"));
    }//addMeasureLine

    void OpticalLightBarrierESP32::getMeasureLine(uint8_t ID, uint16_t *pStart, uint16_t *pEnd){
        if(ID >= m_MeasureLines.size()) return;
        (*pStart) = m_MeasureLines[ID]->Start;
        (*pEnd) = m_MeasureLines[ID]->End;
    }//getMeasureLine

    uint16_t OpticalLightBarrierESP32::frameWidth(void)const{
        return m_FrameWidth;
    }//frameWidth

}//name-space


#endif