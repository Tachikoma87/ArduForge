/*****************************************************************************\
*                                                                           *
* File(s): CC1101.h and CC1101.cpp                                          *
*                                                                           *
* Content: This class can be used to transmit and receive data with a       *
*          CC1101 transceiver using various frequencies.                    *
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
     * \brief This class implements data transmitting and receiving for devices with a CC1101 transceiver chip.
     * 
     * You are free to configure the pins of the device on any available digital pins. Specify the pin assignment in the CC1101::begin method. 
     * The implementation has been inspired by SpaceTeddy's (https://github.com/SpaceTeddy/CC1101) and simonmonk's (https://github.com/simonmonk/CC1101_arduino) CC1101 implementations. They where a great help so thanks for sharing.
     * \warning Don't forget that the CC1101 requires 3.3V input and you will nee a a logic converter or voltage dividers in case you connect from a higher voltage device such as the Arduino. There are tutorials mentioning that it is possible to connect directly, but I fried one breakout board with direct connection, so better be save than sorry. You can use 220ohm/470ohm voltage dividers to connect from a 5V device. 
     * \remarks The class was mainly tested using 433 MHz. Only small package sizes are allowed since larger package sizes require elaborate syncing of sender and receiver. I was able to send 59 bytes of payload data in single packages without problems.
     */
    class CC1101{
    public:
        /**
         * \brief Enumeration of the CC1101's config registers.
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
            SRES = 0x30, ///< Reset chip
            SFSTXON = 0x31, ///< Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
            SXOFF = 0x32, ///< Turn off crystal oscillator
            SCAL = 0x33, ///< Calibrate frequency synthesizer and turn it off (enables quick start). 
            SRX = 0x34, ///< Enable RX. Perform calibration first if coming from IDLE and MCSM0.FS_AUTOCAL01.
            STX = 0x35, ///< In IDLE state: Enable TX. Perform calibration first if MCSM0.FS_AUTOCAL01. If in RX state and CCA is enabled: Only go to TX if channel is clear.
            SIDLE = 0x36, ///< Exit RX/TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable
            SAFC = 0x37, ///< Perform AFC adjustment of the frequency synthesizer 
            SWOR = 0x38, ///< Start automatic RX polling sequence (Wake-on-Radio)
            SPWD = 0x39, ///< Enter power down mode when Csn goes high.
            SFRX = 0x3A, ///< Flush the RX FIFO buffer.
            SFTX = 0x3B, ///< Flush the TX FIFO buffer.
            SWORRST = 0x3C, ///< Reset real time clock 
            SNOP = 0x3D, ///< No operation. May be used to pad strobe commands to two INT8Us for simpler software
        };//CmdStrobes

        /**
         * \brief Register definitions.
         * 
         */
        enum Reg: uint8_t{
            PARTNUM = 0x30, ///< Part number for CC1101
            VERSION = 0x31, ///< Current version number
            FREQEST = 0x32, ///< Frequency offset estimate
            LQI = 0x33, ///< Demodulator estimate for Link Quality
            RSSI = 0x34, ///< Received signal strength indication
            MARCSTATE = 0x35, ///< Control state machine state (Register used primarily for test purposes)
            WORTIME1 = 0x36, ///< wake-on-radio (WOR) time (High byte)
            WORTIME0 = 0x37, ///< wake on radio (WOR) time (Low byte)
            PKTSTATUS = 0x38, ///< Current GD=x status and packet status
            VCO_VC_DAC = 0x39, ///< Current setting from PLL calibration module
            TXBYTES = 0x3A, ///< Underflow and number of bytes in the TX FIFO
            RXBYTES = 0x3B, ///< Overflow and number of bytes in the RX FIFO

            RCCTRL1_STATUS = 0x3C, ///< Last RC oscillator calibration result
            RCCTRL0_STATUS = 0x3D, ///< Last RC oscillator calibration result

            PATABLE = 0x3E, ///< Power control selection (Index into PA-Table)
            TXFIFO = 0x3F, ///< FIFO access
            RXFIFO = 0x3F ///< FIFO access
        }; //registers 

        /**
         * \brief FIFO access definitions (single or burst)
         * 
         */
        enum Fifo: uint8_t{
            WRITE_BURST = 0x40, ///< burst write 
            READ_SINGLE = 0x80, ///< single byte read
            READ_BURST = 0xC0, ///< burst read
            BYTES_IN_RXFIFO = 0x7F // number of bytes in FIFO
        }; //Fifo 

        /**
         *  \brief Definition of errors that may occur.
         * 
         */
        enum Error: uint8_t{
            NO_ERROR = 0x00,        ///< No error occurred
            INVALID_DEVICE_ADR = 0x01, ///< Device address invalid (0x00 and 0xFF reserved for broadcast)
            VALUE_OUT_OF_BOUNDS = 0x02, ///< Invalid value specified
            NULLPTR_ERROR = 0x03, ///< Nullpointer error
            RECEIVED_OVERSIZED_PACKAGE = 0x04, ///< Received package size exceeds configuration
        };//Error

        static const uint8_t BroadcastAdr = 0x00; ///< Default broadcast address
        static const uint8_t BraodcastAdrAlt = 0xFF; ///< Alternative broadcast address

        /**
         * \brief Constructor
         **/
        CC1101(void);

        /**
         * \brief Destructor
         */
        ~CC1101(void);

        /**
         * \brief Starts communication with the CC1101 device
         * 
         * \param[in] DeviceAddr Address of the device use for package filtration. 0x00 and 0xFF are reserved for broadcasts.
         * \param[in] Gdo0 Gdo0 pin
         * \param[in] Freq Frequency to use. \see CC1101::Frequency
         * \param[in] Gdo2 Gdo2 pin (not used in current implementation, set to 0)
         * \param[in] Csn Csn (aka SS) pin
         * \param[in] Mosi Master's mosi (master output slave input) pin 
         * \param[in] Miso Master's miso (master input slave output) pin
         * \param[in] Sck Sck (clock) pin
         * \remarks miso, mosi, sck and Csn (SS) pin are usually hardware specific. Make sure pin wiring matches your device. Default values are valid for Arduino.
         **/
        void begin(uint8_t DeviceAddr, uint8_t Gdo0 = 2, uint8_t Freq = F_433, uint8_t Gdo2 = 0x0, uint8_t Csn = 10,  uint8_t Mosi = 11, uint8_t Miso = 12, uint8_t Sck = 13);

        /**
         * \brief End communication with the CC1101 device
         */
        void end(void);

        /**
         * \brief Sends out a broadcast message.
         * 
         * \param[in] pData Data to send.
         * \param[in] Size Number of bytes to send.
         */
        void broadcast(uint8_t *pData, uint8_t Size);

        /**
         * \brief Send a data package to a particular device or use 0x00/0xFF for a broadcast message.
         * 
         * \param[in] Receiver Address of receiving device
         * \param[in] pData Data to send
         * \param[in] Size Number of bytes to send
         */
        void sendData(uint8_t Receiver, uint8_t *pData, uint8_t Size);

        /**
         * \brief Puts the device in receive mode. Data can be received only after this method was called.
         */
        void receiveMode(void);

        /**
         * \brief Returns number of bytes that have been received by the radio device.
         * 
         * \return Number of bytes available
         */
        uint8_t dataAvailable(void);

        /**
         * \brief Read received data
         * 
         * \param[out] pData Buffer which will store the data. Must not be nullptr.
         * \param[out] pReceiver Will store target address of the package. Can be nullptr.
         * \param[out] pSender Will store address of the package's originator. Can be nullptr.
         * \param[out] pRSSI Will store receiver signal strength indicator (RSSI) in dBm for the received package.
         * \return Number of bytes read.
         */
        uint8_t receiveData(uint8_t *pData, uint8_t *pReceiver, uint8_t *pSender, int8_t *pRSSI);

        /**
         * \brief Returns last occurred error.
         * 
         * \param[in] Clear Clear error after read?
         * \return Last occurred error.
         * \see CC1101::Error
         */
        Error lastError(bool Clear = true);

        /**
         * \brief Set a new device address
         * 
         * \param[in] Addr New address. Must be in range from 0x01 to 0xFE
         */
        void deviceAddress(uint8_t Addr);

        /**
         * \brief Returns current device address.
         * 
         * \return Current device address
         */
        uint8_t deviceAddress(void)const;

        /**
         * \brief Set device to new Channel.
         * 
         * \param[in] Channel New channel number
         */
        void channel(uint8_t Channel);

        /**
         * \brief Returns current channel number.
         * 
         * \return Current channel number
         */
        uint8_t channel(void);

        /**
         * \brief Set new output power level. Can be used to amplify or reduce radio strength. Valid values are in range -30 to 10. Unit is dBm. Default is 0.
         * 
         * \param[in] dBm Power amplification/reduction in dBm. Valid range is -30 to 10.
         */
        void outputPowerLevel(int8_t dBm);

        /**
         * \brief Returns current output power amplification/reduction in dBm.
         * 
         * \return Current output power amplification/reduction in dBm
         */
        int8_t outputPowerLevel(void);

        /**
         * \brief Device will enter power down state to reduce energy consumption.
         */
        void powerDown(void);

        /**
         * \brief Wake device from powered down state.
         */
        void wakeUp(void);
    
        /**
         * \brief Returns receiver strength indicator read from the respective register and converted to dBm. 
         * 
         * \return Current RSSI value in dBm
         * \remarks Reading from the register seems to be not very reliable. Better use the RSSI value returned by the C1101::receiveData method.
         */
        int8_t rssi(void);

        /**
         * \brief Returns link quality indicator read from the respective register.
         * 
         * \return Link quality indicator read from the respective register.
         * 
         */
        uint8_t lqi(void);

        /**
         * \brief Returns the chip part number.
         * 
         * \return Chip part number
         */
        uint8_t partNumber(void)const;

        /**
         * \brief Returns chip version number.
         * 
         * \return Chip version number
         */
        uint8_t version(void)const;
    private:
        /**
         * \brief Initialize SPI communication with the radio device.
         */
        void spiInit(void);

        /**
         * \brief Ends SPI communication with the radio device.
         */
        void spiEnd(void);

        /**
         * \brief SPI mode configuration.
         */
        void spiMode(uint8_t Config);

        /**
         * \brief Transfers a single byte via SPI and returns received byte.
         * 
         * \param[in] Value Byte to send.
         * \return Received byte 
         */
        uint8_t spiTransfer(uint8_t Value)const;

        /**
         * \brief Resets the device.
         */
        void reset(void);

        /**
         * \brief Writes a register via SPI.
         * 
         * \param[in] Reg Register address to write to.
         * \param[in] Value Byte value that will be written in the register.
         */
        void spiWriteReg(uint8_t Reg, uint8_t Value);

        /**
         * \brief Writes a series of bytes to a register.
         * 
         * \param[in] Reg Register to write to.
         * \param[in] pData Data bytes that will be send.
         * \param[in] Count Number of bytes to send.
         * 
         */
        void spiWriteRegBurst(uint8_t Reg, uint8_t *pData, uint8_t Count);

        /**
         * \brief Sends a strobe command to the device.
         * 
         * \param[in] Cmd Strobe command value.
         * \see CC1101::CmdStrobes
         */
        void spiStrobe(uint8_t Cmd);

        /**
         * \brief Read a single byte from a register.
         * 
         * \param[in] Reg Register address.
         * \return Byte value of the specified register
         */
        uint8_t spiReadReg(uint8_t Reg)const;

        /**
         * \brief Read values from a register in burst mode.
         * 
         * \param[in] Reg Register address
         * \param[out] pData Buffer where values will be stored.
         * \param[in] Count Number of bytes to read
         */
        void spiReadRegBurst(uint8_t Reg, uint8_t *pData, uint8_t Count);

        /**
         * \brief Helper method to read FIFO status.
         * 
         * \param[in] Addr FIFO address
         * \return Status value
         */
        uint8_t spiReadStatus(uint8_t Addr);

        /**
         * \brief Initializes chip configuration for a particular frequency.
         * 
         * \param[in] Freq Frequency that will be used. Can Be Frequency::F_915, Frequency::F433, or Frequency::F_868
         * 
         */
        void regConfigSettings(uint8_t Freq);

        /**
         * \brief Converts Receiver signal strength indicator (RSSI) from decimal value (from register) to dBm.
         * 
         * \param[in] RssiValueDec Decimal RSSI value.
         * \return RSSI value in dBm
         */
        int8_t rssiConvert(uint8_t RssiValueDec);

        uint8_t m_Miso; ///< Miso Pin
        uint8_t m_Mosi; ///< Mosi Pin
        uint8_t m_Csn;  ///< CSn pin (aka SS pin)
        uint8_t m_Sck;  ///< Sck (clock) pin
        uint8_t m_Gdo0; ///< GDo0 pin
        uint8_t m_Gdo2; ///< GDo2 pin

        uint8_t m_AvailableData; ///< Bytes available in RX buffer after receiving a package.

        Error m_LastError; ///< Last error that occurred during execution
    }; // CC1101

}//name space

#endif