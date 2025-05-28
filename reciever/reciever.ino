#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "keys.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
String senderDevice = "esp32_sender";  


void setup() {
  Serial.begin(9600);
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  LoRa.setPins(5, 15, 2);  
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed!");
    lcd.print("LoRa Failed!");
    while (1);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  lcd.clear();
  lcd.print("WiFi Connected");
  Serial.println("LoRa Ready - Receiver");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");
}

bool isAuthenticated(String deviceId) {
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
  
  String payload = "{\"device_id\":\"" + deviceId + "\"}";
  int httpCode = http.POST(payload);

  String response = http.getString();
  http.end();

  Serial.println("Auth status: " + response);
  return (httpCode == 200 && response == "yes");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }

    Serial.print("Received LoRa: ");
    Serial.println(received);

    // Check auth
    if (isAuthenticated(senderDevice)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("From Auth Dev:");
      lcd.setCursor(0, 1);
      lcd.print(received);
    } else {
      Serial.println("Sender not authenticated. Ignoring message.");
      lcd.clear();
      lcd.print("Untrusted Device");
    }
  }

  delay(100);  // Small delay for stability
}
