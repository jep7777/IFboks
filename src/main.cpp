#include <Arduino.h>
#include "TrykknapperIF.h"
#include "LCD_displayIF.h"
#include "interfaceboksControl.h"
#include "utility.h"




//Start state in main manu
State currentState = State::MAIN_MENU;

//initialize objects for the classes
LCD_displayIF display(Serial2);
InterfaceboksControl mainControl(display, currentState);
TrykknapperIF buttons(buttonFlags, mainControl);

  void setup() {
    display.begin(9600); //BAUD rate to display is 9600

    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);
    pinMode(BUTTON4, INPUT_PULLUP);

    attachInterrupt(BUTTON1, handleButtonPress1, FALLING);
    attachInterrupt(BUTTON2, handleButtonPress2, FALLING);
    attachInterrupt(BUTTON3, handleButtonPress3, FALLING);
    attachInterrupt(BUTTON4, handleButtonPress4, FALLING);

    mainControl.openMainMenu();
  }

void loop() {


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


if(mainControl.getCurrentState() == State::SHOWER_RUNNING){
  mainControl.measurementSequence();

}


}
