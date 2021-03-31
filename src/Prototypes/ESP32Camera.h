/*****************************************************************************\
*                                                                           *
* File(s): ESP32Camera.h and ESP32Camera.cpp                                *
*                                                                           *
* Content: Basic class to initialize and deal with an ESP32 camera.         *
*                                                                           *
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
#ifndef __ESP32CAMERA_H__
#define __ESP32CAMERA_H__
#ifdef ESP32

#include <Arduino.h>
#include <inttypes.h>
#include <esp_camera.h>

namespace ArduForge{

    /**
     * @brief Configurates and initializes an ESP32 camera module.
     * 
     * After initializing you can use the usual esp camera methods to retrieve image frames.
     * @note This class is designed to be used with an AI_THINKER type module and an OV2640 camera. Don't know whether it works with other configurations, too.
     * 
     */
    class ESP32Camera{
    public:

        /**
         * @brief Constructor
         */
        ESP32Camera(void);

        /**
         * @brief Destructor
         */
        ~ESP32Camera(void);

        /**
         * @brief Starts the camera module.
         * @param[in] Framesize Image size to capture.
         * @param[in] PixelFormat Pixelformat that will be provided.
         * @param[in] JpegQuality Jpeg quality setting. Ignored if Pixelformat is not of jpeg type.
         * @note Using other format than jpeg significantly decreases framerate, but still faster than retrieving and converting jpeg.
         */
        bool begin(framesize_t Framesize, pixformat_t PixelFormat, uint8_t JpegQuality = 10);

        /**
         * @brief Ends communication with the sensor.
         */
        void end(void);

    protected:

    };//ESP32Camera
}//name-space

#endif // ESP32
#endif // Header-Guard

