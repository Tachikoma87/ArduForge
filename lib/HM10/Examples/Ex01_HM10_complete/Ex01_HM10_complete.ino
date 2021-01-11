/*****************************************************************************\
*                                                                           *
* File(s): EX01_HM10_complete.ino                                           *
*                                                                           *
* Content: This example sketch shows how to use the HM10 class to perform   *
*          interaction with an HM- device.                                  *
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
#include <Arduino.h>
#include <HM10.h>

ArduForge::HM10 BTLE;
/**
 * The example sketch shows how to use the ArduForge::HM10 class for the interaction with an HM-10 BTLE device. 
 * To test the code you can install the Serial Bluetooth Terminal app on your smart phone or tablet to establish a bluetooth connection to your HM-10 module.
 */

// if 0 auto detect is started
// once you know your device's baudrate you should define it here to accelerate startup
uint32_t Baudrate = 0; 
const uint8_t TXPin = 2; // TX pin on the receiving device (Connected to RX pin on HM10 module)
const uint8_t RXPin = 3; // RX pin on the receiving device (connected to TX pin on HM10 module)
const uint8_t StatePin = 4; // state pin is used to check bluetooth connection status
const char DeviceName[] = "ArduForgeBTLE";
// last action in startup routine is printing the help page of the HM10 module if this flag is set
const bool PrintHelpPage = true;

// internal usage to delay status update prints
uint32_t StatusTimeout = 0;

/**
 * \brief Queries an information from the BTLE module via UART and prints the response
 * 
 */
void printInfo(const char *pATCmd){
    char Buffer[48];
    memset(Buffer, 0, sizeof(Buffer));
    if(!BTLE.connected() && 0 < BTLE.issueATCommand(pATCmd, (uint8_t*)Buffer)){
        Serial.write(Buffer);
    }
}

/**
 * \brief Queries the help page from the BTLE module and prints it to serial. Works only if device has no bluetooth connection yet.
 */
void printHelp(void){
    if(BTLE.connected()) return;
    BTLE.issueATCommand("AT+HELP\r\n");
    int32_t Start = millis();
    while(millis() - Start < 5000){
        if(BTLE.available()) Serial.write(BTLE.read());
    } 
}//printHelp

void setup() {
    // initialize serial connection
    Serial.begin(115200);

    // if baudrate is set to zero we start auto discovery
    if(Baudrate == 0){
        Baudrate = ArduForge::HM10::discoverBaud(RXPin, TXPin);

        if(Baudrate == 0) Serial.println("No UART device responding on any baudrate!");
        else {
            Serial.print("UART device responding at baudrate: "); Serial.println(Baudrate);
        }    
    } 

    // initialize BTLE module
    BTLE.begin(RXPin, TXPin, StatePin, DeviceName, true, Baudrate);

    char Reply[32];
    // change role to slave (ROLE0)
    if(!BTLE.connected()) BTLE.issueATCommand("AT+ROLE0\r\n", (uint8_t*)Reply);

    // print some information
    printInfo("AT\r\n"); // should return "OK"
    printInfo("AT+VERSION\r\n");
    printInfo("AT+NAME\r\n");
    printInfo("AT+PIN\r\n");
    printInfo("AT+ADDR\r\n");

    if(PrintHelpPage) printHelp();
}

void loop() {
    // print status update every 10 seconds
    // we tell the connected device how long the server is already up and running
    // and we print to serial the connection status of the BTLE module (connected or not connected)
    if(millis() - StatusTimeout > 10000){
        StatusTimeout = millis();
        BTLE.print("Server up time: "); BTLE.print<int32_t>(StatusTimeout/1000); BTLE.print(" seconds\n");
        Serial.print("Module Status: "); Serial.print( (BTLE.connected()) ? "connected\n" : "not connected\n");
    }

    // if there is data available we read it from the serial input and assume it is a number
    // then we build the squared value and return the result to the BTLE module
    if(BTLE.available()){
        uint8_t Buffer[32];
        BTLE.read(Buffer, BTLE.available());
        int32_t Value = 0;
        sscanf((char*)Buffer, "%ld", &Value);
        Serial.print("Received number: "); Serial.print(Value); Serial.println();
        if(Value != 0){
            sprintf((char*)Buffer, "%ld * %ld = %ld\n", Value, Value, Value*Value);
            BTLE.print((char*)Buffer);
        }
    }

    delay(50);
}//loop