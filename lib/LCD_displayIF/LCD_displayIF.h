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

        void displayShowerScreen1(double, int); //display total flow and max flow
        void displayShowerScreen2(double, double); //display total energy and max energy
        void displayShowerScreen3(double, bool); //display energy price and status (is it outdated?)


        void displayCursor(int);




    private:
        HardwareSerial& uart_;
        SerLCD lcd_;


};
