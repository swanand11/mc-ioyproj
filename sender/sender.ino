#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD settings
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);

  Wire.begin(21, 22);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();

  LoRa.setPins(5, 15, 2);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed!");
    lcd.print("LoRa Failed!");
    while (1);
  }

  Serial.println("LoRa Ready");
  lcd.print("LoRa Ready");

  delay(1000);
  lcd.clear();
  lcd.print("Sent msgs:");

  // Send msg1
  sendMessage("msg1");
  delay(1000);

  // Send msg2
  sendMessage("msg2");
  delay(1000);

  lcd.setCursor(0,1);
  lcd.print("Done sending");
}

void loop() {
  // Nothing to do here
}

void sendMessage(String message) {
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  Serial.print("Sent: ");
  Serial.println(message);

  lcd.setCursor(0, 1);
  lcd.print(message + "        ");  // Clear trailing chars
}
