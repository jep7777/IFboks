#include "interfaceboksControl.h"

InterfaceboksControl::InterfaceboksControl (LCD_displayIF& display, State& currentState, BruserboksIF& bruserboksIF) :
            display_(display), currentState_(currentState), bruserboksIF_(bruserboksIF) {}


int InterfaceboksControl::maxCursorIndex(){
       switch(currentState_) {
                case State::MAIN_MENU:
                    return 1;
                case State::SETTINGS:
                    return 3;
                case State::SHOWER_RUNNING:
                    return 11;
                default:
                    throw std::runtime_error("Something went wrong");
            }
}

void InterfaceboksControl::updateSubScreenIndex(){
    if(0 <= cursorIndex_ && cursorIndex_ < 4){
        subScreenIndex_ = 0;
    }
    else if(4 <= cursorIndex_ && cursorIndex_ < 8){
        subScreenIndex_ = 1;
    }
    else if(8 <= cursorIndex_ && cursorIndex_ < 12){
        subScreenIndex_ = 2;
    }
    else{
        throw std::runtime_error("cursorIndex out of bounds");
    }
}

void InterfaceboksControl::incCursorIndex(){
    //If shower ended screen is showing, any button goes back to main menu
    if(currentState_ == State::SHOWER_ENDED){
        currentState_ = State::MAIN_MENU;
        openMainMenu();
        return;
    }

    if(cursorIndex_ < maxCursorIndex()){
        cursorIndex_++;
    }

    //if state is SHOWER_RUNNING, check if new subscreen should be displayed
    if(currentState_== State::SHOWER_RUNNING){
        updateSubScreenIndex();
        updateSubScreen();
    }
    else{
        display_.displayCursor(cursorIndex_);
    }
    
}


void InterfaceboksControl::decCursorIndex(){
    //If shower ended screen is showing, any button goes back to main menu
    if(currentState_ == State::SHOWER_ENDED){
        currentState_ = State::MAIN_MENU;
        openMainMenu();
        return;
    }

    if(cursorIndex_ > 0){
        cursorIndex_--;
    }

    //if state is SHOWER_RUNNING, check if new subscreen should be displayed
    if(currentState_== State::SHOWER_RUNNING){
        updateSubScreenIndex();
        updateSubScreen();
    }
    else{
        display_.displayCursor(cursorIndex_);
    }
    

    
}


void InterfaceboksControl::handleSelect(){
    switch(currentState_){

        case State::MAIN_MENU :
            if(cursorIndex_ == 0){
                currentState_ = State::SHOWER_RUNNING;
                startShower();
            }
            else{
                currentState_ = State::SETTINGS;
                openSettingsMenu();

            }
            break;

        case State::SETTINGS :
            if(cursorIndex_ == 0){
                settings_.incTempMaxWater();
                display_.updateSettingsMenuWater(settings_.getTempMaxWater());
            }
            else if(cursorIndex_ == 1){
                settings_.incTempMaxEnergy();
                display_.updateSettingsMenuEnergy(settings_.getTempMaxEnergy());
            }
            else if(cursorIndex_ == 2){
                currentState_ = State::MAIN_MENU;
                settings_.saveTempSettings();
                display_.displaySaveMessage(settings_.getMaxWater(), settings_.getMaxEnergy());
                openMainMenu();
            }
            else{
                currentState_ = State::MAIN_MENU;
                settings_.resetTempSettings();
                display_.displayCancelMessage(settings_.getMaxWater(), settings_.getMaxEnergy());
                openMainMenu();
            }
            break;

        case(State::SHOWER_ENDED) :
            currentState_ = State::MAIN_MENU;
            openMainMenu();
            break;


        default :
        {}
    }
}

void InterfaceboksControl::handleDecrement(){
    switch(currentState_){
        case(State::MAIN_MENU) :
            break;

        case(State::SETTINGS) :
            if(cursorIndex_ == 0){
                settings_.decTempMaxWater();
                display_.updateSettingsMenuWater(settings_.getTempMaxWater());
            }
            else if(cursorIndex_ == 1){
                settings_.decTempMaxEnergy();
                display_.updateSettingsMenuEnergy(settings_.getTempMaxEnergy());
            }
            break;

        case(State::SHOWER_ENDED) :
            currentState_ = State::MAIN_MENU;
            openMainMenu();
            break;

        default :
        {}
    }
}

void InterfaceboksControl::openMainMenu(){
    cursorIndex_ = 0;
    subScreenIndex_ = 0;
    display_.displayMainMenu();
    display_.displayCursor(cursorIndex_);
}

void InterfaceboksControl::openSettingsMenu(){
    cursorIndex_ = 0;
    subScreenIndex_ = 0;
    display_.displaySettingsMenu(settings_.getMaxWater(), settings_.getMaxEnergy());
    display_.displayCursor(cursorIndex_);
}

void InterfaceboksControl::startShower(){
    cursorIndex_ = 0;
    subScreenIndex_ = 0;
    showerValues_.resetValues();
    resetExceededFlags();
    resetTimeAtLastFlow();
    display_.displayShowerScreen0(showerValues_.getLatestFlowRate(),
                 showerValues_.getTotalWater(), settings_.getMaxWater(), showerValues_.getLatestTemp(), waterExceededFlag_);
    display_.displayCursor(cursorIndex_);
    bruserboksIF_.emptyHWBuffer();
}

void InterfaceboksControl::updateSubScreen(){
   if(subScreenIndex_ != previousSubScreenIndex_){
        
        if(subScreenIndex_ == 0){
            display_.displayShowerScreen0(showerValues_.getLatestFlowRate(),
                 showerValues_.getTotalWater(), settings_.getMaxWater(), showerValues_.getLatestTemp(), waterExceededFlag_);
        }
        else if(subScreenIndex_ == 1){
            display_.displayShowerScreen1(showerValues_.getTotalEnergy(), settings_.getMaxEnergy(), energyExceededFlag_);
        }
        else if(subScreenIndex_ == 2){
            display_.displayShowerScreen2(2.47,true); //hardcoded set energy price, hardcoded set bool for "is outdated"
        }
        else{
            throw std::runtime_error("subScreenIndex out of bounds");
        }
   }

   display_.displayCursor(cursorIndex_);
   previousSubScreenIndex_ = subScreenIndex_;

}

void InterfaceboksControl::exitShower(){
    currentState_ = State::SHOWER_ENDED;
    display_.displayShowerEnded(showerValues_.getTotalWater(), showerValues_.getTotalEnergy(), 
                                    2.47 * showerValues_.getTotalEnergy(), true); //hardcoded energy price 2.47
}

void InterfaceboksControl::measurementSequence(){
    bruserboksIF_.readToBuffer();

    if(!bruserboksIF_.checkReadingReadyFlag()){
        return; //if no reading ready, exit function
    }


    //if reading ready, handle the reading
    bruserboksIF_.resetReadingReadyFlag();
    const char* currentReading = bruserboksIF_.getReading(); //returns pointer to string with readings to be parsed

    if(!checkReadingValid(currentReading)){
        return; //if reading not valid, exit function
    }

    //parse and store temperature
    double temp = parseTemperature(currentReading);
    if(!checkTempValid(temp)){
        display_.displayInvalidTempWarning(temp);
        exitShower();
        return; //if temp invalid, exit function
    }
    showerValues_.updateLatestTemperature(temp);
    
    //parse and store volume and flowrate
    double volume = parseVolume(currentReading);
    if(checkNoFlowTimer(volume)){
        display_.displayNoFlowMessage();
        exitShower();
        return; //if no flow for too long, exit function
    }
    showerValues_.updateLatestVolume(volume);
    showerValues_.updateTotalWater();
    
    double flowRate = parseFlowRate(currentReading);
    showerValues_.updateLatestFlowRate(flowRate);

    //calculate and update total energy
    showerValues_.updateTotalEnergy();

    //check if max water is exceeded
    if(!waterExceededFlag_ &&
            settings_.checkMaxWaterExceeded(showerValues_.getTotalWater()))
            {
                setWaterExceededFlag();
                display_.displayWaterExceededWarning();

                //to get back to showerScreen 0
                cursorIndex_ = 0;
                subScreenIndex_ = 0;
                previousSubScreenIndex_ = 1;
                updateSubScreen();
    }

    //check if max energy is exceeded
    if(!energyExceededFlag_ &&
            settings_.checkMaxEnergyExceeded(showerValues_.getTotalEnergy()))
            {
                setEnergyExceededFlag();
                display_.displayEnergyExceededWarning();

                //to get back to showerScreen 1
                cursorIndex_ = 4;
                subScreenIndex_ = 1;
                previousSubScreenIndex_ = 0;
                updateSubScreen();
    }

    //update values on display at the end
    updateDisplayValues();
}



bool InterfaceboksControl::checkReadingValid(const char* reading){
    // Find colon separator
    const char* colonPos = strchr(reading, ':');
    if (!colonPos) {
        return false;  // no colon found
    }
    
    // Extract data part (before colon) into temporary buffer
    size_t dataLen = colonPos - reading; //only works because the two pointers points to chars in the same string
    char dataPart[50];
    if (dataLen >= sizeof(dataPart)) {
        return false;  // data too long
    }
    strncpy(dataPart, reading, dataLen); //copy reading into dataPart, but only dataLen number of chars
    dataPart[dataLen] = '\0';
    
    // Calculate checksum of data part
    unsigned char calculatedChecksum = calculateChecksum(dataPart);
    
    // Parse received checksum from hex digits after colon
    unsigned char receivedChecksum = 0;
    if (sscanf(colonPos + 1, "%hhx", &receivedChecksum) != 1) {
        //sscanf reads from colonPos + 1 (starts reading after colon),
        //the "x" means read hexadecimal numbers, the "hh" means convert into unsigned char
        //finally, it is written into receivedChecksum
        return false;  // failed to parse hex
    }
    
    // Compare checksums
    return (calculatedChecksum == receivedChecksum);
}

unsigned char InterfaceboksControl::calculateChecksum(const char* dataPart){
    unsigned char checksum = 0;

	while (*dataPart != '\0')
	{
		checksum = checksum ^ *dataPart;
		dataPart++;
	}

	return checksum;
}

double InterfaceboksControl::parseTemperature(const char* reading){
    double flowRate, volume, temp; //sscanf needs pointers to doubles, so double are declared here
    //sscanf returns number of successfully parsed values.
    //so success, if it returns three
    if(sscanf(reading, "F%lf %lf %lf", &flowRate, &volume, &temp) == 3){
        return temp;
    }
    return -1.0; //parse failed
}

double InterfaceboksControl::parseFlowRate(const char* reading){
    double flowRate, volume, temp;
    if(sscanf(reading, "F%lf %lf %lf", &flowRate, &volume, &temp) == 3){
        return flowRate;
    }
    return -1.0; // parse failed
}

double InterfaceboksControl::parseVolume(const char* reading){
    double flowRate, volume, temp;
    if(sscanf(reading, "F%lf %lf %lf", &flowRate, &volume, &temp) == 3){
        return volume;
    }
    return -1.0; // parse failed
}

bool InterfaceboksControl::checkTempValid(double temp){
    if(0.0 < temp && temp <=80.0){
        return true;
    }
    return false;
}

bool InterfaceboksControl::checkNoFlowTimer(double flowRate){
    unsigned long now = millis();

    if(flowRate == 0.0){
        return((now - timeAtLastFlow_) > 15000); //returns true, if there as been no flow for 15 seconds (accepttest time)
    }
    else if(flowRate > 0.0){
        timeAtLastFlow_ = now;
        return false;
    }
    else{
            throw std::runtime_error("invalid flowRate values");
    }
}

void InterfaceboksControl::resetTimeAtLastFlow(){
    timeAtLastFlow_ = millis();
}

void InterfaceboksControl::updateDisplayValues(){
    if(subScreenIndex_ == 0){
        display_.updateShowerScreen0(showerValues_.getLatestFlowRate(),
             showerValues_.getTotalWater(), showerValues_.getLatestTemp());
    }
    else if(subScreenIndex_ == 1){
        display_.updateShowerScreen1(showerValues_.getTotalEnergy());
    }
   
}

void InterfaceboksControl::setWaterExceededFlag(){
    waterExceededFlag_ = true;
}

void InterfaceboksControl::setEnergyExceededFlag(){
    energyExceededFlag_ = true;
}

void InterfaceboksControl::resetExceededFlags(){
    waterExceededFlag_ = false;
    energyExceededFlag_ = false;
}

State InterfaceboksControl::getCurrentState() const{
    return currentState_;
}