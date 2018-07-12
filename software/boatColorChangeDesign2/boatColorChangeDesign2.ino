//Code by Laura March 2018
//Revised July 2018 for the Magazine beach installation
//Offspring of the boatColorChangeDesign1 code for updated hardware
//changed include fade potentiometer and the long button press being not quite so long
//Combines many sketches in the boat color change folder
//dotstar leds color control
//radio
//potentiometers
//long and short presses of the buttons

///////////////////////////////////////////////////////////////////////////////////////////
///////////// RADIO SETUP
#include <RH_RF95.h>
// for feather m0
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
// Blinky on receipt
//#define LED 13
//for Tx
int16_t packetnum = 0;  // packet counter, we increment per xmission
//variable for telling us what mode we're in

//CONTROLLER ID
//each boat and controller pair has a match ID set to make sure the right one gets the right colors
int thisBoatID = 89;

///////////// DOTSTAR SETUP
#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET
#define NUMPIXELS 10 // Number of LEDs in strip
// Here's how to control the LEDs from any two pins:
#define DATAPIN    13
#define CLOCKPIN   11
Adafruit_DotStar strip = Adafruit_DotStar(
 NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.
// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DOTSTAR_BRG);
// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.
int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)


/////////// POTENTIOMETER VARIABLES
// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
//three color sliders and the one fade pot
const int numReadings = 10;
int readings[4][numReadings];      // the readings from the analog input
int readIndex[4] = {0,0,0,0};              // the index of the current reading
int total[4] = {0,0,0,0};                  // the running total
int average[4] = {0,0,0,0};                // the average
int inputPin[4] = {A0, A1, A2, A3};
int potVector[4] = {0, 1, 2, 3};     //an index vector for the number of pots
//pull out the variable that's used for the fade potentiometer
int fade = 0;


/////// SINGLE LED COLORS
//the current and most recent color of the one button light that is directly controlled by the sliders
//R G B
int currentColor[3]={0,0,0};
int lastColor[3]={0,0,0};

/////// LED BUTTON COLORS
//The matrix of colors in the 2x4 grid plus the first LED, 3 values each--RED BLUE GREEN
int buttonColors[9][3];






/////// BUTTON VARIABLES
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin[5]={5, 6, 9, 10, 12};
char buttonName[9] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I'};
int buttonLED[9] = {3, 6, 1, 4, 5, 8, 2, 7, 0}; //the buttons aren't in order on the board! these are the LEDs they correspond to
// Variables will change:
int buttonState[9]= {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
int pinState[5];
int lastbuttonState[9] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};
int longButtonStatus[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int lastPinState[5] = {LOW, LOW, LOW, LOW, LOW}; 
int reading[5]; //current reading off the pin
// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime[5] = {0, 0, 0, 0, 0};  // the last time the output pin was toggled
unsigned long debounceDelay = 35;    // the debounce time; increase if the output flickers (previously 50)
int longButtonTime = 1500; //how long does a button press need to be to be called long, originally 3000
unsigned long buttonTime[9] = {0,0,0,0,0,0,0,0,0}; //the last time each button changed state for real (debounced changed)

/////// DATA FOR SENDING
//version with the seeboat data
typedef struct {
  //strings are annoying to send, avoid them
  int boatID;
  int fade;
  int colRed;
  int colBlue;
  int colGreen;
} Payload;
Payload theData;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  radioStartup();
  dotStarSetup();
  potSetup();
//  switchSetup();
  buttonPinSetup();
  buttonColorsInit();
//    dotStarSet(0, 100, 100, 100);

}


/////////////////////////////////////////////////////////////////////////
void loop() {
  //Read all the potentiometers and set the lights accordingly
  potRead(0);
  potRead(1);
  potRead(2);
  
  //Do the measurement on the third potentiometer
  potRead(3);
  //put that measurement into a variable
  fade = average[3];
  //if the fade value has changed by enough, send it by radio without changing the other colors/parameters....

  if (abs(fade-theData.fade)>10){
    //put it together with the full data packetand then send
    dataFade();
    radioSend();
  }

  
  //the next one only applies to the three color pots
  potToColor();

//  switchRead();
// colorLoop();
// dotStarSet(0, 0, 255, 0);
  //Set the LED color and update the data to take the change of color into account
  dotStarSet(0, currentColor[0], currentColor[1],currentColor[2]);
  buttonColorUpdate(0);

/*
  Serial.print(buttonColors[0][0]);
  Serial.print(buttonColors[0][1]);
  Serial.println(buttonColors[0][2]);

  Serial.print(buttonColors[1][0]);
  Serial.print(buttonColors[1][1]);
  Serial.println(buttonColors[1][2]);
*/

  //Read the fade switch
  //switchRead();



  //DO ALL THE BUTTON STUFF!
    // check to see if you just pressed the button
  //readButtonPins();
  for (int i = 0; i<5; i++){
   // read the state of all of the button pins:
  reading[i] = digitalRead(buttonPin[i]);
  }


  //check for pin change due to noise or button pressing and update the debounce time accordingly
  //pinCheckNoisePress();
  // If the switch changed, due to noise or pressing:
  //NOTE: LAST DEBOUNCE TIME IS ON THE PINS...BUT LONG AND SHORT PRESS IS ON THE BUTTONS
  for (int i = 0; i<5; i++){
  if (reading[i] != lastPinState[i]) {
    // reset the debouncing timer
    lastDebounceTime[i] = millis();
  }
  }
    // if the pin state has changed:
  for (int i = 0; i<5; i++){
  if ((millis() - lastDebounceTime[i]) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    //if any of the pins have changed, make the prior button state the most recent button state
//    if (reading[i] != pinState[i]) {
//      pinState[i] = reading[i];   ////////////////////////////////////////////////
//      for (int j=0; j<9; j++){
//      lastbuttonState[j] = buttonState[j];
//      }
//    } ////////////////////////////////
    //Store the new button states
      buttonState[0] = reading[0]*reading[1];
      buttonState[1] = reading[0]*reading[2];
      buttonState[2] = reading[0]*reading[3];
      buttonState[3] = reading[0]*reading[4];
      buttonState[4] = reading[1]*reading[2];
      buttonState[5] = reading[1]*reading[3];
      buttonState[6] = reading[1]*reading[4];
      buttonState[7] = reading[2]*reading[3];
      buttonState[8] = reading[2]*reading[4];

    //note the time of the most recent button change for each button
        for (int k = 0; k<9; k++){
          if (buttonState[k] != lastbuttonState[k]){
            buttonTime[k]=millis();
            //Serial.print(buttonState[k]);
            //Serial.print(lastbuttonState[k]);
            //Serial.println(buttonName[k]);
          }
        }

//short button press: when something has changed, the button has switched off and the time has been short enough
    for (int k = 0; k<9; k++){
      if ((buttonState[k] != lastbuttonState[k]) && (buttonState[k]==LOW) && (millis()-buttonTime[k]<longButtonTime)){
        Serial.print("test1");
        if (longButtonStatus[k]==0){
          ////////////          DO THE THING FOR THE SHORT BUTTON PRESS ////////////////////////////////
          //i.e. send the data to the boat!
             Serial.print("SHORT BUTTON PRESS FINISHED: ");
            Serial.println(buttonName[k]);
            dataAssemble(buttonLED[k]);
            radioSend();
        }
        //if we're just unpressing from a long button press, don't do anything, just prep for the next press
         if (longButtonStatus[k]==1){
          longButtonStatus[k]=0;
        }
    }
    }
  
//    }
//  }


//See if there are buttons that have been high for a looong time
for (int k = 0; k<9; k++){
  //the first time 
  if ((millis()-buttonTime[k])>longButtonTime && buttonState[k]==HIGH){
    /////////////////////        DO THE THING FOR THE LONG BUTTON PRESS //////////////////////
    //i.e. change the color of that LED to the color that's currently in the first button
    //don't do anything if there's a long press on the first button
    if (longButtonStatus[k]==0){
        Serial.print("LONG BUTTON PRESS HAPPENING: ");
        Serial.println(buttonName[k]);
        //Serial.print(lastbuttonState[k]);
        //Serial.println(buttonState[k]);
        //update the data and change the color
        buttonColorUpdate(buttonLED[k]);
        dotStarSet(buttonLED[k], currentColor[0], currentColor[1],currentColor[2]);
        
        //this makes sure we don't also trigger the short button press when we let go of the button and that we don't keep triggering the long button press 
        longButtonStatus[k]=1;
    }
  }
}

  }
//  }
  
  // save the reading.  Next time through the loop, it'll be the lastPinState:
  //updatePinState();
  for (int i = 0; i<5; i++){  
  lastPinState[i] = reading[i];
  }
  //NEW do the same for the button state too
  for (int j=0; j<9; j++){
    lastbuttonState[j] = buttonState[j];
   }
  }
  
}

