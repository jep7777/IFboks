#include "TrykknapperIF.h"

TrykknapperIF::TrykknapperIF(volatile uint8_t& buttonStatus, InterfaceboksControl& control) : 
            buttonFlags_(buttonStatus), control_(control) {}


//returns true, if the flag corresponding to the argument is set
bool TrykknapperIF::checkButtonFlag(uint8_t flagToCheck){
    noInterrupts();
    bool isSet = false;
    if(buttonFlags & flagToCheck){
        isSet = true;
    }
    //only reset the flag that was checked
    buttonFlags &= ~flagToCheck;
    interrupts();
    return isSet;
}

InterfaceboksControl& TrykknapperIF::getControlClass(){
    return control_;
}
