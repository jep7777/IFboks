#include "BruserboksIF.h"

BruserboksIF::BruserboksIF(HardwareSerial& uart1) : 
    uart1_(uart1), readingReadyFlag_(false) {}

void BruserboksIF::begin(unsigned long baud){
    uart1_.begin(baud, SERIAL_8N1, 16, 18); //16RX, 18TX
}

void BruserboksIF::emptyHWBuffer(){
    
        //code for emptying hardware buffer of pin used to recieve from Mega2560
        
        //for USB testing
        while(Serial.available() > 0){
            Serial.read();  // read and discard
        } 

        
        //for testing with mega2560
        while(uart1_.available() > 0){
            uart1_.read();  // read and discard
        } 


    }

void BruserboksIF::readToBuffer(){

    //code for reading from HW buffer into buffer.
    while(uart1_.available() > 0){
        char c = uart1_.read();

        // if next char is '\0', an entire string has been recieved
        if(c == '\0'){
            buffer_[bufferIndex_] = '\0'; //null-terminate
            readingReadyFlag_ = true; //signal that an entire reading is ready
            bufferIndex_ = 0; //reset for next message
            break;
        }
        if(bufferIndex_ < sizeof(buffer_) - 1){
            buffer_[bufferIndex_++] = c; //append char to buffer
        }
    }


/* 
    //for testing, read from hardcoded strings

    unsigned long now = millis(); //get current time
    static unsigned long timeAtLastIndexUpdate = millis();

    if((now - timeAtLastIndexUpdate) > 2000){ //check if 2 seconds have passed
        strncpy(buffer_, testData_[testIndex_], sizeof(buffer_) - 1); //copy testData into buffer_
        buffer_[sizeof(buffer_) - 1] = '\0'; //terminate string

        if(testIndex_ < (sizeof(testData_) / sizeof(testData_[0]) - 1)){
            testIndex_++; //increment testIndex as long as theres strings left in testData
        }

        readingReadyFlag_ = true;
        timeAtLastIndexUpdate = millis();
    }

     
 */


}

bool BruserboksIF::checkReadingReadyFlag(){
    return readingReadyFlag_;
}

void BruserboksIF::resetReadingReadyFlag(){
    readingReadyFlag_ = false;
}

const char* BruserboksIF::getReading(){

    return buffer_;
}