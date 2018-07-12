/*
functions to get data all together and put it in a form that can be sent by radio 
*/


//////////////////////////////////////////////////////////////////////////////
void dataAssemble(int i){
  //Puts the data together in an array ready to be sent.
  //i is the value we're pulling from in the LED matrix (0 is the first LED in the matrix, e.g. the second LED in the strand)
//  theData.fade = fade;
  //NOTE: color order is red, blue, green 
  theData.colRed = buttonColors[i][0]; 
  theData.colBlue = buttonColors[i][1]; 
  theData.colGreen =  buttonColors[i][2];

  //this is always constant
  theData.boatID = thisBoatID;

//    Serial.println(theData.fade);
    Serial.println(theData.colRed);
    Serial.println(theData.colBlue);
    Serial.println(theData.colGreen);
}

//////////////////////////////////////////////////////////////////////////////
//function to update the fade data part of the data packet
void dataFade(){
  theData.fade = fade;
}


