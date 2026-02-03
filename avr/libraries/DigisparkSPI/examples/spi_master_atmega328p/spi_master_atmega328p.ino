 //Master Arduino Code:
//SPI MASTER (ARDUINO)
//SPI COMMUNICATION BETWEEN TWO ARDUINO 
//CIRCUIT DIGEST

//edited by Ian Walsh for testing with ATTiny85 SPI (via USI)


#include<SPI.h>                             //Library for SPI 
#define LED 13           
#define ipbutton 2
int buttonvalue;
int x;
void setup (void)
{
  Serial.begin(115200);                   //Starts Serial Communication at Baud Rate 115200 
  //pinMode(ipbutton,INPUT);                //Sets pin 2 as input 
  pinMode(LED,OUTPUT);                    //Sets pin 7 as Output
  SPI.begin();                            //Begins the SPI commnuication
  SPI.setClockDivider(SPI_CLOCK_DIV4);    //Sets clock for SPI communication at 8 (16/8=2Mhz)
  digitalWrite(SS,HIGH);                  // Setting SlaveSelect as HIGH (So master doesnt connnect with slave)
}
void loop(void)
{
  byte Mastersend,Mastereceive;          
  buttonvalue++;   //Reads the status of the pin 2
  digitalWrite(SS, LOW);                  //Starts communication with Slave connected to master
  Mastersend = buttonvalue;                            
  Mastereceive=SPI.transfer(Mastersend); //Send the mastersend value to slave also receives value from slave
  digitalWrite(SS, HIGH);

  Serial.println(Mastereceive);
  delay(1000);
}