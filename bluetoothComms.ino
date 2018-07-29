/*
 * BLUETOOTH COMMS
 * General functions for sending out data to and from the Adafruit Bluefruit app.
 * Much of it is based on Adafruit's guides and example code for Bluefruit comms
 * via their Feather M0.
 */


void bleSetup(){
  if(!Serial){
    Serial.begin(9600);
  }
  if( !ble.begin(VERBOSE) ) // Adafruit allowed us to turn on / off error messages
  {
    if(Serial) Serial.println("BLE could not be initialized.");
  }

  ble.info();
}

void bleConnect(){
  while(! ble.isConnected()){ //Adafruit's recommended wait routine
    delay(500);
  }
  ble.setMode(BLUEFRUIT_MODE_DATA); // set to simple UART back and forth comms
  bleIsInit = true;
}


void bleReceiveCommand(bool ECHO = false){
  if(!Serial){
    Serial.begin(9600);
  }
  String userCommand = "";
  if(ble.available() == 6){
    while ( ble.available() ){
      userCommand.concat(char(ble.read()));
    }
    receivedCommand = true;
    runCommand(userCommand, true);
  }
  else{
    while(ble.available()){
      userCommand.concat(char(ble.read()));
    }
    ble.println("ERROR: Invalid command: " + String(userCommand));
  }
  ble.println();
  
}

void blePrintHelp(){
ble.println("Valid Commands:\n");
ble.println("00000 Print Command List");
ble.println("00001 Read Battery Voltage");
//ble.println("00002 RTC Reset to Default");
//ble.println("00003 Bluetooth Reset to Default");
//ble.println("00004 SD Reset to Default");

//ble.println("10000 Display Sensor Commands");
ble.println("10001 Print Pressure");
ble.println("10002 Print Temperature");
//ble.println("10011 Take Pressure Reading, print to SD and serial");
//ble.println("10012 Take Temperature Reading, print to SD and serial");

//ble.println("20000 Display SD Commands");
//ble.println("20001 Write to SD File");
//ble.println("20011 Read entire SD File, print out on serial");
//ble.println("20012 Read n characters from SD File, print out on serial");

ble.println("30000 Setup and Connect BLE");
}

