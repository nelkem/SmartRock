

// Check CONFIG file and update settings

// Check that config.txt exists
bool checkConfig(){
  if(SD.exists("CONFIG.txt")){
    return true;
  }
  else return false;
}

bool getSettings(){
  /*  grab config settings from CONFIG.txt. If there is no config file, the function
   *   will return 'false'. Otherwise, the settings will be stored in the settings[][]
   *   matrix, where each settingname and settingvalue are stored as strings. This
   *   function should follow an SD initialization, and updateSettings() should be called
   *   if getSettings() returns true.
   */

  File figFile = SD.open("CONFIG.txt", FILE_READ);
   
  if(figFile){
    for(int i = 0; i<NUMSETTINGS; i++){
      while(figFile.available() && figFile.peek() != '_'){
        figFile.read(); //skip character until it is '_', denoting the beginning of a setting
      }
      figFile.read(); //skip '_' character
      while(figFile.available() && figFile.peek() != ' '){
        settings[i][0] += String(figFile.read()); //add character to setting name;
      }
      if(figFile.available()){
        figFile.read(); //ignore space between setting name and value
      }
      while(figFile.available() && figFile.peek() != ';'){ // ';' char marks the end of a setting
        settings[i][1] += figFile.read(); //add the char to the setting value string
      }
      
    }
    return true;
  }
  else return false;
}

void updateSettings(){
  //This is where settings ripped from the config file are updated. Since all values are strings,
  //setting-specific blocks convert the string values to integers if necessary using toInt().
  
  for(int i=0; i<NUMSETTINGS; i++){
    String cmd = settings[i][0];
      
      if(cmd == "LOGMODE"){
        LOGMODE = settings[i][1].toInt();
      }
        
      else if(cmd == "LOGINT"){
        LOGINT = abs(settings[i][1].toInt()) * 1000;
      }
        
      else if(cmd == "FILENAME"){
        FILENAME = settings[i][1];
      }
  }
}
