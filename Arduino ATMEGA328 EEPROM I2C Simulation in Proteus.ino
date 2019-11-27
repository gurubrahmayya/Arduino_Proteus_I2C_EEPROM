#include <Wire.h>    
#define disk1 0x50    //Address of 24LC256 eeprom chip, when A0, A1 & A2 all are GND
byte randNumber;
byte read_data;
void setup(void)
{
  Serial.begin(9600);
  Wire.begin();  
  unsigned int address = 0;
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
}
void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
  delay(5);
}
byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress,1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void loop() 
{
  for (int i=0; i<10; i++)
  {
    randNumber = random(255);
    writeEEPROM(disk1, i, randNumber);
    Serial.print("Random data to write: ");
    Serial.print(randNumber, HEX);
    Serial.println(" Into memory location: "+String(i));
    delay(500);
    read_data = readEEPROM(disk1, i);
    delay(500);
    if(randNumber==read_data)
    {
      Serial.print("Successfull read data: ");
      Serial.print(read_data, HEX);
      Serial.println(" From address location: "+String(i));
      digitalWrite(8,LOW);
    }
    else
    {
      Serial.print("Failed read data: ");
      Serial.print(read_data, HEX);
      Serial.println(" From address location: "+String(i));
      digitalWrite(8,HIGH);
    }
    delay(1000);
  }
}