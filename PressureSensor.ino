
//I2C commands, ripped from the datasheet's "figure 2".
const byte MS_RESET =              0x1E;
const byte MS_CONVERTD1_256 =      0x40;
const byte MS_CONVERTD1_512 =      0x42;
const byte MS_CONVERTD1_1024 =     0x44;
const byte MS_CONVERTD1_2048 =     0x46;
const byte MS_CONVERTD1_4096 =     0x48;
const byte MS_CONVERTD2_256 =      0x50;
const byte MS_CONVERTD2_512 =      0x52;
const byte MS_CONVERTD2_1024 =     0x54;
const byte MS_CONVERTD2_2048 =     0x56;
const byte MS_CONVERTD2_4096 =     0x58;
const byte MS_ADCREAD =            0x00;
const byte MS_PROMREAD0 =          0xA0;

uint16_t MS_PROM[8]; //storing calibration data

//I2C Address
const byte MS_ADDRESS=             0x77;

float tempC = 0;//compensated, final temperature
unsigned long tempNC = 0;//uncompensated, raw temp value
float pressureC = 0;//compensated, final pressure
unsigned long pressureNC = 0;//uncompensated, raw pressure value


void initMS(){
  Serial.begin(9600);
  Wire.begin();
  PSreset();
  if(Serial) Serial.println("Initialized");
  Serial.end();
}

void PSreset(){
  Wire.beginTransmission(MS_ADDRESS);
  Wire.write(MS_RESET);
  Wire.endTransmission();
  delay(10);
  
  //get PROM values
  for(int i=0; i<8; i++){
    //we want to read PROM variable 'i'
    Wire.beginTransmission(MS_ADDRESS);
    Wire.write(MS_PROMREAD0 + 2*i);
    Wire.endTransmission();

    //each PROM variable is stored in two bytes
    Wire.requestFrom(MS_ADDRESS, 2);
    /*if(Wire.available()){
      MS_PROM[i] = Wire.read();
    }
    Serial.print("PROM: " + String(MS_PROM[i]) + ", ");
    if(Wire.available()){
      MS_PROM[i] = uint16_t(MS_PROM[i]<<8) + uint16_t(Wire.read());
      Serial.println(String(MS_PROM[i]));
    }
    */
    if(Wire.available()){
      MS_PROM[i] = Wire.read()<<8;
    }
    if(Wire.available()){
      MS_PROM[i] += Wire.read();
    }
    if(Serial) Serial.println(MS_PROM[i]);
  }
}

bool readPressure(){
  Wire.beginTransmission(MS_ADDRESS);//start a transmission sequence
  Wire.write(MS_CONVERTD1_4096);
  Wire.endTransmission();

  delay(10); //4096 OSR conversion requires 9ms max.
  
  Wire.beginTransmission(MS_ADDRESS);
  Wire.write(MS_ADCREAD);
  Wire.endTransmission();

  Wire.requestFrom(MS_ADDRESS, 3);
  if(!Wire.available()) return false;
  else{
    
    int i = 0;
    while(Wire.available() && i<3){
      pressures[i] = Wire.read();
      i++;
    }
    
    //concatenate the three sets of seven bits
    //p1 represents the MSB, p3 represents the LSB
    unsigned long p0 = pressures[0] << (2*8);
    unsigned long p1 = pressures[1] << (8);
    pressureNC = p0 + p1 + pressures[2];
    
    return true; 
  }

}

bool readTemp(){

  // tell the sensor to compensate and convert the temperature values
  // and store them on the ADC register.
  Wire.beginTransmission(MS_ADDRESS);//start a transmission sequence
  Wire.write(MS_CONVERTD2_4096);
  Wire.endTransmission();

  delay(10); //4096 OSR conversion requires 9ms max.

  // tell the sensor we want to prepare to send the ADC values
  Wire.beginTransmission(MS_ADDRESS);
  Wire.write(MS_ADCREAD);
  Wire.endTransmission();
  
  // tell the sensor to send over the ADC values
  Wire.requestFrom(MS_ADDRESS, 3);
  if(!Wire.available()) return false;
  else{
    int i = 0;
    while(Wire.available() && i<3){
      temperatures[i] = Wire.read();
      i++;
    }
    
    unsigned long t0 = temperatures[0] << (2*8);
    unsigned long t1 = temperatures[1] << (8);
    tempNC = (t0 + t1 + temperatures[2]);
    
    return true;
  }
}


void calc_Temp_Press(){
  
  int32_t dT = 0;
  pressureNC = 0;
  tempNC = 0;
  
  readTemp();
  
  //calculate compensated temperature
  dT = tempNC - MS_PROM[5] * uint16_t(1<<8);
  tempC = (2000 + (dT * MS_PROM[6]) / uint32_t(1<<23))/100.00;

  readPressure();

  //calculate temperature-compensated pressure
  int64_t OFF = MS_PROM[2]* uint32_t(1<<16) + (MS_PROM[4]*dT)/ uint16_t(1<<7);
  
  int64_t SENS = MS_PROM[1]* uint16_t(1<<15) + (MS_PROM[3]*dT)/ uint16_t(1<<8); 
  pressureC = (((pressureNC * SENS / uint32_t(1<<21)) - OFF) / uint16_t(1<<15));
    
}

bool update_Temp_Press(){
  //use this to get the temperature and pressure everytime
  
  if(readTemp()){ //request raw temperature reading
    if(readPressure()){ //request raw pressure reading
      calc_Temp_Press(); //calculate the compensated pressure and temperature
      return true;
    }
  }
  return false;
}


