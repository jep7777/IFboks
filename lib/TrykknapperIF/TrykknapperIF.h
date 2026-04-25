#pragma once
#include <Arduino.h>
#include "InterfaceboksControl.h"



class TrykknapperIF{
    public:

        TrykknapperIF(volatile uint8_t&, InterfaceboksControl&);

        bool checkButtonFlag(uint8_t);

        InterfaceboksControl& getControlClass();
    private:
        //pointer to buttonStatus, which is defined in "utility.h"
        volatile uint8_t& buttonFlags_;
        InterfaceboksControl& control_;


};