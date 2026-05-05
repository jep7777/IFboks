#pragma once
#include <Arduino.h>

class BruserboksIF{
    public:

    BruserboksIF();

    void emptyHWBuffer();
    void readToBuffer();
    bool checkReadingReadyFlag();
    void resetReadingReadyFlag();
    const char* getReading();

    private:
    char buffer_[32]{};
    bool readingReadyFlag_;

    //for unit test of ESP32
    const char* testData_[10] = {
        "F2.20 0.56 0.1",
        "F2.15 0.62 20.2",
        "F2.25 0.68 20.3",
        "F2.30 0.75 20.4",
        "F2.10 0.82 20.5",
        "F1.95 0.95 19.6",
        "F2.40 1.15 20.7",
        "F2.05 1.30 20.8",
        "F2.50 1.50 20.9",
        "F2.60 1.75 21.1"
    };
    size_t testIndex_ = 0;

};