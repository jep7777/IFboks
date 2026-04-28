#include "interfaceboksControl.h"

InterfaceboksControl::InterfaceboksControl
    (LCD_displayIF& display, State& currentState) :
            display_(display), currentState_(currentState),
            settings_(), subMenuIndex_(0), cursorIndex_(0) {}


int InterfaceboksControl::maxCursorIndex(){
       switch(currentState_) {
                case State::MAIN_MENU:
                    return 1;
                case State::SETTINGS:
                    return 3;
                case State::SHOWER_RUNNING:
                    return 7;
                default:
                    throw std::runtime_error("Something went wrong");
            }

}

void InterfaceboksControl::incCursorIndex(){
    if(cursorIndex_ < maxCursorIndex()){
        cursorIndex_++;
    }
    //if index goes from 3 to 4, change subMenuIndex
    if(cursorIndex_ == 4){
        subMenuIndex_ = 1;
        changeSubMenu();
    }
    
    
    display_.displayCursor(cursorIndex_);
}


void InterfaceboksControl::decCursorIndex(){
if(cursorIndex_ > 0){
        cursorIndex_--;
    }

    //if index goes from 4 to 3, change subMenuIndex
    if(cursorIndex_ == 3){
        subMenuIndex_ = 0;
        changeSubMenu();
    }
    

    display_.displayCursor(cursorIndex_);
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
    subMenuIndex_ = 0;
    display_.displayMainMenu();
    display_.displayCursor(cursorIndex_);
}

void InterfaceboksControl::openSettingsMenu(){
    cursorIndex_ = 0;
    subMenuIndex_ = 0;
    display_.displaySettingsMenu(settings_.getMaxWater(), settings_.getMaxEnergy());
    display_.displayCursor(cursorIndex_);
}

void InterfaceboksControl::startShower(){
    cursorIndex_ = 0;
    subMenuIndex_ = 0;
    display_.displayShowerMenu(subMenuIndex_);
    display_.displayCursor(cursorIndex_);
}

void InterfaceboksControl::changeSubMenu(){
    display_.displayShowerMenu(subMenuIndex_);
}