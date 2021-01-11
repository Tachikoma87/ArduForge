/*****************************************************************************\
*                                                                           *
* File(s): HC0X.h and HC0X.cpp                                              *
*                                                                           *
* Content: This class handles communication with HM10 BTLE devices.         *
*                                                                           *
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

#ifndef ARDUFORGE_HM10_H
#define ARDUFORGE_HM10_H 

#include <SoftwareSerial.h>
#include <inttypes.h>

namespace ArduForge{
    /**
     * \brief Class for the communication with an HM10 device via UART.
     * 
     * The SoftSerial class is used for UART communication, so you con use any digital ports you like. 
     * There seems to be an awful many different BTLE devices, which all accept a variety of AT commands, or do not. You will have to do some testing. The HM10::issueATCommand method will issue the command via the serial connection and you can investigate the results. Don't forget carrier return and/or line end symbol, depending on your device they are necessary, or not. Well, good luck.
     * 
     * With the "AT+HELP\r\n" command the device will print out a list of commands it accepts. See example files on how to print Help to serial monitor. Here the list of commands my device supposedly accepts.
     * 
     * AT command | Description
     * ---------- | -----------
     * AT+VERSION | Get firmware, bluetooth, HCI and LMP version
     * AT+HELP | List all the commands
     * AT+NAME | Get/Set local device name
     * AT+PIN | Get/set code for pairing
     * AT+PASS | Get/Set pin for pairing
     * AT+BAUD | Get/Set baud rate
     * AT+LADDR | Get local bluetooth address
     * AT+ADDR | Get local bluetooth address
     * AT+DEFAULT | Restore factory default
     * AT+RENEW | Restore factory default
     * AT+STATE | Get current state
     * AT+PWRM | Get/Set power on mode (low power)
     * AT+POWE | Get/Set RF transmit power
     * AT+SLEEP | Sleep mode
     * AT+ROLE | Get/Set current role
     * AT+PARI | Get/Set UART parity bit
     * AT+STOP | Get/Set UART stop bit
     * AT+START | System start working
     * AT+IMME | System wait for command when power on
     * AT+IBEA | Switch iBeacon mode
     * AT+IBE0 | Set iBeacon UUID 0
     * AT+IBE1 | Set iBeacon UUID 1
     * AT+IBE2 | Set iBeacon UUID 2
     * AT+IBE3 | Set iBeacon UUID 3
     * AT+MARJ | Set iBeacon MARJ
     * AT+MINO | Set iBeacon MINO
     * AT+MEA  | Set iBeacon MEA
     * AT+NOTI | Notify connection event
     * AT+UUID | Get/Set system SERVER_UUID
     * AT+CHAR | Get/Set system CHAR_UUID
     * 
     * \image html HM10Wiring.png
     * 
     * \remarks Class is designed to handle slave role of BTLE device only. My device can not perform master role, as you can see by the supported commands :(
     */
    class HM10{
    public:

        /**
         * \brief Establishes UART connection using the specified pins and tries different baudrates until a response is acknowledged. Then this baudrate is returned.
         * 
         * \param[in] RXPin RX pin
         * \param[in] TXPin TX pin
         * \return Baudrate which generated a response or 0 if no response has been received. 
         */
        static uint32_t discoverBaud(uint8_t RXPin, uint8_t TXPin);

        /**
         * \brief Constructor
         */
        HM10(void);

        /**
         * \brief Destructor
         */
        ~HM10(void);

        /**
         * \brief Establishes a connection to the BTLE device using specified pins. The SoftSerial class is used for the UART connection, so any digital pin will do.
         * 
         * \param[in] RXPin RX pin (connected to TX pin of the HM-10 device)
         * \param[in] TXPin TX pin (connected to RX pin of the HM-10 device)
         * \param[in] StatePin Status pin
         * \param[in] pName Name of the device. Can be nullptr.
         * \param[in] Reset Execute reset command on startup
         * \param[in] Baud Baudrate of the device. If default does not work try HM10::discoverBaudrate
         */
        void begin(uint8_t RXPin, uint8_t TXPin, uint8_t StatePin, const char* pName, bool Reset = true, uint32_t Baud = 9600);

        /**
         * \brief Ends communication with the device.
         */
        void end(void);

        /**
         * \brief Check if device responds to AT commands. Works only if device is in AT mode.
         * 
         * \return true if device responds correctly, false otherwise
         */
        bool check(void);

        /**
         * \brief Returns number of bytes available on the serial connection.
         * 
         * \return Number of bytes ready to be read.
         */
        uint16_t available(void);

        /**
         * \brief Reads a number of bytes from UART
         * 
         * \param[in] pBuffer Buffer to write the bytes to
         * \param[in] ByteCount Number of bytes to read
         * \return Number of bytes read
         */
        uint16_t read(uint8_t *pBuffer, uint16_t ByteCount);

        /**
         * \brief Read a single byte from UART.
         * 
         * \return Value read
         */
        uint8_t read(void);

        /**
         * \brief Write a number of bytes to UART
         * 
         * \param[in] pBuffer Bytes to write
         * \param[in] ByteCount Number of bytes to write
         * \return Number of bytes written
         */
        uint16_t write(uint8_t *pBuffer, uint16_t ByteCount);

        /**
         * \brief Prints a single printable type to UART
         * 
         * \param[in] Msg Data to print
         */
        template<typename T>
        void print(T Msg){
            if(nullptr != m_pCom) m_pCom->print(Msg);
        }//print

        /**
         * \brief Issues an AT command. Works only if device is not already connected. Use HM10::connected to find out.
         * 
         * \param[in] pCmd AT command string.
         * \param[in] pAnswerBuffer Stores the answer from the device.
         * \param[in] Timeout Maximum waiting time for a response in milliseconds.
         * \return Length of the response string in bytes or -1 if an error occurred.
         * \remarks If the device does not respond as intended, check whether your device requires carrier return and line ending symbols as part of the command string.
         */
        int16_t issueATCommand(const char *pCmd, uint8_t *pAnswerBuffer = nullptr, uint16_t Timeout = 1000);

        /**
         * \brief Returns the active serial connection.
         * 
         * \return Pointer to active UART connection
         */
        SoftwareSerial *serialConnection(void);

        /**
         * \brief Checks whether the device is connected to any remote device.
         * 
         * \return True if connected, false otherwise.
         */
        bool connected(void)const;

    protected:
        uint8_t m_RX; ///< RX pin for UART
        uint8_t m_TX; ///< TX pin for UART
        uint8_t m_State; ///< State pin. High if device has a bluetooth connection, false otherwise (AT mode)
        SoftwareSerial *m_pCom; ///< UART software connection
    };//HM10 

}//name space

#endif