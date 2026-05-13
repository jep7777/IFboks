#include <Arduino.h>
#include "TrykknapperIF.h"
#include "LCD_displayIF.h"
#include "interfaceboksControl.h"


#define BUTTON1 13
#define BUTTON2 12
#define BUTTON3 14
#define BUTTON4 27

//contains flags, that are set on button push
volatile uint8_t buttonFlags = 0;

//ISR sets flag of corresponding button
void IRAM_ATTR handleButtonPress1() { buttonFlags |= (1 << 0); }
void IRAM_ATTR handleButtonPress2() { buttonFlags |= (1 << 1); }
void IRAM_ATTR handleButtonPress3() { buttonFlags |= (1 << 2); }
void IRAM_ATTR handleButtonPress4() { buttonFlags |= (1 << 3); }

//Start state in main manu
State currentState = State::MAIN_MENU;

//initialize objects
LCD_displayIF display(Serial2); //display object sends via UART serial2
BruserboksIF bruserboksIF(Serial1); //bruserboksIF object recieves via UART serial1
InterfaceboksControl mainControl(display, currentState, bruserboksIF);
TrykknapperIF buttons(buttonFlags, mainControl);

  void setup() {
    Serial.begin(9600); //for testing through USB
    display.begin(9600); //BAUD rate to display is 9600
    bruserboksIF.begin(9600); //BAUD rate to bruserboksIF is 9600. RX is gpio 16, TX is gpio18

    //set internal pullup of button pins
    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT);
    pinMode(BUTTON3, INPUT);
    pinMode(BUTTON4, INPUT);

    //attach ISR's to buttons falling edge
    attachInterrupt(BUTTON1, handleButtonPress1, RISING);
    attachInterrupt(BUTTON2, handleButtonPress2, RISING);
    attachInterrupt(BUTTON3, handleButtonPress3, RISING);
    attachInterrupt(BUTTON4, handleButtonPress4, RISING);

    //start with opening main menu
    mainControl.openMainMenu();
  }

void loop() {

//check each button flag
if(buttons.checkButtonFlag(1<<0)){
  buttons.getControlClass().decCursorIndex();
}
else if(buttons.checkButtonFlag(1<<1)){
  buttons.getControlClass().incCursorIndex();
}
else if(buttons.checkButtonFlag(1<<2)){
  buttons.getControlClass().handleSelect();
}
else if(buttons.checkButtonFlag(1<<3)){
  buttons.getControlClass().handleDecrement();
}

//if shower is running, run measurement sequence
if(mainControl.getCurrentState() == State::SHOWER_RUNNING){
  mainControl.measurementSequence();

}


}
