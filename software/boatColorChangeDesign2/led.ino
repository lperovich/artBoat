/*
 Functions that change the LEDs
 */

/////////////////////////////////////////////////////////////////////////
//Function to setup all the dotStar LEDs for color changing; use in setup look
void dotStarSetup(){
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  
}
//////////////////////////////////////////////////////////////////////
//function to set a certain dotstarLED to a certain color
void dotStarSet(int ledNum, int redVal, int blueVal, int greenVal){
  //ledNum is the number of the led in the stip (starts at 0)
  strip.setPixelColor(ledNum, strip.Color(redVal, blueVal, greenVal));
  strip.show();
}

//////////////////////////////////////////////////////////////////////
void buttonColorsInit(){
  for (int i = 0; i<9; i++){
  //Set all the LEDs to one color; remember the first one is special actually
  dotStarSet(i, 10, 10, 10);
}
  //Make all the colors variables to white for all matrix buttons
  //for some reason it hates it if you put another loop on the inside for 0,1,2
  for (int i = 0; i<9; i++){
    buttonColors[i][0]=0;    
    buttonColors[i][1]=0;    
    buttonColors[i][2]=0;    
  }
}
//////////////////////////////////////////////////////////////////////
//function to update any individual button color data. the first entry changes often with the pots
void buttonColorUpdate(int i){
  //i is the button that we're considering (0 to 8)
    buttonColors[i][0]= currentColor[0];
    buttonColors[i][1]= currentColor[1];
    buttonColors[i][2]= currentColor[2];
}



//////////////////////////////////////////////////////////////////////
//Function to send colors through a strip of dotstar LEDs one at a time
//the previous light in the strip turns off as the new one turns on
void colorLoop(){
  strip.setPixelColor(head, color); // 'On' pixel at head
  strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
  strip.show();                     // Refresh strip
  delay(20);                        // Pause 20 milliseconds (~50 FPS)

  if(++head >= NUMPIXELS) {         // Increment head index.  Off end of strip?
    head = 0;                       //  Yes, reset head index to start
    if((color >>= 8) == 0)          //  Next color (R->G->B) ... past blue now?
      color = 0xFF0000;             //   Yes, reset to red
  }
  if(++tail >= NUMPIXELS) tail = 0; // Increment, reset tail index
  
}


//////////////////////////////////////////////////////////////////////
//function that checks if the current color values are the same as the old ones
boolean colorChange(){
  boolean colorChange = 0;
  for (int i=0;i<3; i++){
  colorChange =colorChange +(currentColor[i]!= lastColor[i]);
  }
  return(colorChange);
}



