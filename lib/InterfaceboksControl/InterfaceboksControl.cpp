#include "interfaceboksControl.h"

InterfaceboksControl::InterfaceboksControl
    (LCD_displayIF& display, State& currentState) :
            display_(display), currentState_(currentState) {}


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
            display_.displayShowerScreen2(2.47,true);
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
                                    173.2, true);
}

void InterfaceboksControl::measurementSequence(){
    bruserboksIF_.readToBuffer();

    if(!bruserboksIF_.checkReadingReadyFlag()){
        return; //if no reading ready, exit function
    }


    //if reading ready, handle the reading
    bruserboksIF_.resetReadingReadyFlag();
    const char* currentReading = bruserboksIF_.getReading(); //pointer to string with readings to be parsed
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
    //maybe some checksum here
    return true;
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
        return((now - timeAtLastFlow_) > 6000); //returns true, if there as been no flow for 60 seconds
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