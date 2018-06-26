//////////////////////////////////////////////////////
void buttonPinSetup(){
  for (int i = 0; i<5; i++){
  pinMode(buttonPin[i], INPUT);
  }  
}
////////////////////////////////////////////
//save the most recent pin reading as the last pin state (for the end of the loop)
void updatePinState(){
  for (int i = 0; i<5; i++){  
  lastPinState[i] = reading[i];
  }
  }

///////////////////
  void newButtonStates(){
    //upade all the button states, based on what pins they're connected to
  //this happens any time any of the pins change (w/debounce)
      buttonState[0] = pinState[0]*pinState[1];
      buttonState[1] = pinState[0]*pinState[2];
      buttonState[2] = pinState[0]*pinState[3];
      buttonState[3] = pinState[0]*pinState[4];
      buttonState[4] = pinState[1]*pinState[2];
      buttonState[5] = pinState[1]*pinState[3];
      buttonState[6] = pinState[1]*pinState[4];
      buttonState[7] = pinState[2]*pinState[3];
      buttonState[8] = pinState[2]*pinState[4];
  }
/////////////////////////
void readButtonPins(){
  for (int i = 0; i<5; i++){
   // read the state of all of the button pins:
  reading[i] = digitalRead(buttonPin[i]);
  }
}
///////////////////////
  void pinCheckNoisePress(){
  // If the switch changed, due to noise or pressing:
  //NOTE: LAST DEBOUNCE TIME IS ON THE PINS...BUT LONG AND SHORT PRESS IS ON THE BUTTONS
  for (int i = 0; i<5; i++){
  if (reading[i] != lastPinState[i]) {
    // reset the debouncing timer
    lastDebounceTime[i] = millis();
  }
  }
  }
/////////////////////////
void updateButtonTime(){
  for (int k = 0; k<9; k++){
    if (buttonState[k] != lastbuttonState[k]){
      buttonTime[k]=millis();
//      Serial.print(buttonState[k]);
//      Serial.print(lastbuttonState[k]);
//      Serial.println(buttonName[k]);
    }
  }
}

