#pragma once
#include <Arduino.h>

#define BUTTON1 13
#define BUTTON2 12
#define BUTTON3 14
#define BUTTON4 27

//enum for states
enum class State {MAIN_MENU, SETTINGS, SHOWER_RUNNING};

//contains button flags, set by ISR's
extern volatile uint8_t buttonFlags;

void IRAM_ATTR handleButtonPress1();
void IRAM_ATTR handleButtonPress2();
void IRAM_ATTR handleButtonPress3();
void IRAM_ATTR handleButtonPress4();



/* //DET HER NEDE VIRKER IKKE, FOR DET ER IKKE PÅLIDELIGT AT TJEKKE FOR FALLING ELLER RISING EDGE MED IF STATMENT
void IRAM_ATTR handleButtonPress1()
{
    //static makes sure, that it is only initialized to zero on first call
    //and them remembered between calls
    static unsigned long lastChange = 0;

    bool risingEdge(digitalRead(BUTTON1));
    unsigned long now = millis();

    if(risingEdge){
        //if true, rising edge, button was released
        //update time since last change
        lastChange = now;
    }
    else{
        //if false, falling edge, button was pressed
        if((now - lastChange) > debounceTime){
            //set flag only if debounce time has passed since last change
            buttonFlags |= (1<<0);
            lastChange = now;
        }

    }
} */
