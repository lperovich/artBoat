/*
Functions to control the sending and receiving of the messages
 */
// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX


//////////////////////////////////////////////////////////////////////////////////////
//Function to get the radio started in the setup

void radioStartup(){
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  delay(200);
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
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
  delay(200);
}
//////////////////////////////////////////////////////////////////////
//function to send a structured message
void radioSend(){

//from the moteino code
    //fill in the struct with new values--this happens in the data assemble part    

//RF69 version
//    if (radio.sendWithRetry(GATEWAYID, (const void*)(&theData), sizeof(theData)))
//      Serial.print(" ok!");

//      Serial.print("Sending "); Serial.println(buf);
  rf95.send((uint8_t *)(&theData), sizeof(theData));
  
//    else Serial.print(" nothing...");
//    Serial.println();
//    Blink(LED,3);

//  Serial.print("Sending "); Serial.println(buf);
//  rf95.send((uint8_t *)buf, sizeof(buf));

}



//////////////////////////////////////////
//Function to listen to see if anyone is talking to us
//if we hear something, we print it to serial and we send a reply back to acknowledge it
/*
void radioListen(){
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len))
    {
      //digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got: ");
      //this works best for numbers
      //the first number is the device ID
      for (int i=0; i<1; i++){
        Serial.println(buf[i]); //this seems to give us normal number values
          tempReceivedDeviceID=buf[i];
      }
      //the other numbers are the message
      for (int i=1; i<len; i++){
        Serial.println(buf[i]); //this seems to give us normal number values
        tempMessageReceive[i-1]=buf[i]; //store the message here at first, then we'll put it in the overall message string
      }
      //OLD: Serial.println((char*)buf);
       Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      //Store this message and move all the other messages down
      messageStoreReceive();
      //clear out the variables so we can receive the next message
      clearMessageVarsSend();
      delay(10);
      //reset the time (so we start playing at the beginning of the message, not halfway through) (maybe this isn't good??)
      playLoopStartTime = millis();
      j=0;
*/

//FIND A WAY TO STORE THIS AS NORMAL NUMBERS THAT WE CAN USE LIKE BEFORE
//Right now we are getting things like: 3 6 9 C 63 63 63 63 63 63 63 63 63 63 63 
//(63=99 in hex)
      
 /*     // Send a reply
      uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      //digitalWrite(LED, LOW);
   */
   /*   
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}
*/




