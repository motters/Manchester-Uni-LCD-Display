/*################################################
 # Program by Sam Mottley
 # Email Info@SamMottley.Co.Uk
 # CopyRight All Rights Reserved
 #
 ##################################################*/
/*
 * Include the relivent librarys
 */
  // LCD controll library
  #include <LiquidCrystal.h>
  // 12 bit A/D library 

/*
 * Set global varblies to be used across the program
 */
  //Set analogue inputs
  #define SELPIN 10 //Selection Pin 
  #define DATAOUT 11//MOSI 
  #define DATAIN  13//MISO 
  #define SPICLOCK  12//Clock
  int readSingleADPortArray[] = {0,1,2,3,4,5,6,7};
  float voltages[] = {0,1,2,3,4,5,6,7};
  char* voltagesStrings[] = {"","","","","","","",""};
  float makeToVoltageArray[] = {0,1,2,3,4,5,6,7};
  float voltagesMovingOne[] = {0,1,2,3,4,5,6,7};
  float voltagesMovingTwo[] = {0,1,2,3,4,5,6,7};
  float voltagesMovingThree[] = {0,1,2,3,4,5,6,7};
  float voltagesMovingFour[] = {0,1,2,3,4,5,6,7};
  float voltagesMovingAverage[] = {0,1,2,3,4,5,6,7};
  int readvalue;
  float noiseFilter[] = {0,1,2,3,4,5,6,7};
  float voltagesPolarities[] = {0,1,2,3,4,5,6,7};
  float voltagePercentageTunning[] = {0,1,2,3,4,5,6,7};
  
  //Set Digital inputs
  
  //set LCD display
  LiquidCrystal lcd[2] = {LiquidCrystal(27, 23, 30, 32, 34, 36),LiquidCrystal(27, 25, 30, 32, 34, 36)};  
  char* lcdReadOut = "";
  long previousMillis = 0;
  long updateEvery = 200; //how fast the lcd display is updated

/*
 * Default setup to be ran before loop
 */
void setup(){
  /*
   * Set LCD 
   */
    // set up the LCD's number of columns and rows: 
    lcd[0].begin(40, 2);
    lcd[1].begin(40, 2);
    // Print the welcomeing message to the screen
    lcd[0].print("Electron Gun Suply");
    lcd[0].setCursor(0,1);
    lcd[0].print("Setting up system... Please wait");
    
    lcd[1].print("School of Chemistry");
    lcd[1].setCursor(0,1);
    lcd[1].print("Electronics Phone: 0161 275 4641");
  
  /*
   * Set 12 bit A / D
   */
    //set pin modes 
    pinMode(SELPIN, OUTPUT); 
    pinMode(DATAOUT, OUTPUT); 
    pinMode(DATAIN, INPUT); 
    pinMode(SPICLOCK, OUTPUT); 
    //disable device to start with 
    digitalWrite(SELPIN,HIGH); 
    digitalWrite(DATAOUT,LOW); 
    digitalWrite(SPICLOCK,LOW);
  
  
  /*
   * Set voltages
   */
     //below set your eight voltages. If include a decimal dont forget to float the number
     voltages[0] = (float)4.096;//4.096
     voltages[1] = (float)4.096;
     voltages[2] = (float)4.096;
     voltages[3] = (float)4.096;
     voltages[4] = (float)4.096;
     voltages[5] = (float)4.096;
     voltages[6] = (float)4.096;
     voltages[7] = (float)4.096;
   
   /*
    * Set voltage strings (What are display in front of the voltage on the lcd display)
    */  
      //Set the strings
       voltagesStrings[0] = "Gun En = ";
       voltagesStrings[1] = "Anode    = ";
       voltagesStrings[2] = "GL1A   = ";
       voltagesStrings[3] = "Grid     = ";
       voltagesStrings[4] = "Def 1  = ";
       voltagesStrings[5] = "Def 2    = ";
       voltagesStrings[6] = "Def 3  = ";
       voltagesStrings[7] = "Filament = ";
     
   /*
    * Postive and Negative numbers 
    */
      //Below set the value at which the signal changes from negative to positive
       voltagesPolarities[0] = 0;
       voltagesPolarities[1] = 0;
       voltagesPolarities[2] = 0;
       voltagesPolarities[3] = 0;
       voltagesPolarities[4] = 0;//2048
       voltagesPolarities[5] = 0;
       voltagesPolarities[6] = 0;
       voltagesPolarities[7] = 0;
       
   /*
    * multple to tune results relative to the voltage
    */
      //set any ofsets to tune the system in (EG bad resisitor tolerance etc)
       voltagePercentageTunning[0] = 1; //1.0002
       voltagePercentageTunning[1] = 1;
       voltagePercentageTunning[2] = 1;
       voltagePercentageTunning[3] = 1;
       voltagePercentageTunning[4] = 1;
       voltagePercentageTunning[5] = 1;
       voltagePercentageTunning[6] = 1;
       voltagePercentageTunning[7] = 1;
       
  /*
   * Noise filter relivent to averaged voltage 
   */
     //Set relivent to the voltage out put NOT the digital input
      noiseFilter[0] = 0; 
      noiseFilter[1] = 0;
      noiseFilter[2] = 0;
      noiseFilter[3] = 0;
      noiseFilter[4] = 0;
      noiseFilter[5] = 0;
      noiseFilter[6] = 0;
      noiseFilter[7] = 0;
   
  /*
   * Run throught and get the starting firgures
   */ 
     //Run through four times to get a moving avarage started
     for (int i=3; i>=0; i--){
       collectData();
       makeToVoltage();
       averageInputData();
     }
     delay(4000);
     lcd[0].clear();
     lcd[1].clear();
}

/*
 * Main program loop
 */
void loop(){
  collectData(); //collect the data from ports
  makeToVoltage(); // convert the data to voltage
  averageInputData(); //do the moving average
  
  //  
  unsigned long currentMillis = millis(); 
  if(currentMillis - previousMillis > updateEvery) {
    previousMillis = currentMillis;  
    generateDisplay();
  }
}

/*
 * Generate dispay data
 * @return string
 */
void generateDisplay(){
  lcd[0].setCursor(0,0);
  
  //floatToString(char * outstr, double val, byte precision, byte widthp)
  lcd[0].print(voltagesStrings[0]); 
  lcd[0].setCursor(10,0);
  lcd[0].print("      ");
  lcd[0].setCursor(9,0);
  lcd[0].print(voltagesMovingAverage[0], 3);
  lcd[0].setCursor(16,0);
  lcd[0].print(" V");
  
  lcd[0].print(" |"); 
  
  lcd[0].setCursor(21,0);
  lcd[0].print(voltagesStrings[1]); 
  lcd[0].print(voltagesMovingAverage[1]);
  lcd[0].setCursor(39,0);
  lcd[0].print("V");
  
  lcd[0].setCursor(0,1);

  lcd[0].print(voltagesStrings[2]); 
  lcd[0].print(voltagesMovingAverage[2]);
  lcd[0].setCursor(16,1);
  lcd[0].print(" V");
  
  lcd[0].print(" |");
  
  lcd[0].setCursor(21,1);
  lcd[0].print(voltagesStrings[3]); 
  lcd[0].print(voltagesMovingAverage[3]);
  lcd[0].setCursor(39,1);
  lcd[0].print("V");
  
  lcd[1].setCursor(0,0);
  
  lcd[1].print(voltagesStrings[4]); 
  lcd[1].print(voltagesMovingAverage[4]);
  lcd[1].setCursor(16,0);
  lcd[1].print(" V");
  
  lcd[1].print(" |");
  
  lcd[1].setCursor(21,0);
  lcd[1].print(voltagesStrings[5]); 
  lcd[1].print(voltagesMovingAverage[5]);
  lcd[1].setCursor(39,0);
  lcd[1].print("V");
  
  lcd[1].setCursor(0,1);
  
  lcd[1].print(voltagesStrings[6]); 
  lcd[1].print(voltagesMovingAverage[6]);
  lcd[1].setCursor(16,1);
  lcd[1].print(" V");
 
  lcd[1].print(" |");
  
  lcd[1].setCursor(21,1);
  lcd[1].print(voltagesStrings[7]); 
  lcd[1].print(voltagesMovingAverage[7]);
  lcd[1].setCursor(39,1);
  lcd[1].print("V");

}

/*
 * Read a single A=>D port 
 * @currentData array of current input data
 * @return string
 */
int readSingleADPort(int channel){
  int adcvalue = 0;
  byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3)

  //allow channel selection
  commandbits|=((channel-1)<<3);

  digitalWrite(SELPIN,LOW); //Select adc
  // setup bits to be written
  for (int i=7; i>=3; i--){
    digitalWrite(DATAOUT,commandbits&1<<i);
    //cycle clock
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);    
  }

  digitalWrite(SPICLOCK,HIGH);    //ignores 2 null bits
  digitalWrite(SPICLOCK,LOW);
  digitalWrite(SPICLOCK,HIGH);  
  digitalWrite(SPICLOCK,LOW);

  //read bits from adc
  for (int i=11; i>=0; i--){
    adcvalue+=digitalRead(DATAIN)<<i;
    //cycle clock
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);
  }
  digitalWrite(SELPIN, HIGH); //turn off device
  return adcvalue;
}

/*
 * Collect the current values on the anolgue and digital inputs
 * @return array
 */
void collectData(){
  int portNumber;
  for (int i=7; i>=0; i--){
    portNumber = i;
    readSingleADPortArray[i] = readSingleADPort(portNumber);
  }
}

/*
 * Make input into a voltage 
 * @currentData array of current input data
 * @return float
 */
void makeToVoltage(){
  int portNumber;
  for (int i=7; i>=0; i--){
    portNumber = i;
    if(voltagesPolarities[portNumber] != 0){
      float steps; 
      steps = (((float)readSingleADPortArray[portNumber] - voltagesPolarities[portNumber]) / 4096)*(float)voltages[portNumber];
      makeToVoltageArray[portNumber] = (float)steps;
    }else{
      float steps; 
      steps = ((float)readSingleADPortArray[portNumber] / 4096)*(float)voltages[portNumber];
      makeToVoltageArray[portNumber] = (float)steps;
    }
  }
}

/*
 * Generate a moving average 
 * @currentData array of current input data
 * @return array
 */
void averageInputData(){
  int portNumber;
  
  //Drop four add three
  for (int i=7; i>=0; i--){
    portNumber = i;
    voltagesMovingFour[portNumber] = voltagesMovingThree[portNumber];
  }
  
  //Drop Three add Two
  for (int i=7; i>=0; i--){
    portNumber = i;
    voltagesMovingThree[portNumber] = voltagesMovingTwo[portNumber];
  }
  
  //Drop Two add One
  for (int i=7; i>=0; i--){
    portNumber = i;
    voltagesMovingTwo[portNumber] = voltagesMovingOne[portNumber];
  }
  
  //Drop one and add the new read value
  for (int i=7; i>=0; i--){
    portNumber = i;
    voltagesMovingOne[portNumber] = makeToVoltageArray[portNumber];
  }
  
  //Now average the values
  for (int i=7; i>=0; i--){
    portNumber = i;
    float newAverage = (voltagesMovingOne[portNumber] + voltagesMovingTwo[portNumber] + voltagesMovingThree[portNumber] + voltagesMovingFour[portNumber]) / 4;
    float maxValue = voltagesMovingAverage[portNumber] + noiseFilter[portNumber];
    
    //Filter out noise
    if((float abs(voltagesMovingAverage[portNumber] - newAverage) >= noiseFilter[portNumber]) || (voltagesMovingAverage[portNumber] == 0) || (voltagesMovingAverage[portNumber] == 1) || (voltagesMovingAverage[portNumber] == 2) || (voltagesMovingAverage[portNumber] == 3) || (voltagesMovingAverage[portNumber] == 4) || (voltagesMovingAverage[portNumber] == 5) || (voltagesMovingAverage[portNumber] == 6) || (voltagesMovingAverage[portNumber] == 7)) {   
        voltagesMovingAverage[portNumber] = newAverage * voltagePercentageTunning[portNumber];
    }
    

  }
  
}


