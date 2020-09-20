/*****************************************************************************\
*                                                                           *
* File(s): MF52.h and MF52.cpp                                              *
*                                                                           *
* Content: Class to interact with an MF52 NTC Thermistor by using a basic   *
*          voltage divider circuit.                                         *
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
#ifndef ARDUFORGE_MPU6050_H
#define ARDUFORGE_MPU6050_H

#include <inttypes.h>
#include <Wire.h>

namespace ArduForge{
    class MPU6050{
    public:
   
        struct RawSensorData{
            int16_t AccelX, AccelY, AccelZ;
            int16_t GyroX, GyroY, GyroZ;
            float Temperature;

            RawSensorData(void){
                AccelX = AccelY = AccelZ = 0;
                GyroX = GyroY = GyroZ = 0;
                Temperature = 0.0;
            }//Constructor

        };//RawSensorData

        struct SensorData{
            float AccelX, AccelY, AccelZ;
            float GyroX, GyroY, GyroZ;
            float Temperature;
        };//SensorData

        enum struct GyroConfig : int8_t{
            SCALE_250 = 0,
            SCALE_500,
            SCALE_1000,
            SCALE_2000,
        };//GyroConfig

        enum struct AccelConfig: int8_t{
            SCALE_2G = 0,
            SCALE_4G,
            SCALE_8G,
            SCALE_16G,
        };//AccelConfig

        MPU6050(void);
        ~MPU6050(void);

        void begin(TwoWire *pWire = &Wire, int16_t Adr = 0x68, AccelConfig AC = AccelConfig::SCALE_2G, GyroConfig GC = GyroConfig::SCALE_250);
        void end(void);

        void sleep(void);
        void wake(void);

        void configAccel(AccelConfig Value);
        void configGyro(GyroConfig Value);
        void configThermistor(bool Enable);

        RawSensorData readRaw(void);
        SensorData read(void);

        void calibrate(void);
    protected:
        int16_t accelSensivity(AccelConfig Value);
        float gyroSensivity(GyroConfig Value);
        int8_t readRegister(int8_t Reg);

        int16_t m_SensorAdr;
        TwoWire *m_pWire;
        AccelConfig m_AccelConfig;
        GyroConfig m_GyroConfig;
        RawSensorData m_Calibration;
    };//MPU6050

}//names-pace


#endif