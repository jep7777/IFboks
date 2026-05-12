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

    if(cursorIndex > 3 && cursorIndex <= 7){
        cursorIndex -= 4;
    }
    else if(cursorIndex > 7){
        cursorIndex -= 8;
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

void LCD_displayIF::displayShowerScreen0(double flowRate, double totalWater, int maxWater, double latestTemp, bool maxWaterExceeded){
    lcd_.clear();
    lcd_.setCursor(0,0);
    lcd_.print("Flowrate:   "); lcd_.print(flowRate, 1); lcd_.print("L/m");

    lcd_.setCursor(0,1);
    lcd_.print("Totalt:     "); lcd_.print(totalWater,1); lcd_.print("L");

    lcd_.setCursor(0,2);
    if(maxWaterExceeded){
        lcd_.print("Maks.:OVER! "); lcd_.print(maxWater); lcd_.print("L");
    }
    else{
        lcd_.print("Maks.:      "); lcd_.print(maxWater); lcd_.print("L");
    }
    
    lcd_.setCursor(0,3);
    lcd_.print("Temp.:      "); lcd_.print(latestTemp, 1); 
    
    //where ".." is printed depends on size of temp
    if(latestTemp < 10.0){
        lcd_.print("C ..");
    }
    else{
        lcd_.print("C..");
    }

}

void LCD_displayIF::displayShowerScreen1(double totalEnergy, double maxEnergy, bool maxEnergyExceeded){
    lcd_.clear();
    lcd_.setCursor(0,0);
    lcd_.print("                 ..");

    lcd_.setCursor(0,1);
    lcd_.print("En.forbr.: "); lcd_.print(totalEnergy, 2); lcd_.print("kWh");

    lcd_.setCursor(0,2);
    if(maxEnergyExceeded){
        lcd_.print("Maks.:OVER!"); lcd_.print(maxEnergy, 1); lcd_.print("kWh");
    }
    else{
        lcd_.print("Maks.:     "); lcd_.print(maxEnergy, 1); lcd_.print("kWh");
    }

    lcd_.setCursor(0,3);
    lcd_.print("                 ..");
}

void LCD_displayIF::displayShowerScreen2(double energyPrice, bool isOutdated){
    lcd_.clear();

    lcd_.setCursor(0,0);
    lcd_.print("                 ..");

    lcd_.setCursor(0,1);
    lcd_.print("En.pris: "); lcd_.print(energyPrice, 2); lcd_.print("kr/kWh");

    if(isOutdated)
    lcd_.setCursor(0,2);
    lcd_.print("(Foraeldet pris)");
}

void LCD_displayIF::updateShowerScreen0(double flowRate, double totalWater, double temp){
    lcd_.setCursor(12,0);
    lcd_.print("       "); //the clear previous value
    lcd_.setCursor(12,0);
    lcd_.print(flowRate, 1); lcd_.print("L/m");

    lcd_.setCursor(12,1);
    lcd_.print("      ");
    lcd_.setCursor(12,1);
    lcd_.print(totalWater, 1); lcd_.print("L");

    lcd_.setCursor(12,3);
    lcd_.print("     ");
    lcd_.setCursor(12,3);
    lcd_.print(temp, 1); lcd_.print("C");
}

void LCD_displayIF::updateShowerScreen1(double totalEnergy){
    lcd_.setCursor(11,1);
    lcd_.print("        "); //to clear previous value
    lcd_.setCursor(11,1);
    lcd_.print(totalEnergy, 2); lcd_.print("kWh");

   
}

void LCD_displayIF::displayWaterExceededWarning(){
    lcd_.clear();
    lcd_.setCursor(1,1);
    lcd_.print("Maks. vandforbrug");
    lcd_.setCursor(1,2);
    lcd_.print("overskredet!");
    delay(2000);
}


void LCD_displayIF::displayEnergyExceededWarning(){
    

    lcd_.clear();
    lcd_.setCursor(1,1);
    lcd_.print("Maks. energiforbrug");
    lcd_.setCursor(1,2);
    lcd_.print("overskredet!");
    delay(2000);
}

void LCD_displayIF::displayShowerEnded(double totalWater, double totalEnergy, double totalPrice, bool isOutdated){
    lcd_.clear();
    lcd_.print("No flow!");
    lcd_.setCursor(0,1);
    lcd_.print("Shower ended");
    delay(3000);

    lcd_.clear();
    lcd_.setCursor(0,0);
    lcd_.print("Vandforb.: "); lcd_.print(totalWater,1); lcd_.print("L");
    lcd_.setCursor(0,1);
    lcd_.print("En.forb.:  "); lcd_.print(totalEnergy,2); lcd_.print("kWh");
    lcd_.setCursor(0,2);
    lcd_.print("Tot. pris: "); lcd_.print(totalPrice,1); lcd_.print("kr.");
    lcd_.setCursor(0,3);
    if(isOutdated){
        lcd_.print("(Gammel energipris)");
    }
    
}

