/* TODO:
 *  [ ] Store all strings in flash memory
 *  [ ] Move all settings to CONFIG.txt
 *  Add Simple Bluefruit Comms:
 *    [x] Send data point on request
 *    [ ] Send data every sample
 *    [x] Receive commands
 *    [ ] Update settings
 *  [ ] Second order temp compensation
 *  [ ] SD functionality
 */

#include "Includes.h"

void setup() {
  delay(3000);
  initSD(SD_ChipSelect);
  Serial.println("initializing MS");
  initMS();
  Serial.println("Init MS Success");
  pinMode(A7, INPUT);
  if(getSettings()){
    updateSettings();
  }
  if(!Serial){
    Serial.begin(9600);
  }
  Serial.println("bleSetup");
  bleSetup();
  Serial.println("setup finished");
  //bluefruit's initialization to use SPI
}

void loop() {
  if(!Serial){
    Serial.begin(9600);
  }
  Serial.println("Enter a command. '00000' for help.");
  if(ble.isConnected()){
    if(!bleIsInit){
      bleConnect();
    }
    
    ble.println("Enter a command. '00000' for help.");
  }
  while(!receivedCommand){
    
    serialCheck();

    //check bluetooth comms:
    if(ble.isConnected() && !bleIsInit){ //if the user just connected to bluetooth
      bleConnect();//initialize BLE
      delay(5000);// wait a few seconds
      ble.println("Enter a command. '00000' for help."); // let them know what to do
    }
    //if a command has been sent, parse it
    if(ble.isConnected() && ble.available()) bleReceiveCommand(false);
    
    if(!ble.isConnected()) bleIsInit = false;//if BLE is disconnected, we'll need to reinitialize
    
  }
  receivedCommand = false;
}
