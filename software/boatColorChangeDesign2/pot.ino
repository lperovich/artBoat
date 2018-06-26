////////////////////////////////////////////////////////
//Function to setup the three potentiometers
void potSetup(){
  // initialize all the readings to 0:
  for (int i=0; i<4; i++){
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[i][thisReading] = 0;
  }
}
}

/////////////////////////////////////////////////////////
//Function to read the value of the potentiometers and smooth it out
//the values go into a vector called "average" that goes from 0 to 1024 
void potRead(int whichPot){
//which potentiometer is 0, 1, 2, 3
  // subtract the last reading:
  total[whichPot] = total[whichPot] - readings[whichPot][readIndex[whichPot]];
  // read from the sensor:
  readings[whichPot][readIndex[whichPot]] = analogRead(inputPin[whichPot]);
  // add the reading to the total:
  total[whichPot] = total[whichPot] + readings[whichPot][readIndex[whichPot]];

//  total[whichPot] = total[whichPot] + readings[whichPot][readIndex];
  // advance to the next position in the array:
  readIndex[whichPot] = readIndex[whichPot] + 1;

  // if we're at the end of the array...
  if (readIndex[whichPot] >= numReadings) {
    // ...wrap around to the beginning:
    readIndex[whichPot] = 0;
  }

  // calculate the average:
  average[whichPot] = total[whichPot] / numReadings;
  // send it to the computer as ASCII digits
  //Serial.println(average[whichPot]);
//  delay(2);        // delay in between reads for stability
}

//////////////////////////////////////////////////////////////////////
//function that takes the current potentiometer values and maps them to colors
void potToColor(){
  //for each potentiometer, turn the value into a color (R, G, B)
  //ONLY THE THREE COLOR ONES, not the fade one (so only <3)
  //Note that we need to flip to direction so that it increases left to right (not right to left)
  for (int i=0; i<3; i++){
    currentColor[i]=map(average[i], 0, 1024, 255, 0);
//    Serial.println(currentColor[i]);
  }

}

