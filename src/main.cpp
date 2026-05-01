#include <Arduino.h>
#include "TrykknapperIF.h"
#include "LCD_displayIF.h"
#include "interfaceboksControl.h"
#include "utility.h"

hw_timer_t* timer = NULL; //pointer to timer struct
volatile bool sendRequestFlag = false; //is set when new request is to be sent

void IRAM_ATTR timerISR(){
  sendRequestFlag = true;
}

//Start state in main manu
State currentState = State::MAIN_MENU;

//initialize objects for the classes
LCD_displayIF display(Serial2);
InterfaceboksControl mainControl(display, currentState, &sendRequestFlag);
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

    timer = timerBegin(0, 80, true); //timer0, prescaler 80(gives freq og 80MHz/80 = 1MHz), true means counting up
    timerAttachInterrupt(timer, timerISR, true); //attach timer to timerISR
    timerAlarmWrite(timer, 1000000, true); //set alarm every 1M ticks(every 1 seconds)
    timerAlarmEnable(timer);
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


/* 
//if state is SHOWER_RUNNING, check if request should be sent
if(mainControl.currentState_ == State::SHOWER_RUNNING){
  mainControl.checkSendRequestFlag(); //starts request sequence
  mainControl.measurementSequence(); //starts measurementSequence

} */


}
