// PIN DEFINITIONS

#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    

const int SD_ChipSelect = 12;



/*
 * *******************************************************************************************
 */
// GLOBAL SETTINGS:
#define VERBOSE false
#define NUMSETTINGS 3
String settings [NUMSETTINGS][2]; // for storing strings of settings ripped from the config file

// In config file
unsigned long LOGMODE  = 0; // 1 to enable logging, 0 or other to disable logging
unsigned long LOGINT   = 60000; //60 seconds is the default sample interval
String        FILENAME = "LOG1.txt";

// Not yet in config file
const uint8_t SIZE = 3;
byte pressures[SIZE];
byte temperatures[SIZE];

/*
 * *******************************************************************************************
 */
bool receivedCommand = false;
bool bleIsInit = false;

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST); 
