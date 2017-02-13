/*
    ----------------------------------------------
    Title:   E_FUSE.ino
    Author:  Martin Pålsson
    Description:
    E-Fuse programmer for SHARP GP2Y0E02B, GP2Y0E03
    -----------------------------------------------
*/
    
#include <Wire.h>

/* Default address of sensor */
#define ADDRESS (0x80 >> 1) // 7 MSB, default address of sensor

/* Desired address of the sensor */
#define SETADDR 0xE0        // 4 MSB, will bitshift when used.

/*  Prototypes  */
int ScanBus();
uint8_t readSequence1(uint8_t devAddr, uint8_t regAddr);
void writeSequence(uint8_t devAddr, uint8_t regAddr, uint8_t data);
void EFuseSlaveID(uint8_t newID);

/* Global variables */
//uint8_t checkSum = 0; //  Not currently used
void setup()
{
  pinMode(4, OUTPUT);       //  Connect to Vpp pad on sensor
  pinMode(13, OUTPUT);      //  Onboard LED
  digitalWrite(13, HIGH);   //  To indicate that serial communication has been established. No led - Connection to PC established.
  Wire.begin();
 
  Serial.begin(9600);
  
  while (!Serial);             // Due: wait for serial monitor
  digitalWrite(13, LOW);       // To acknowledge that serial communication has been established.
  
  Serial.println("\nGP2Y0E02B E-Fuse Programmer Version 1.0");
}
void loop()
{
  Serial.println("Scanning I2C bus...");
  if(ScanBus())
  {
    EFuseSlaveID(byte(SETADDR));
  }
  else
  {
    Serial.println("No devices found on bus");
  }
  /* Nothing else to do */ 
  while(1);;
}
void EFuseSlaveID(uint8_t newID)
{
    /* ----- Stage 1 ----- */
  Serial.println("Stage 1 started.");
  
  writeSequence(byte(ADDRESS), byte(0xEC), byte(0xFF));
  Serial.println("Data = 0xFF is set in Address = 0xEC");
  
  digitalWrite(4, HIGH);
  Serial.println("3.3V is applied in the Vpp terminal");
  
  /* ----- Stage 2 ------ */
  Serial.println("Stage 2 started.");
  writeSequence(byte(ADDRESS), byte(0xC8), byte(0x00));
  Serial.println("Data = 0x00 is set in Address = 0xC8");
  
  /* ----- Stage 3 ------ */
  Serial.println("Stage 3 started.");
  writeSequence(byte(ADDRESS), byte(0xC9), byte(0x45));
  Serial.println("Data = 0x45 is set in Address = 0xC9");

  /* ----- Stage 4 ------ */
  /* THIS IS WHERE THE ADDRESS WILL BE SET! */
  Serial.println("Stage 4 started.");
  writeSequence(byte(ADDRESS), byte(0xCD), byte(newID >> 4));
  Serial.println("Data = SETADDR >> 4 is set in Address = 0xCD");


  /* ----- Stage 5 ------ */
  Serial.println("Stage 5 started.");
  writeSequence(byte(ADDRESS), byte(0xCA), byte(0x01));
  Serial.println("Data = 0x01 is set in Address = 0xCA");
  Serial.println("Wait for 500 us");
  delayMicroseconds(500);

  /* ----- Stage 6 ------ */
  Serial.println("Stage 6 started.");
  writeSequence(byte(ADDRESS), byte(0xCA), byte(0x00));
  Serial.println("Data = 0x00 is set in Address = 0xCA");
  digitalWrite(4, LOW); 
  Serial.println("Vpp terminal grounded.");

  /* ----- Stage 7 ------ */
  Serial.println("Stage 7 started.");
  writeSequence(byte(ADDRESS), byte(0xEF), byte(0x00));
  Serial.println("Data = 0x00 is set in Address = 0xEF");
  
  writeSequence(byte(ADDRESS), byte(0xC8), byte(0x40));
  Serial.println("Data = 0x40 is set in Address = 0xC8");
  
  writeSequence(byte(ADDRESS), byte(0xC8), byte(0x00));
  Serial.println("Data = 0x00 is set in Address = 0xC8");

  /* ----- Stage 8 ------ */
  Serial.println("Stage 8 started.");
  writeSequence(byte(ADDRESS), byte(0xEE), byte(0x06));
  Serial.println("Data = 0x06 is set in Address = 0xEE");

  /* ----- Stage 9 ------ */
  Serial.println("Stage 9 started.");
  writeSequence(byte(ADDRESS), byte(0xEC), byte(0xFF));
  Serial.println("Data = 0xFF is set in Address = 0xEC");
  
  writeSequence(byte(ADDRESS), byte(0xEF), byte(0x03));
  Serial.println("Data = 0x03 is set in Address = 0xEF");

  Serial.println("Read out the data in Address = 0x27.");
  Serial.print("Data: 0B");
  Serial.println(readSequence1(byte(ADDRESS), byte(0x27)), BIN);

  writeSequence(byte(ADDRESS), byte(0xEF), byte(0x00));
  Serial.println("Data = 0x00 is set in Address = 0xEF");
  
  writeSequence(byte(ADDRESS), byte(0xEC), byte(0x7F));
  Serial.println("Data = 0x7F is set in Address = 0xEC");

//  checkSum = (uint8_t)readSequence1(byte(ADDRESS), byte(0x27));
//
//  if((checkSum % 16) != 1)
//  {
//    Serial.println("ERROR!   >:(\nE-fuse probably broken.");
//    Serial.print("Data: 0B");
//    Serial.println(readSequence1(byte(ADDRESS), byte(0x27)), BIN);
//    
//    /* ----- Stage 10 - 1 ----- */
//    Serial.println("Stage 10 - 1 started.");
//    writeSequence(byte(ADDRESS), byte(0xEC), byte(0xFF));
//    Serial.println("Data = 0xFF is set in Address = 0xEC");
//    digitalWrite(4, HIGH);
//    Serial.println("3.3V is applied in Vpp terminal");
//    
//    /* ----- Stage 10 - 2 ----- */
//    Serial.println("Stage 10 - 2 started.");
//    writeSequence(byte(ADDRESS), byte(0xC8), byte(0x37));
//    Serial.println("Data = 0x37 is set in Address = 0xC8");
//
//    /* ----- Stage 10 - 3 ----- */
//    Serial.println("Stage 10 - 3 started.");
//    writeSequence(byte(ADDRESS), byte(0xC9), byte(0x74));
//    Serial.println("Data = 0x74 is set in Address = 0xC9");
//
//    /* ----- Stage 10 - 4 ----- */
//    Serial.println("Stage 10 - 4 started.");
//    writeSequence(byte(ADDRESS), byte(0xCD), byte(0x04));
//    Serial.println("Data = 0x04 is set in Address = 0xCD");
//
//    /* ----- Stage 10 - 5 ----- */
//    Serial.println("Stage 10 - 5 started.");
//    writeSequence(byte(ADDRESS), byte(0xCA), byte(0x01));
//    Serial.println("Data = 0x01 is set in Address = 0xCA");
//    delayMicroseconds(500);
//    Serial.println("Wait for 500 us.");
//
//    /* ----- Stage 10 - 6 ----- */
//    Serial.println("Stage 10 - 6 started.");
//    writeSequence(byte(ADDRESS), byte(0xCA), byte(0x00));
//    Serial.println("Data = 0x00 is set in Address = 0xCA");
//    digitalWrite(4, LOW);
//    Serial.println("Vpp terminal is grounded.");
//
//    /* ----- Stage 10 - 1' ----- */
//    Serial.println("Stage 10 - 1' started.");
//    writeSequence(byte(ADDRESS), byte(0xEC), byte(0xFF));
//    Serial.println("Data = 0xFF is set in Address = 0xEC");
//    digitalWrite(4, HIGH);
//    Serial.println("3.3V is applied in Cpp terminal");
//    
//    /* ----- Stage 10 - 2' ----- */
//    Serial.println("Stage 10 - 2' started.");
//    writeSequence(byte(ADDRESS), byte(0xC8), byte(0x3F));
//    Serial.println("Data = 0x3F is set in Address = 0xC8");
//
//    /* ----- Stage 10 - 3' ----- */
//    Serial.println("Stage 10 - 3' started.");
//    writeSequence(byte(ADDRESS), byte(0xC9), byte(0x04));
//    Serial.println("Data = 0x04 is set in Address = 0xC9");
//
//    /* ----- Stage 10 - 4' ----- */
//    /* THIS IS WHERE THE ADDRESS IS PROGRAMMED */
//    Serial.println("Stage 10 - 4' started.");
//    writeSequence(byte(ADDRESS), byte(0xCD), byte(newID >> 4));
//    Serial.println("Data = 0x08 is set in Address = 0xCD");
//
//    /* ----- Stage 10 - 5' ----- */
//    Serial.println("Stage 10 - 5' started.");
//    writeSequence(byte(ADDRESS), byte(0xCA), byte(0x01));
//    Serial.println("Data = 0x01 is set in Address = 0xCA");
//    delayMicroseconds(500);
//    Serial.println("Wait for 500 us.");
//
//    /* ----- Stage 10 - 6' ----- */
//    Serial.println("Stage 10 - 6' started.");
//    writeSequence(byte(ADDRESS), byte(0xCA), byte(0x00));
//    Serial.println("Data = 0x00 is set in Address = 0xCA");
//    digitalWrite(4, LOW);
//    Serial.println("Vpp terminal is grounded.");
//
//    /* ----- Stage 10 - 7 ------ */
//    Serial.println("Stage 10 - 7 started.");
//    writeSequence(byte(ADDRESS), byte(0xEF), byte(0x00));
//    Serial.println("Data = 0x00 is set in Address = 0xEF");
//    
//    writeSequence(byte(ADDRESS), byte(0xC8), byte(0x40));
//    Serial.println("Data = 0x40 is set in Address = 0xC8");
//    
//    writeSequence(byte(ADDRESS), byte(0xC8), byte(0x00));
//    Serial.println("Data = 0x00 is set in Address = 0xC8");
//  
//    /* ----- Stage 10 - 8 ------ */
//    Serial.println("Stage 10 - 8 started.");
//    writeSequence(byte(ADDRESS), byte(0xEE), byte(0x06));
//    Serial.println("Data = 0x06 is set in Address = 0xEE");
//
//    /* ------ Stage 10 - 9 ----- */
//    Serial.println("Stage 10 - 9 started.");
//    writeSequence(byte(ADDRESS), byte(0xEC), byte(0xFF));
//    Serial.println("Data = 0xFF is set in Address = 0xEC");
//    writeSequence(byte(ADDRESS), byte(0xEF), byte(0x03));
//    Serial.println("Data = 0x03 is set in Address = 0xEF");
//    
//    uint8_t x18Val = readSequence1(byte(ADDRESS), byte(0x18));
//    uint8_t x19Val = readSequence1(byte(ADDRESS), byte(0x19));
//
//    Serial.print("0x18 = ");
//    Serial.print(x18Val, HEX);
//    Serial.print("\tx19Val = ");
//    Serial.println(x19Val, HEX);
//
//    if(x18Val != 0x82 || x19Val != 0x00)
//    {
//      Serial.println("Not possible to correct error.");
//    }
//    else
//    {
//      Serial.println("E-Fuse programming finished with bit replacement.");
//    }
//  }
//  else
//  {
//    Serial.println("E-Fuse programming finished.");
//  }
}
void writeSequence(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
  Wire.beginTransmission(devAddr);
  Wire.write(byte(regAddr));
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t readSequence1(uint8_t devAddr, uint8_t regAddr)
{
  Wire.beginTransmission(devAddr);
  Wire.write(byte(regAddr));
  Wire.endTransmission();

  Wire.requestFrom(devAddr, 1);

  return Wire.read();
}

int ScanBus()
{
  byte error, address;
  int nDevices;
 
  //Serial.println("I2C Scanner starting...");
 
  nDevices = 0;
  for(address = 0; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address << 1,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
  {
    Serial.println("No I2C devices found\n");
    return 0;
  }
  else
  {
    Serial.print(nDevices);
    Serial.println(" devices found.");
    Serial.println("I2C Scanner done.\n");
    return nDevices;
  }
}
