#include "BruserboksIF.h"

void BruserboksIF::requestReading(){
        //insert code for sending a reading request to 2560 with UART
        
        //for now, this code for testing without 2560
        readingReadyFlag = true;
    }

bool BruserboksIF::checkReadingReadyFlag(){
    return readingReadyFlag;
}

void BruserboksIF::resetReadingReadyFlag(){
    readingReadyFlag = false;
}

String BruserboksIF::getReading(){
    //insert code for getting a reading from 2560 with UART
        
    //for now, this code for testing without 2560
    static double volumeReading = 0.0;
    static double tempReading = 0.0;

    volumeReading += 0.1;
    tempReading += 0.1;
    String toReturn = String(volumeReading, 4) + " " + String(tempReading, 4);
    return toReturn;
}