#pragma once
#include <Arduino.h>
#include <exception>
#include "LCD_displayIF.h"
#include "Brugerindstillinger.h"
#include "utility.h"

class InterfaceboksControl{

    public:

        InterfaceboksControl(LCD_displayIF&, State&);

        //increment/decrement cursorIndex, based on maxCursorIndex
        void incCursorIndex();
        void decCursorIndex();

        void handleSelect();
        void handleDecrement();

        void openMainMenu();
        void openSettingsMenu();
        void openShowerRunning();
        void changeSubMenu();

    private:
         //returns maximum cursor index based on state
        int maxCursorIndex();
        
        int subMenuIndex_; //to keep track of current submenu
        int cursorIndex_; //to keep track of cursor position
        LCD_displayIF& display_; 
        State& currentState_;
        Brugerindstillinger settings_;
};  