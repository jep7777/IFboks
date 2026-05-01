#include "interfaceboksControl.h"

InterfaceboksControl::InterfaceboksControl
    (LCD_displayIF& display, State& currentState, volatile bool* sendRequestFlag) :
            display_(display), currentState_(currentState),
            settings_(), showerValues_(), sendRequestFlag_(sendRequestFlag),
            subScreenIndex_(0), cursorIndex_(0) {}


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
    display_.displayShowerScreen0(1, settings_.getMaxWater(), showerValues_.getLatestTemp());
    display_.displayCursor(cursorIndex_);
}

void InterfaceboksControl::updateSubScreen(){
   if(subScreenIndex_ != previousSubScreenIndex_){

        if(subScreenIndex_ == 0){
            display_.displayShowerScreen0(1.1, settings_.getMaxWater(), showerValues_.getLatestTemp());
        }
        else if(subScreenIndex_ == 1){
            display_.displayShowerScreen1(1, settings_.getMaxEnergy());
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

void InterfaceboksControl::checkSendRequestFlag(){
    noInterrupts();
    if(*sendRequestFlag_ && !awaitingReadingFlag_){
        *sendRequestFlag_ = false;
        display_.test();
    }
    interrupts();
}

void InterfaceboksControl::measurementSequence(){
    //TODO! lav det her
}