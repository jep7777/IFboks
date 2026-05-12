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

        void displayShowerScreen0(double, double, int, double, bool); //display flowrate, total water, max water and temp. Bool is true, if water exceeded
        void displayShowerScreen1(double, double, bool); //display total energy and max energy. Bool is true if energy exceeded
        void displayShowerScreen2(double, bool); //display energy price and status (is it outdated?)
        void updateShowerScreen0(double, double, double); //update flowrate, total water and temp
        void updateShowerScreen1(double); //update total energy
        void displayWaterExceededWarning();
        void displayEnergyExceededWarning();
        void displayShowerEnded(double, double, double, bool);

        void displayCursor(int);

        //for testing timer interrupt
        void test();




    private:
        HardwareSerial& uart_;
        SerLCD lcd_;


};
