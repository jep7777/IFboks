#include "LCD_displayIF.h"
#include <Arduino.h>

//constructor is to take Serial2, so it uses uart
LCD_displayIF::LCD_displayIF(HardwareSerial& uart) : 
    uart_(uart), lcd_() {}

void LCD_displayIF::begin(unsigned long baud){
    uart_.begin(baud);
    lcd_.begin(uart_);
}

void LCD_displayIF::displayCursor(int cursorIndex){
    //clear last column
    lcd_.setCursor(19,0); lcd_.print(" ");
    lcd_.setCursor(19,1); lcd_.print(" ");
    lcd_.setCursor(19,2); lcd_.print(" ");
    lcd_.setCursor(19,3); lcd_.print(" ");

    if(cursorIndex > 3){
        cursorIndex -= 4;
    }
    lcd_.setCursor(19,cursorIndex);
    lcd_.print("<");
}

void LCD_displayIF::displayMainMenu(){
    
    lcd_.clear();
    lcd_.setCursor(0,0);
    lcd_.print("Start bad");
    lcd_.setCursor(0,1);
    lcd_.print("Indstillinger");
}

void LCD_displayIF::displaySettingsMenu(int maxWater, double maxEnergy){
    lcd_.clear();
    lcd_.setCursor(0,0);
    lcd_.print("Max vand:   "); lcd_.print(maxWater); lcd_.print("L");

    lcd_.setCursor(0,1);
    lcd_.print("Max energi: "); lcd_.print(maxEnergy, 1); lcd_.print("kWh");

    lcd_.setCursor(0,2);
    lcd_.print("Gem");
    lcd_.setCursor(0,3);
    lcd_.print("Annuller");
}

void LCD_displayIF::updateSettingsMenuWater(int maxWater){
    lcd_.setCursor(12,0);
    lcd_.print("    "); //to clear the space
    lcd_.setCursor(12,0);
    lcd_.print(maxWater); lcd_.print("L");
}

void LCD_displayIF::updateSettingsMenuEnergy(double maxEnergy){
    lcd_.setCursor(12,1);
    lcd_.print("       "); //to clear the space
    lcd_.setCursor(12,1); 
    lcd_.print(maxEnergy, 1); lcd_.print("kWh");
}

void LCD_displayIF::displayCancelMessage(int maxWater, double maxEnergy){
    lcd_.clear();
    lcd_.setCursor(0,0);
    lcd_.print("Max vand:   "); lcd_.print(maxWater); lcd_.print("L");

    lcd_.setCursor(0,1);
    lcd_.print("Max energi: "); lcd_.print(maxEnergy, 1); lcd_.print("kWh");

    lcd_.setCursor(4,2);
    lcd_.print("Aendringer");
    lcd_.setCursor(4,3);
    lcd_.print("annulleret!");

    delay(2000);
}

void LCD_displayIF::displaySaveMessage(int maxWater, double maxEnergy){
    lcd_.clear();
    lcd_.setCursor(0,0);
    lcd_.print("Max vand:   "); lcd_.print(maxWater); lcd_.print("L");

    lcd_.setCursor(0,1);
    lcd_.print("Max energi: "); lcd_.print(maxEnergy, 1); lcd_.print("kWh");

    lcd_.setCursor(4,2);
    lcd_.print("Aendringer");
    lcd_.setCursor(4,3);
    lcd_.print("gemt!");

    delay(2000);
}

void LCD_displayIF::displayShowerMenu(int subMenuIndex){
    lcd_.clear();
    if(subMenuIndex == 0){
        lcd_.setCursor(0,0);
        lcd_.print("Bad startet!");
        lcd_.setCursor(0,2);
        lcd_.print("Submenu 1");
        lcd_.setCursor(0,4);
        lcd_.print("vvvvvvvvvvvvvvvvvvv");
    }
    else if(subMenuIndex == 1){
        lcd_.setCursor(0,0);
        lcd_.print("^^^^^^^^^^^^^^^^^^^");
        lcd_.setCursor(0,2);
        lcd_.print("Submenu 2");
    }
  
    

}