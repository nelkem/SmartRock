// InitializeSettings.ino: *******************************************************
bool checkConfig();
bool getSettings();
void updateSettings();

// PressureSensor.ino: *******************************************************
void initMS();
void PSreset();
bool readPressure();
bool readTemp();
void calc_Temp_Press();
bool update_Temp_Press();

// SerialComms.ino: *******************************************************
void serialCheck();
void runCommand(String, bool);
void printHelp();

// SmartRock-DataLogging.ino: *******************************************************
void logDataPoint(double, String);
void readAllData(String);
bool initSD(int);
bool writeToFile(String, String);
String readFromFile(String, int);
bool checkTime();

// bluetoothComms.ino: *******************************************************
void bleSetup();
void bleReceiveCommand(bool);
