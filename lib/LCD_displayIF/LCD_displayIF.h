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

        void displayShowerScreen0(double, double, int, double); //display flowrate, total water, max water and temp
        void displayShowerScreen1(double, double); //display total energy and max energy
        void displayShowerScreen2(double, bool); //display energy price and status (is it outdated?)
        void updateShowerScreen0(double, double, double); //update flowrate, total water and temp
        void updateShowerScreen1(double); //update total energy

        void displayCursor(int);

        //for testing timer interrupt
        void test();




    private:
        HardwareSerial& uart_;
        SerLCD lcd_;


};
