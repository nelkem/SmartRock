void logDataPoint(double point, String filename){
  String dataString = String(point) + ",";
  
  if(!Serial){
    Serial.begin(9600);
  }
  
  File dataFile = SD.open(filename, FILE_WRITE);

  if(dataFile){
    dataFile.print(dataString);
    dataFile.close();
  }
  else{
    if(Serial) Serial.println("error opening " + filename);
  }
  if(Serial) Serial.println("Data Point: " + dataString);
  Serial.end();
  return;
}

/*
 * ************************************************************************
 */
void readAllData(String filename){
  if(!Serial){
    Serial.begin(9600);
  }
  
  File dataFile = SD.open(filename, FILE_READ);
  if(dataFile){
    while(dataFile.available()){
      if(Serial) Serial.print(dataFile.read());
    }
    dataFile.close();
  }
  else{
    if(Serial) Serial.println("error opening " + filename);
  }
  Serial.end();
  dataFile.close();
}

/*
 * ************************************************************************
 */

bool initSD(int pin){
  pinMode(pin, OUTPUT);
  
  if(!Serial){
    Serial.begin(9600);
  }
  Serial.print("Initializing SD... ");
  if(!SD.begin(pin)){
    Serial.println("SD Init Failed");
    return false;
  }
  else{
    Serial.println("SD Init Successful! Congratulations!");
    return true;
  }
}

/*
 * ************************************************************************
 */

bool writeToFile(String fName, String fString){
  if(!Serial){
    Serial.begin(9600);
  }
  
  File dataFile = SD.open(fName, FILE_WRITE);

  if(dataFile){
    dataFile.print(fString);
    dataFile.close();
  }
  else{
    if(Serial) Serial.println("error opening " + fName);
    return false;
  }
  return true;
}


/*
 * ************************************************************************
 */
 
String readFromFile(String fName, int numChars){
  if(!Serial){
    Serial.begin(9600);
  }

  File dataFile = SD.open(fName, FILE_READ);
  String fString = "";
  if(dataFile){
    
    while(dataFile.available() && numChars > 0){
      numChars --;
      fString += dataFile.read();
    }
    if(numChars > 0){
      if(Serial) Serial.println("Not enough characters in file.");
    }
    dataFile.close();
  }
  else if(Serial) Serial.println("ERROR: " + fName + " could not be opened.");
  
  return fString;
}

bool checkTime(){
  if(true);
}



