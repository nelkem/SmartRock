// For simplicity, the Serial Comms are almost exclusively used for debugging. The command codes
// defined in Serial Comms will be identical to the commands sent over bluetooth, once that is
// integrated.

/*

Command
00000     Full Device Reset to Default
00001     Read Battery Voltage
00002     RTC Reset to Default
00003     Bluetooth Reset to Default
00004     SD Reset to Default
00005     Pressure Sensor Reset to Default

10000     Display Sensor Commands
10001     Take Pressure Reading, print out to serial
10002     Take Temperature Reading, print out to serial
10011     Take Pressure Reading, print to SD and serial
10012     Take Temperature Reading, print to SD and serial

20000     Display SD Commands
20001     Write to SD File
20011     Read entire SD File, print out on serial
20012     Read n characters from SD File, print out on serial

30000     Setup and Connect BLE

*/



void serialCheck(){
if(!Serial){
  Serial.begin(9600);
}
if(Serial){
if(Serial.available() > 0){ //CU
  receivedCommand = true;
  String thisCommand = "";
  if(Serial.available() == 7){
    delay(10);//CU
    
    //ask for 5 digits
    char c; //command length
    for(int i = 0; i<7; i++){
      c = Serial.read();
      thisCommand.concat(c);
    }
    if(Serial) Serial.print("Parsing command " + thisCommand);
    runCommand( thisCommand, false);
  }
  else{
    while(Serial.available()){
      char c = Serial.read();
      thisCommand.concat(c);
    }
    if(Serial) Serial.println("ERROR: Invalid command: " + thisCommand);
  }
  if(Serial) Serial.println();
}
}

}

void runCommand(String command, bool sendBLE = false){
    Serial.begin(9600);
  
  switch(command.toInt())
  {
    case 00000: // Print Help ***********************************************
    {
      if(sendBLE){
        blePrintHelp();
      }
      else serialPrintHelp();
    }
      break;

    case 00001: // READ BATTERY ***********************************************
    {
      float bat = float(analogRead(A7) * (2.00/1024)*3.30);
      Serial.println("Battery Voltage: " + String(bat) + "V");
      if(sendBLE) ble.println("Battery Voltage: " + String(bat) + "V");
    }
      break;

    case 10001: // PRESSURE READING ***********************************************
    {
      if(update_Temp_Press()){
        if(Serial) Serial.println( "Pressure: " + String(pressureC) + " Pa");
        if(sendBLE){
          ble.println( "Pressure: " + String(pressureC) + " Pa" );
        }
      }
      else{
        if(Serial) Serial.println("Could not read pressure.");
        if(sendBLE){
          ble.println("Could not read pressure.");
        }
      }
    }
      break;

    case 10002: // TEMPERATURE READING ***********************************************
    {
      if(update_Temp_Press()){
        
        if(Serial) Serial.println( "Temperature: " + String( tempC ) + " C");
        if(sendBLE){
          ble.println( "Temperature: " + String( tempC ) + " C");
        }
      }
      else{
        if(Serial) Serial.println("Could not read temperature.");
        if(sendBLE){
          ble.println("Could not read temperature.");
        }
      }
    }
      break;

    case 20001: // WRITE TO SD FILE ***********************************************
    {

      // User inputs file name
      if(Serial) Serial.print("File to write to: ");
      while(!Serial.available());
      String fName = "";
      while(Serial.available()){
        fName += Serial.read();
      }
      if(Serial) Serial.println(fName);

      // User inputs string to write
      if(Serial) Serial.print("String to write: ");
      while(!Serial.available());
      String uString = "";
      while(Serial.available()){
        uString += Serial.read();
      }

      if( writeToFile(fName, uString) ) if(Serial) Serial.println("Wrote '" + uString + "' to " + fName + ".");
      else if(Serial) Serial.println("Error encountered. Did not write to file.");
    }
      break;

    case 20002: // READ NUMBYTES FROM SD FILE ***********************************************
    {
      //User inputs file name
      if(Serial) Serial.print("File to read from: ");
      while(!Serial.available());
      String fName = "";
      while(Serial.available()){
        fName += Serial.read();
      }
      if(Serial) Serial.println(fName);

      // User inputs number of chars to read
      if(Serial) Serial.print("Number of characters to read: ");
      while(!Serial.available());
      uint8_t numBytes = 0;
      
      while(Serial.available()){
        numBytes += int(Serial.read()) * (10 * Serial.available());
      }
      String fString = readFromFile(fName, numBytes);
      if(Serial) Serial.println(fString);
    }
      break;

    case 30000: // Setup and Connect BLE Device ***********************************************
    {
      if(Serial) Serial.print("Setting up BLE Device. Connect with your phone!");
      bleConnect();
    }
      break;
      
    default: // Default means the user entered a valid format but nonexistant command ********************************
      {
      if(Serial) Serial.println("ERROR: Invalid Command");
      if(sendBLE){
        ble.println("ERROR: Invalid Command");
      }
      }
      break;
  }
}

void serialPrintHelp(){
  if(!Serial){
    Serial.begin(9600);
  }
if(Serial) Serial.println("Valid Commands:\n");
if(Serial) Serial.println("00000     Print Command List");
if(Serial) Serial.println("00001     Read Battery Voltage");
//if(Serial) Serial.println("00002     RTC Reset to Default");
//if(Serial) Serial.println("00003     Bluetooth Reset to Default");
//if(Serial) Serial.println("00004     SD Reset to Default\n");

//if(Serial) Serial.println("10000     Display Sensor Commands");
if(Serial) Serial.println("10001     Take Pressure Reading, print out to serial");
if(Serial) Serial.println("10002     Take Temperature Reading, print out to serial");
//if(Serial) Serial.println("10011     Take Pressure Reading, print to SD and serial");
//if(Serial) Serial.println("10012     Take Temperature Reading, print to SD and serial\n");

//if(Serial) Serial.println("20000     Display SD Commands");
//if(Serial) Serial.println("20001     Write to SD File");
//if(Serial) Serial.println("20011     Read entire SD File, print out on serial");
//if(Serial) Serial.println("20012     Read n characters from SD File, print out on serial\n");

if(Serial) Serial.println("30000     Setup and Connect BLE\n");
}

