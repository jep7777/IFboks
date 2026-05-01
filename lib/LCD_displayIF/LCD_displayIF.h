#pragma once
#include <SerLCD.h>

class LCD_displayIF {
    public:
        LCD_displayIF(HardwareSerial&);
        void begin(unsigned long);

        void displayMainMenu();

        void displaySettingsMenu(int, double);
        void updateSettingsMenuWater(int);
        void updateSettingsMenuEnergy(double);
        void displayCancelMessage(int, double);
        void displaySaveMessage(int, double);

        void displayShowerScreen0(double, int, double); //display total flow and max flow
        void displayShowerScreen1(double, double); //display total energy and max energy
        void displayShowerScreen2(double, bool); //display energy price and status (is it outdated?)

        void displayCursor(int);

        //for testing timer interrupt
        void test();




    private:
        HardwareSerial& uart_;
        SerLCD lcd_;


};
