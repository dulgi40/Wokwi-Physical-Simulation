#include <SPI.h> // SPI Header

void setup() {
  Serial.begin(115200);


  // SPI Setup (4 PIN, Loopback System)
  SPI.begin(18, 19, 23, 5); 
  Serial.println("SPI Loopback Test Start");
}

void loop() {

  // Start SPI communication setting
  // 1MHz speed, MSB first, Mode 0 (most common mode)
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  // Select SPI slave (CS LOW means communication start)
  digitalWrite(5, LOW);

  // Prepare data to send
  byte sendData = 0x3A;

  // Send 1 byte and receive data at same time (Full-Duplex)
  byte receivedData = SPI.transfer(sendData);

  // Communication end (CS HIGH)
  digitalWrite(5, HIGH);

  // Finish SPI transaction
  SPI.endTransaction();

  // Print send/receive result for checking
  Serial.print("Sent: 0x");
  Serial.print(sendData, HEX);
  Serial.print(" | Received: 0x");
  Serial.println(receivedData, HEX);

  // Wait 1 second for next test
  delay(1000);
}
