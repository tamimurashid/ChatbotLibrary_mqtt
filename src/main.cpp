#include <ChatbotAPI.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFi.h>

// ===== WIFI CREDENTIAL =======

const char* ssid = "Reindeer";
const char* password = "200120022003";

// ====== DEVICE AUTHENTICATION ===

const char* device_id = "";
const char* auth_token = "";

// ===== DECLARATION ============

// ===== Sensor Pins =====
#define DHTPIN      4
#define DHTTYPE     DHT22
#define FLAME_PIN   35
#define SMOKE_PIN   34
#define BUZZER_PIN  23

// ===== Objects =====
ChatbotAPI chatbot;
LiquidCrystal_I2C lcd(0x27, 20, 4);
DHT dht(DHTPIN, DHTTYPE);


//  ====== SETUP  FUNCTION =====
void setup(){

  //  serial begin for serial communication 
  Serial.begin(115200);

  // === initialization ====
  chatbot.begin(device_id, auth_token);
  WiFi.begin(ssid, password);
  dht.begin();
  lcd.init();
  lcd.backlight();
  

}