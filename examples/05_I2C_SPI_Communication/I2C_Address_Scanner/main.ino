#include <Wire.h>
#include <LiquidCrystal_I2C.h>
  
//LCD Object
LiquidCrystal_I2C lcd(0x27, 16, 2); 


void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA, SCL

  // LCD Setup for Display
  lcd.init();
  lcd.backlight();
  
  Serial.println("Scanning...");


}

void loop() {

  // I2C Address Communication
  // Note: Continuous scanning is for demonstration only

  for (byte address = 1; address < 127; address++) { 

    // Find I2C Address
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Found device at 0x");
      Serial.println(address, HEX);
    }
  }

  //Display Words In LCD
  lcd.setCursor(0,0);
  lcd.print("Found:");
  lcd.setCursor(0,1);
  lcd.print("0x27");


  delay(1000);
}
