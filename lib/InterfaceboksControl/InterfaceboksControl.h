#pragma once
#include <Arduino.h>
#include <exception>
#include "LCD_displayIF.h"
#include "Brugerindstillinger.h"
#include "Badanalyse.h"
#include "BruserboksIF.h"
#include "utility.h"

class InterfaceboksControl{

    public:

        InterfaceboksControl(LCD_displayIF&, State&);

        State& currentState_;

        //increment/decrement cursorIndex, based on maxCursorIndex
        void incCursorIndex();
        void decCursorIndex();

        void handleSelect();
        void handleDecrement();

        void openMainMenu();
        void openSettingsMenu();
        void startShower();
        void updateSubScreen();

       
        void measurementSequence();
        bool checkReadingValid(const char*);
        double parseTemperature(const char*);
        double parseVolume(const char*);
        double parseFlowRate(const char*);
        bool checkTempValid(double);
        bool checkNoFlowTimer(double);        //check if there has been no flow for 60 seconds
        void updateDisplayValues();

    private:
         //returns maximum cursor index based on state
        int maxCursorIndex();
        void updateSubScreenIndex();

        int subScreenIndex_; //to keep track of current subscreen during shower
        int previousSubScreenIndex_; //to check, if subscreen has changed
        int cursorIndex_; //to keep track of cursor position
        LCD_displayIF& display_; 
        Brugerindstillinger settings_;
        Badanalyse showerValues_;
        BruserboksIF bruserboksIF_;

};  