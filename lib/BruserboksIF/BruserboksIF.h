#pragma once
#include <Arduino.h>

class BruserboksIF{
    public:

    void requestReading();
    void readToBuffer();
    bool checkReadingReadyFlag();
    void resetReadingReadyFlag();
    String getReading(); //apparently Arduino has a String class

    private:
    String buffer;
    bool readingReadyFlag;
};