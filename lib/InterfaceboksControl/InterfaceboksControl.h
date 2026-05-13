#pragma once
#include <Arduino.h>
#include <exception>
#include "LCD_displayIF.h"
#include "Brugerindstillinger.h"
#include "Badanalyse.h"
#include "BruserboksIF.h"

//enum for states
enum class State {MAIN_MENU, SETTINGS, SHOWER_RUNNING, SHOWER_ENDED};

class InterfaceboksControl{

    public:

        InterfaceboksControl(LCD_displayIF&, State&, BruserboksIF&);

        //increment/decrement cursorIndex, based on maxCursorIndex
        void incCursorIndex();
        void decCursorIndex();

        void handleSelect();
        void handleDecrement();

        void openMainMenu();
        void openSettingsMenu();
        void startShower();
        void updateSubScreen();
        void exitShower();
        

       
        void measurementSequence();
        bool checkReadingValid(const char*);
        unsigned char calculateChecksum(const char*);
        double parseTemperature(const char*);
        double parseVolume(const char*);
        double parseFlowRate(const char*);
        bool checkTempValid(double);
        bool checkNoFlowTimer(double);        //check if there has been no flow for 60 seconds
        void resetTimeAtLastFlow(); //called at start of each shower
        void updateDisplayValues();
        void setWaterExceededFlag();
        void setEnergyExceededFlag();
        void resetExceededFlags(); //called at start of each shower

        State getCurrentState() const;

    private:
         //returns maximum cursor index based on state
        int maxCursorIndex();
        void updateSubScreenIndex();

        int subScreenIndex_{0}; //to keep track of current subscreen during shower
        int previousSubScreenIndex_{0}; //to check, if subscreen has changed
        int cursorIndex_{0}; //to keep track of cursor position
        bool waterExceededFlag_{false}; //is set when total water exceeds maxWater
        bool energyExceededFlag_{false}; //is set when total energy exceeds maxEnergy
        unsigned long timeAtLastFlow_{0}; //used to check, if 60 seconds have passed witout flow
        LCD_displayIF& display_; 
        Brugerindstillinger settings_;
        State& currentState_;
        Badanalyse showerValues_;
        BruserboksIF& bruserboksIF_;

};  