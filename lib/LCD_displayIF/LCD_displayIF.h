#pragma once
#include <SerLCD.h>

class LCD_displayIF {
    public:
        LCD_displayIF(HardwareSerial&);
        void begin(unsigned long);

        void displayMainMenu();

        void displaySettingsMenu(int, double);
        void updateWaterValue(int);
        void updateEnergyValue(double);
        void displayCancelMessage(int, double);
        void displaySaveMessage(int, double);

        //skal ændres senere
        void displayShowerRunning(int);

        void displayCursor(int);




    private:
        HardwareSerial& uart_;
        SerLCD lcd_;


};
