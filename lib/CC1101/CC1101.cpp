#include <Arduino.h>
#include "CC1101.h"

namespace ArduForge{
    
    uint8_t PaTable[8] = {0x12, 0x0E, 0x1D, 0x34, 0x60, 0x84, 0xC8, 0xC0};

    CC1101::CC1101(void){
        m_Mosi = 0x00;
        m_Miso = 0x00;
        m_Csn = 0x00;
        m_Sck = 0x00;
        m_Gdo0 = 0x00;
        m_Gdo2 = 0x00;

    }//Constructor 

    CC1101::~CC1101(void){
        end();
    }//Destructor

    void CC1101::begin(uint8_t DeviceAddr, uint8_t Gdo0, uint8_t Freq, uint8_t Gdo2, uint8_t Csn,  uint8_t Mosi, uint8_t Miso, uint8_t Sck){
        end();
        m_Mosi = Mosi;
        m_Miso = Miso;
        m_Csn = Csn;
        m_Sck = Sck;
        m_Gdo0 = Gdo0;
        m_Gdo2 = Gdo2;

        // initialize internal buffer 
        m_AvailableData = 0;
        
        spiInit();  // spi initialization
        pinMode(m_Gdo0, INPUT);
        //pinMode(m_Gdo2, INPUT); // currently unused
        digitalWrite(m_Csn, HIGH);
        digitalWrite(m_Sck, HIGH);
        digitalWrite(m_Mosi, LOW);
        reset(); // reset device
        regConfigSettings(Freq); // CC1101 register config
        spiWriteRegBurst(Reg::PATABLE, PaTable, 8); // CC1101 PATABLE configuration
        deviceAddress(DeviceAddr); // set device address
        outputPowerLevel(0); // 0 dBm output power level default
    }//begin

    void CC1101::end(void){
        m_AvailableData = 0;
        spiEnd();
        m_Mosi = 0x00;
        m_Miso = 0x00;
        m_Csn = 0x00;
        m_Sck = 0x00;
        m_Gdo0 = 0x00;
        m_Gdo2 = 0x00;      
    }//end

    void CC1101::broadcast(uint8_t *pData, uint8_t Size){
        sendData(BroadcastAdr, pData, Size);
    }//sendData 

    void CC1101::sendData(uint8_t Receiver, uint8_t *pData, uint8_t Size){
        // put in idle 
        spiStrobe(CmdStrobes::SIDLE);
        delayMicroseconds(10);  
        spiWriteReg(Reg::TXFIFO, Size + 3); // number of bytes to send (payload size + receiver address  + sender address)     
        spiWriteReg(Reg::TXFIFO, Receiver);
        spiWriteReg(Reg::TXFIFO, deviceAddress());
        spiWriteReg(Reg::TXFIFO, Size); // byte count of payload
        spiWriteRegBurst(Reg::TXFIFO, pData, Size);
        spiStrobe(CmdStrobes::STX);
        while(!digitalRead(m_Gdo0));
        while(digitalRead(m_Gdo0));
        spiStrobe(CmdStrobes::SFTX);
    }//sendData

    void CC1101::receiveMode(void){
        spiStrobe(CmdStrobes::SRX);
        m_AvailableData = 0;
    }//receive 

    uint8_t CC1101::dataAvailable(void){
        if(digitalRead(m_Gdo0)){
            while(digitalRead(m_Gdo0));     
            if(spiReadStatus(Reg::RXBYTES) & Fifo::BYTES_IN_RXFIFO) m_AvailableData = spiReadReg(Reg::RXFIFO) & 0x7F;
        }
        return m_AvailableData;
    }//checkReceiveFlag 

    uint8_t CC1101::receiveData(uint8_t *pData, uint8_t *pReceiver, uint8_t *pSender, int8_t *pRSSI){     
        uint8_t Rval = 0;

        if(m_AvailableData > 0){
            uint8_t PkgHeader[3];

            spiReadRegBurst(Reg::RXFIFO, PkgHeader, 3); // read package header
            if(nullptr != pReceiver) (*pReceiver) = PkgHeader[0];
            if(nullptr != pSender) (*pSender) = PkgHeader[1];
            Rval = PkgHeader[2];
            spiReadRegBurst(Reg::RXFIFO, pData, Rval); // read payload
            if(nullptr != pRSSI) (*pRSSI) = rssiConvert(spiReadReg(Reg::RXFIFO));

            m_AvailableData = 0;
        }

        // flush RX buffer
        spiStrobe(CmdStrobes::SFRX);

        return Rval;
    }//receiveData 

    CC1101::Error CC1101::lastError(bool Clear){
        CC1101::Error Rval = m_LastError;
        if(Clear) m_LastError = Error::NO_ERROR;
        return Rval;
    }//LastError


    void CC1101::deviceAddress(uint8_t Addr){
        if(0x00 == Addr || 0xFF == Addr){
            m_LastError = Error::INVALID_DEVICE_ADR;
        }else{
            spiWriteReg(Config::ADDR, Addr);
        }
    }//deviceAddress 

    uint8_t CC1101::deviceAddress(void)const{
        return spiReadReg(Config::ADDR);
    }//deviceAddress

    void CC1101::channel(uint8_t Channel){
        spiWriteReg(Config::CHANNR, Channel);
    }//channel

    uint8_t CC1101::channel(void){
        return spiReadReg(Config::CHANNR);
    }//channel

    void CC1101::outputPowerLevel(int8_t dBm){
        uint8_t pa = 0xC0;

        if(dBm <= -30) pa = 0x00;
        else if(dBm <= -20) pa = 0x01;
        else if(dBm <= -15) pa = 0x02;
        else if(dBm <= -10) pa = 0x03;
        else if(dBm <= 0) pa = 0x04;
        else if(dBm <= 5) pa = 0x05;
        else if(dBm <= 7) pa = 0x06;
        else if(dBm <= 10) pa = 0x07;

        spiWriteReg(Config::FREND0, pa);
    }//outputPowerLevel

    int8_t CC1101::outputPowerLevel(void){
        int8_t Rval = 0;
        switch(spiReadReg(Config::FREND0)){
            case 0x00: Rval = -30; break;
            case 0x01: Rval = -20; break;
            case 0x02: Rval = -15; break;
            case 0x03: Rval = -10; break;
            case 0x04: Rval = 0; break;
            case 0x05: Rval = 5; break;
            case 0x06: Rval = 7; break;
            case 0x07: Rval = 10; break;
            default: break;
        }//switch[Pa table setting]

        return Rval;
    }//outputPowerLevel

    void CC1101::powerDown(void){
        spiStrobe(CmdStrobes::SIDLE);
        spiStrobe(CmdStrobes::SPWD);
    }//powerDown

    void CC1101::wakeUp(void){
        digitalWrite(m_Csn, LOW);
        delayMicroseconds(10);
        digitalWrite(m_Csn, HIGH);
        delayMicroseconds(10);
        receiveMode();
    }//wakeUp


    void CC1101::spiInit(void){
        // Initializes the SPI pins 
        pinMode(m_Sck, OUTPUT);
        pinMode(m_Mosi, OUTPUT);
        pinMode(m_Miso, INPUT);
        pinMode(m_Csn, OUTPUT);

        // enable SPI Master, MSB, SPI mode0, FOSC/4
        spiMode(0);
    }//spiInit 

    void CC1101::spiEnd(void){
        pinMode(m_Sck, INPUT);
        pinMode(m_Mosi, INPUT);
        pinMode(m_Csn, INPUT);
    }//spiEnd

    void CC1101::spiMode(uint8_t Config){
        // enable SPI master with configuration specified by Config byte
        SPCR = 0;
        SPCR = (Config & 0x7F) | (1<<SPE) | (1<<MSTR);
    }//spiMode 

    uint8_t CC1101::spiTransfer(uint8_t Value)const{ 
        SPDR = Value;
        while(!(SPSR & (1<<SPIF)));
        return SPDR;
    }//spiTransfer 

    void CC1101::reset(void){
        digitalWrite(m_Csn, LOW);
        delay(1);
        digitalWrite(m_Csn, HIGH);
        delay(1);
        digitalWrite(m_Csn, LOW);
        while(digitalRead(m_Miso));
        spiTransfer(CmdStrobes::SRES);
        while(digitalRead(m_Miso));
        digitalWrite(m_Csn, HIGH);

    }//reset 

    void CC1101::spiWriteReg(uint8_t Reg, uint8_t Value){
        digitalWrite(m_Csn, LOW);
        while(digitalRead(m_Miso));
        spiTransfer(Reg);
        spiTransfer(Value);
        digitalWrite(m_Csn, HIGH);

    }//spiWriteReg 

    void CC1101::spiWriteRegBurst(uint8_t Reg, uint8_t *pData, uint8_t Count){
        uint8_t Tmp = Reg | Fifo::WRITE_BURST;
        digitalWrite(m_Csn, LOW);
        while(digitalRead(m_Miso));
        spiTransfer(Tmp);
        for(uint8_t i = 0; i < Count; ++i) spiTransfer(pData[i]);
        digitalWrite(m_Csn, HIGH);

    }//spiWriteBurst 

    void CC1101::spiStrobe(uint8_t Cmd){
        digitalWrite(m_Csn, LOW);
        while(digitalRead(m_Miso));
        spiTransfer(Cmd);
        digitalWrite(m_Csn, HIGH);
    }//spiStrobe 

    uint8_t CC1101::spiReadReg(uint8_t Reg)const{
        uint8_t Rval = 0;

        digitalWrite(m_Csn, LOW);
        while(digitalRead(m_Miso));
        spiTransfer(Reg | Fifo::READ_SINGLE);
        Rval = spiTransfer(0);
        digitalWrite(m_Csn, HIGH);
        return Rval;
    }//spiReadReg 

    void CC1101::spiReadRegBurst(uint8_t Reg, uint8_t *pData, uint8_t Count){
        digitalWrite(m_Csn, LOW);
        while(digitalRead(m_Miso));
        spiTransfer(Reg | Fifo::READ_BURST);
        for(uint8_t i=0; i < Count; ++i) pData[i] = spiTransfer(0);
        digitalWrite(m_Csn, HIGH);
    }//spiReadRegBurst 

    uint8_t CC1101::spiReadStatus(uint8_t Addr){
        uint8_t Rval = 0;
        digitalWrite(m_Csn, LOW);
        while(digitalRead(m_Miso));
        spiTransfer(Addr | Fifo::READ_BURST);
        Rval = spiTransfer(0);
        digitalWrite(m_Csn, HIGH);
        return Rval;
    }//spiReadStatus 

    void CC1101::regConfigSettings(uint8_t Freq){
        spiWriteReg(Config::FSCTRL1, 0x08);
        spiWriteReg(Config::FSCTRL0, 0x00);

        switch(Freq){
            case Frequency::F_868:
                spiWriteReg(Config::FREQ2, Frequency::F2_868);
                spiWriteReg(Config::FREQ1, Frequency::F1_868);
                spiWriteReg(Config::FREQ0, Frequency::F0_868);
                break;
            case Frequency::F_915:
                spiWriteReg(Config::FREQ2, Frequency::F2_915);
                spiWriteReg(Config::FREQ1, Frequency::F1_915);
                spiWriteReg(Config::FREQ0, Frequency::F0_915);
                break;
            case Frequency::F_433:
                spiWriteReg(Config::FREQ2, Frequency::F2_433);
                spiWriteReg(Config::FREQ1, Frequency::F1_433);
                spiWriteReg(Config::FREQ0, Frequency::F0_433);
                break;
            default:
                spiWriteReg(Config::FREQ2, Frequency::F2_433);
                spiWriteReg(Config::FREQ1, Frequency::F1_433);
                spiWriteReg(Config::FREQ0, Frequency::F0_433);
                break;
        }// switch[frequency config]

        spiWriteReg(Config::MDMCFG4, 0x5B);
        spiWriteReg(Config::MDMCFG3, 0xF8);
        spiWriteReg(Config::MDMCFG2, 0x03);
        spiWriteReg(Config::MDMCFG1, 0x22);
        spiWriteReg(Config::MDMCFG0, 0xF8);
        spiWriteReg(Config::CHANNR, 0x00);
        spiWriteReg(Config::DEVIATN, 0x47);
        spiWriteReg(Config::FREND1, 0xB6);
        spiWriteReg(Config::FREND0, 0x10);
        spiWriteReg(Config::MCSM0, 0x18);
        spiWriteReg(Config::FOCCFG, 0x1D);
        spiWriteReg(Config::BSCFG, 0x1C);
        spiWriteReg(Config::AGCCTRL2, 0xC7);
        spiWriteReg(Config::AGCCTRL1, 0x00);
        spiWriteReg(Config::AGCCTRL0, 0xB2);
        spiWriteReg(Config::FSCAL3, 0xEA);
        spiWriteReg(Config::FSCAL2, 0x2A);
        spiWriteReg(Config::FSCAL1, 0x00);
        spiWriteReg(Config::FSCAL0, 0x11);
        spiWriteReg(Config::FSTEST, 0x59);
        spiWriteReg(Config::TEST2, 0x81);
        spiWriteReg(Config::TEST1, 0x35);
        spiWriteReg(Config::TEST0, 0x09);
        spiWriteReg(Config::IOCFG2, 0x0B); // Serial clock synchronous to the data in synchronous serial mode 
        spiWriteReg(Config::IOCFG0, 0x06); // asserts when sync word has been sent/received, and de-asserts at the end of the packet
        spiWriteReg(Config::PKTCTRL1, 0b00000111); // two status bytes will be appended to the payload of the packet, including LQI and CRC OK, Address check and (0x00 & 0xFF Broadcast)
        spiWriteReg(Config::PKTCTRL0, 0x05); // whitening off, CRC enable variable length packets, packet length configured by the first byte after sync word 
        spiWriteReg(Config::ADDR, 0x00); // address used for packet filtration, set default to broadcast address 0x00
      //  spiWriteReg(Config::PKTLEN, 64); // 64 bytes max length (payload), default length | we use variable packet length and don't use this field

    }//regConfigSettings

    int8_t CC1101::rssi(void){
        return rssiConvert(spiReadReg(Reg::RSSI));
    }//rssi 

    int8_t CC1101::rssiConvert(uint8_t RssiValueDec){
        int16_t RssiDec = RssiValueDec; // from unsigned to signed 
        if(RssiDec >= 128) RssiDec -= 256;
        int8_t RssiDbm = (RssiDec/2) - 74; // 74 is offset value for all data rates and frequency according to datasheet
        return RssiDbm;
    }//rssiConvert

    uint8_t CC1101::lqi(void){
        return (spiReadReg(Reg::LQI) & 0x7F);
    }//lqi

    uint8_t CC1101::partNumber(void)const{
        return spiReadReg(Reg::PARTNUM);
    }//partNumber

    uint8_t CC1101::version(void)const{
        return spiReadReg(Reg::VERSION);
    }//version




}//name space