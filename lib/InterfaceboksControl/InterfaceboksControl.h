#pragma once
#include <Arduino.h>
#include <exception>
#include "LCD_displayIF.h"
#include "Brugerindstillinger.h"
#include "Badanalyse.h"
#include "utility.h"

class InterfaceboksControl{

    public:

        InterfaceboksControl(LCD_displayIF&, State&, volatile bool*);

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

        void checkSendRequestFlag();
        void measurementSequence();

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

        volatile bool* sendRequestFlag_; //set if new request is to be sent
        volatile bool* readingReadyFlag_; //set is reading is ready to be read/parsed
        bool awaitingReadingFlag_; //set while waiting for reading

};  