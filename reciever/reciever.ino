#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Change the I2C address if needed (e.g., try 0x3F or 0x26 if 0x27 doesn't work)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  Serial.begin(9600);

  // Initialize I2C for ESP32 (GPIO21 = SDA, GPIO22 = SCL)
  Wire.begin(21, 22);

  lcd.init();          // Use init() instead of begin() for most LiquidCrystal_I2C libraries
  lcd.backlight();
  lcd.clear();

  // Initialize LoRa (setPins: NSS/CS, RST, DIO0)
  LoRa.setPins(5, 15, 2);  
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed!");
    lcd.print("LoRa Failed!");
    while (1);
  }

  Serial.println("LoRa Ready - Receiver");
  lcd.print("Waiting for msg");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    Serial.print("Received: ");
    Serial.println(received);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Received:");
    lcd.setCursor(0, 1);
    lcd.print(received);
  }
}
