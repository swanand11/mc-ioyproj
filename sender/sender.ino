#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "keys.h"
#include <LiquidCrystal_I2C.h>
#include <LoRa.h>

// Initialize LCD, adjust I2C address and size if needed
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);

  // Initialize LCD
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Connecting WiFi");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  lcd.clear();
  lcd.print("WiFi Connected");

  // Initialize LoRa - set pins NSS=5, RESET=15, DIO0=2 or your wiring
  LoRa.setPins(5, 15, 2);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed!");
    lcd.clear();
    lcd.print("LoRa Failed!");
    while (true);
  }
  Serial.println("LoRa Initialized");
  delay(1000);
  lcd.clear();
}

void loop() {
  for(int i=0;i<10;i++){
    String message= "Hello" + String(i);
  

  // Send via LoRa
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
  Serial.println("LoRa Sent: " + message);

  // Display on LCD
  lcd.clear();
  lcd.print("Sent via LoRa:");
  lcd.setCursor(0, 1);
  lcd.print(message);
  
  // Send HTTP POST request
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"device_id\":\"esp32_sender\",\"message\":\"" + message + "\"}";
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response: " + response);
    } else {
      Serial.println("HTTP POST Error: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(3000); // wait 3 seconds before next send
  }

}
