// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX

#include <SPI.h>
#include <RH_RF95.h>

////////////////   RADIO VARIABLES
/* for feather m0 */
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
// Blinky on receipt
#define LED 13

////////////////   DATA VARIABLES
//version with the colorboard boat data
typedef struct {
  //strings are annoying to send, avoid them
  int fade;
  int colRed;
  int colBlue;
  int colGreen;
} Payload;
Payload theData;

int fade;
int colRed=0;
int colGreen=0;
int colBlue=0;
int nowRed;
int nowBlue;
int nowGreen;

////////////////   LED VARIABLES
int fadeCount = 0;
int fadeLength = 50;
int redDiff;
int greenDiff;
int blueDiff;

////////////////   DOTSTAR VARIABLES
#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET
#define NUMPIXELS 55 // Number of LEDs in strip
//The number of LEDs in the strand
int numLEDs=55;
// Here's how to control the LEDs from any two pins:
#define DATAPIN    13
#define CLOCKPIN   11
Adafruit_DotStar strip = Adafruit_DotStar(
 NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.


////////////////////////////////////////////////////////////////
void setup() {
  radioSetup();
  dotStarSetup();
}

void loop(){
  //radioReceive changes the colors in the funtion itself
  radioReceive();

}

///////////////////////////////////////////////////////// FUNCTIONS
void radioSetup(){
    pinMode(LED, OUTPUT);     
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  //take this serial line out if you're not necessarily connected to the computer!
  //while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("Feather LoRa RX Test!");
  
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);

//Print the column headers
Serial.println("fade, colorRed, colorBlue, colorGreen");

}

/////////////////////////////////////////////////////////
void radioReceive(){
    if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len))
    {
      //from moteino code
      theData = *(Payload*)buf;
      //theData = *(Payload*)radio.DATA; //assume radio.DATA actually contains our struct and not something else
      Serial.print(theData.fade);
      Serial.print(",");
      Serial.print(theData.colRed);
      Serial.print(",");
      Serial.print(theData.colBlue);
      Serial.print(",");
      Serial.print(theData.colGreen);  
      Serial.println();
      
      //Do the color setting--CASCADE VERSION
      /*for (int k = 0; k<numLEDs; k++){
        //NOTE: DIFFERENT STRIPS HAVE THE COLORS IN DIFFERENT ORDERS! CHECK THEM!!
       // dotStarSet(k, theData.colBlue, theData.colGreen, theData.colRed);
      }
      */

//Different version for the continuous fade boat
//fade should impact fadeLength
//fade goes from 0 to 1024 or so
//fadeLength should probably go from 0 to 150? 256 is the max color difference, so it shouldn't be more than that
//It also needs to be flipped to make sense with the controller pot orientation
  fadeLength = map(fade, 0, 1023, 150, 0);

//execute the lights change
  stripFade();
/*
      if (fade == 0){
        //Do the color setting--ALL AT ONCE VERSION
        for (int k = 0; k<numLEDs; k++){
          strip.setPixelColor(k, strip.Color(theData.colBlue, theData.colGreen, theData.colRed));
        }
          strip.show();
      }
      if (fade == 1){
        stripFade();
      }
*/



      //Set the storing variables--do this after the color appearing so the fade part works
      fade = theData.fade;
      colRed = theData.colRed;
      colBlue = theData.colBlue;
      colGreen = theData.colGreen;
      
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}
/////////////////////////////////////////////////////////////////////////
//Function to setup all the dotStar LEDs for color changing; use in setup look
void dotStarSetup(){
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  
}

/////////////////////////////////////////
//Not so pretty fade function from one LED color to another

void stripFade(){
    redDiff = theData.colRed-colRed;
    greenDiff = theData.colGreen-colGreen;
    blueDiff = theData.colBlue-colBlue;
    
    for (int i=0; i<fadeLength; i++){
    nowRed = colRed + floor(i*redDiff/fadeLength);
    nowBlue = colBlue + floor(i*blueDiff/fadeLength);
    nowGreen = colGreen + floor(i*greenDiff/fadeLength);
    Serial.println(nowRed);
    Serial.println(nowGreen);
    Serial.println(nowBlue);
    
      for (int k = 0; k<numLEDs; k++){         
        strip.setPixelColor(k, strip.Color(nowBlue, nowGreen, nowRed));
       }
      strip.show();
//      delay(2);
    }
//    fadeCount=0;
}

//////////////////////////////////////////////////////////////////////
//function to set a certain dotstarLED to a certain color
void dotStarSet(int ledNum, int redVal, int blueVal, int greenVal){
  //ledNum is the number of the led in the stip (starts at 0)
  strip.setPixelColor(ledNum, strip.Color(redVal, blueVal, greenVal));
  strip.show();
}


