#pragma once
#include <Arduino.h>

class BruserboksIF{
    public:
        //constructor is to take Serial1, which is of type HardwareSerial
        BruserboksIF(HardwareSerial&);
        //method begin takes BAUD rate
        void begin(unsigned long);

        void emptyHWBuffer();
        void readToBuffer();
        bool checkReadingReadyFlag();
        void resetReadingReadyFlag();
        const char* getReading();

    private:
        char buffer_[32]{};
        size_t bufferIndex_ = 0;
        bool readingReadyFlag_;
        HardwareSerial& uart1_;

    //for unit test of ESP32
        const char* testData_[10] = {
            "F2.20 0.561 0.12",
            "F2.15 0.622 -200.22",
            "F2.25 0.682 20.33",
            "F2.30 0.753 20.44",
            "F2.10 0.827 20.53",
            "F1.95 0.953 19.64",
            "F2.40 0.00 20.72",
            "F2.05 0.00 20.82",
            "F2.50 0.00 20.93",
            "F2.60 0.00 21.14"
        };
        size_t testIndex_ = 0;

};