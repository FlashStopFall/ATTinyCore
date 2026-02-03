//Ian Walsh
//SPI slave using ATTiny85's USI.
//Working correctly up to 4MHz (recieving)

#include <DigiCDC.h>

volatile byte spiData;
volatile bool hadSpiInterrupt = false;
volatile unsigned long lastSpiTime = 0;
byte slaveCounter = 0; // Data to send back to Master

void setup() {                
  SerialUSB.begin(); 
  spiSlaveInit();
}

void loop() {
  if (hadSpiInterrupt) {
    SerialUSB.print("Received: ");
    SerialUSB.println(spiData);
    
    hadSpiInterrupt = false;
    lastSpiTime = millis(); 
  }

  // TIMEOUT SAFETY
  // If connection is lost/noisy, reset the USI to realign bit boundaries
  if (millis() - lastSpiTime > 100) {
      USISR = (1<<USIOIF); 
      lastSpiTime = millis(); 
  }
  
  SerialUSB.refresh(); 
}

ISR(USI_OVF_vect) {
  // 1. READ the received byte
  spiData = USIDR;
  hadSpiInterrupt = true;

  // 2. LOAD the NEXT byte to send
  // We increment a counter so the Master sees changing numbers (0, 1, 2...)
  // This proves we are actively sending, not just echoing.
  slaveCounter++;
  USIDR = slaveCounter; 
  
  // 3. CLEAR flag and RESET counter
  USISR = (1<<USIOIF); 
}

void spiSlaveInit() {
  pinMode(1, OUTPUT); // MISO 
  pinMode(0, INPUT);  // MOSI
  pinMode(2, INPUT);  // SCK 
  
  // CLEAR the data register initially so we don't send garbage first
  USIDR = 0;

  // *** THE CLOCK FIX ***
  // Previously we used (1<<USICS1). 
  // Adding (1<<USICS0) changes the edge sensitivity. 
  // This often fixes the "Left Shift" issue on ATtiny85.
  USICR = (1<<USIWM0)  // Three-wire mode (SPI)
         |(1<<USIOIE)  // Enable Interrupt
         |(1<<USICS1) | (0<<USICS0); // External Clock (Negative Edge / Both Edges logic)
         
  USISR = (1<<USIOIF); // Clear flags
}