////////////////////////////////////////////////////////
/*
void switchSetup() {
  // initialize the button pin as a input:
  pinMode(switchPin, INPUT);
}
*/
/////////////////////////////////////////////////////////
/*
void switchRead() {
  // read the switch input pin:
  switchState = digitalRead(switchPin);

  // compare the switchState to its previous state
  if (switchState != lastSwitchState) {
    // if the state has changed, increment the counter
    if (switchState == HIGH) {
      // if the current state is HIGH then the switch went from off to on:
      switchPushCounter++;
      Serial.println("on");
      //Serial.print("number of switches changes:  ");
      //Serial.println(switchPushCounter);
      fade = 1;
    } else {
      // if the current state is LOW then the switch went from on to off:
      Serial.println("off");
      fade = 1;
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state,
  //for next time through the loop
  lastSwitchState = switchState;

  //put the value into the variable that controls whether we're in fade mode or not
  fade = switchState;
}

*/
