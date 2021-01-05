/*****************************************************************************\
*                                                                           *
* File(s): CC1101.h and CC1101.cpp                                          *
*                                                                           *
* Content: This class can be used to transmit and receive data with a       *
*          CC1101 transceiver using 433 MHz frequency.                      *
*                                                                           *
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

#ifndef ARDUFORGE_CC1101_H
#define ARDUFORGE_CC1101_H 

namespace ArduForge{

    /**
     * \brief This class implements data transmitting and receiving with a CC1101 transceiver using 433 MHz frequency.
     * 
     * 
     * \Todo Documentation
     * 
     */
    class CC1101{
    public:
        /**
         * \brief Enumerate of the CC1101's config registers.
         */
        enum Config: uint8_t{
            IOCFG2 = 0x00, ///< GDO2 output pin configuration
            IOCFG1 = 0x01, ///< GDO1 output pin configuration 
            IOCFG0 = 0x02, ///< GDO2 output pin configuration 
            FIFOTHR = 0x03, ///< RX FIFO and TX FIFO threshold 
            SYNC1 = 0x04, ///< Sync word, high INT8U 
            SYNC0 = 0x05, ///< Sync word, low INT8U 
            PKTLEN = 0x06, ///< Packet length 
            PKTCTRL1 = 0x07, ///< Packet automation control 
            PKTCTRL0 = 0x08, ///< Packet automation control 
            ADDR = 0x09, ///< Device Address
            CHANNR = 0x0A, ///< Channel number 
            FSCTRL1 = 0x0B, ///< Frequency synthesizer control 
            FSCTRL0 = 0x0C, ///< Frequency synthesizer control 
            FREQ2 = 0x0D, ///< Frequency control word, high INT8U 
            FREQ1 = 0x0E, ///< Frequency control word, middle INT8U 
            FREQ0 = 0x0F, ///< Frequency control word, low INT8U 
            MDMCFG4 = 0x10, ///< Modem configuration
            MDMCFG3 = 0x11, ///< Modem configuration 
            MDMCFG2 = 0x12, ///< Modem configuration 
            MDMCFG1 = 0x13, ///< Modem configuration 
            MDMCFG0 = 0x14, ///< Modem configuration 
            DEVIATN = 0x15, ///< Modem deviation setting
            MCSM2 = 0x16, ///< Main radio Control State Machine configuration 
            MCSM1 = 0x17, ///< Main Radio Control State Machine configuration
            MCSM0 = 0x18, ///< Main Radio Control State machine configuration 
            FOCCFG = 0x19, ///< Frequency Offset Compensation configuration 
            BSCFG = 0x1A, ///< Bit Synchronization configuration 
            AGCCTRL2 = 0x1B, ///< AGC control 
            AGCCTRL1 = 0x1C, ///< AGC control 
            AGCCTRL0 = 0x1D, ///< AGC control 
            WOREVT1 = 0x1E, ///< High INT8U Event 0 timeout
            WOREVT0 = 0x1F, ///< Low INT8U Event 0 timeout 
            WORCTRL = 0x20, ///< Wake On Radio Control 
            FREND1 = 0x21, ///< Front end RX configuration 
            FREND0 = 0x22, ///< Front end TX configuration
            FSCAL3 = 0x23, ///< Frequency synthesizer calibration 
            FSCAL2 = 0x24, ///< Frequency synthesizer calibration 
            FSCAL1 = 0x25, ///< Frequency synthesizer calibration 
            FSCAL0 = 0x26, ///< Frequency synthesizer calibration
            RCCTRL1 = 0x27, ///< RC oscillator configuration 
            RCCTRL0 = 0x28, ///< RC oscillator configuration 
            FSTEST = 0x29, ///< Frequency synthesizer calibration control 
            PTESt = 0x2A, ///< Production test
            AGCTEST = 0x2B, ///< AGC test 
            TEST2 = 0x2C, ///< Various test settings 
            TEST1 = 0x2D, ///< Various test settings 
            TEST0 = 0x2E ///< Various test settings 
        };//Config

        /**
         * \brief Frequency control enumerates.
         */
        enum Frequency: uint8_t{
            F_915 = 0x00, ///< Frequency 915 MHz
            F_433 = 0x01, ///< Frequency 433 MHz
            F_868 = 0x02, ///< Frequency 868 MHz

            F2_868 = 0x21, ///< F2 config for 868 MHz
            F1_868 = 0x62, ///< F1 config for 868 MHz
            F0_868 = 0x76, ///< F0 config for 868 MHz

            F2_915 = 0x22, ///< F2 config for 915 MHz
            F1_915 = 0xB1, ///< F1 config for 915 MHz
            F0_915 = 0x3B, ///< F0 config for 915 MHz

            F2_433 = 0x10, ///< F2 config for 433 MHz
            F1_433 = 0xA7, ///< F1 config for 433 MHz
            F0_433 = 0x62 ////< f0 config for 433 MHz
        };//Modulation 

        enum CmdStrobes: uint8_t{
            SRES = 0x30, // Reset chip
            SFSTXON = 0x31, // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
            SXOFF = 0x32, // Turn off crystal oscillator
            SCAL = 0x33, // Calibrate frequency synthesizer and turn it off (enables quick start). 
            SRX = 0x34, // Enable RX. Perform calibraiton first if coming from IDLE and MCSM0.FS_AUTOCAL01.
            STX = 0x35, // In IDLE state: Enable TX. Perform calibration first if MCSM0.FS_AUTOCAL01. If in RX state and CCA is enabled: Only go to TX if channel is clear.
            SIDLE = 0x36, // Exit RX/TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable
            SAFC = 0x37, // Perform AFC adjustment of the frequency synthesizer 
            SWOR = 0x38, // Start automatic RX polling sequence (Wake-on-Radio)
            SPWD = 0x39, // Enter power down mode when Csn goes high.
            SFRX = 0x3A, // Flush the RX FIFO buffer.
            SFTX = 0x3B, // Flush the TX FIFO buffer.
            SWORRST = 0x3C, // Reset real time clock 
            SNOP = 0x3D, // No operation. May be used to pad strobe commands to two INT8Us for simpler software
        };//CmdStrobes

        enum Reg: uint8_t{
            PARTNUM = 0x30,
            VERSION = 0x31,
            FREQEST = 0x32,
            LQI = 0x33,
            RSSI = 0x34,
            MARCSTATE = 0x35,
            WORTIME1 = 0x36,
            WORTIME0 = 0x37,
            PKTSTATUS = 0x38,
            VCO_VC_DAC = 0x39,
            TXBYTES = 0x3A,
            RXBYTES = 0x3B,

            PATABLE = 0x3E,
            TXFIFO = 0x3F,
            RXFIFO = 0x3F
        }; //registers 

        enum Fifo: uint8_t{
            WRITE_BURST = 0x40, // write burst 
            READ_SINGLE = 0x80, // read single 
            READ_BURST = 0xC0, // read burst 
            BYTES_IN_RXFIFO = 0x7F // byte number in RXFifo 
        }; //Fifo 

        enum Error: uint8_t{
            NO_ERROR = 0x00,
            INVALID_DEVICE_ADR = 0x01,
            VALUE_OUT_OF_BOUNDS = 0x02,
            NULLPTR_ERROR = 0x03,
            RECEIVED_OVERSIZED_PACKAGE = 0x04,
        };//Error
        static const uint8_t BroadcastAdr = 0x00;

        CC1101(void);
        ~CC1101(void);

        void begin(uint8_t DeviceAdr, uint8_t Mosi, uint8_t Miso, uint8_t Csn, uint8_t Sck, uint8_t Gdo0, uint8_t Gdo2, uint8_t MaxPackageSize = 64);
        void end(void);

        void broadcast(uint8_t *pData, uint8_t Size);
        void sendData(uint8_t Receiver, uint8_t *pData, uint8_t Size, bool RequestAck = false);
        void receiveMode(void);
        uint8_t dataAvailable(void);
        uint8_t receiveData(uint8_t *pData, uint8_t *pReceiver, uint8_t *pSender, int8_t *pRSSI);

        Error lastError(bool Clear = true);

        void deviceAddress(uint8_t Addr);
        uint8_t deviceAddress(void)const;

        void channel(uint8_t Channel);
        uint8_t channel(void);

        void outputPowerLevel(int8_t dBm);
        int8_t outputPowerLevel(void);

        void powerDown(void);
        void wakeUp(void);
    
        int8_t rssi(void);
        uint8_t lqi(void);

    private:
        void spiInit(void);
        void spiEnd(void);
        void spiMode(uint8_t Config);
        uint8_t spiTransfer(uint8_t Value)const;
        void reset(void);
        void spiWriteReg(uint8_t Reg, uint8_t Value);
        void spiWriteRegBurst(uint8_t Reg, uint8_t *pData, uint8_t Count);
        void spiStrobe(uint8_t Cmd);
        uint8_t spiReadReg(uint8_t Reg)const;
        void spiReadRegBurst(uint8_t Reg, uint8_t *pData, uint8_t Count);
        uint8_t spiReadStatus(uint8_t Addr);
        void regConfigSettings(uint8_t Cfg);

        int8_t rssiConvert(uint8_t RssiValueDec);

        uint8_t m_Miso;
        uint8_t m_Mosi;
        uint8_t m_Csn;
        uint8_t m_Sck;
        uint8_t m_Gdo0;
        uint8_t m_Gdo2;

        uint8_t m_AvailableData;

        Error m_LastError;     
    }; // CC1101

}//name space



#endif