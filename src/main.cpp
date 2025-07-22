#include <Arduino.h>
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

    //==== intialize chatbot object ==== 
    chatbot.begin(device_id, auth_token);

  //==== dht sensor initialize =====
  dht.begin();
  

  // === establish wifi connection ====
  WiFi.begin(ssid, password);
  Serial.println("Connecting to wifi .....");
  while(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(500);
  }
  Serial.println("\n Wifi connected ..");

// === device pin setup =======
  pinMode(FLAME_PIN, INPUT);
  pinMode(SMOKE_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);


  // === lcd intialization 
  lcd.init();
  lcd.backlight();

   // Welcome message on LCD
  lcd.setCursor(0, 0);
  lcd.print(" Smartfy Safety ");
  lcd.setCursor(0, 1);
  lcd.print("  Monitoring   ");
  delay(2000);
  lcd.clear();
  
}

// ==== loop funtion =========

void loop(){


  // == reading data from sensor ===
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int smoke = digitalRead(SMOKE_PIN);
  int flame = digitalRead(FLAME_PIN);

  /* ==== Prepare data for chatbot transmission ====
    In this section, sensor values are formatted as strings 
    and appended with their respective units. This ensures 
    the data is properly structured for display and interpretation 
    in the chatbot interface.
  */
  String tempStr = String(temperature ) + "°C";
  String humStr = String(humidity) + "%";
  String smokeData = String(smoke) + "ppm";

  //======= This prepare  data with logic within it ======
  String smokeStatus = (smoke  ==  LOW) ? "Smoke detected" : "No Smoke detected";
  String flameStatus = (flame  ==  HIGH) ? "Smoke detected" : "No Smoke detected";


  // ==== send data to the chatbot ===

  chatbot.updateData("V1",tempStr);
  chatbot.updateData("V2",humStr);
  chatbot.updateData("V3",smokeData);
  chatbot.updateData("V4",smokeStatus);
  chatbot.updateData("V5",flameStatus);

  // === handle alarm tone ====

  // Handle buzzer alarm
  if (flame == LOW || smoke == HIGH) {
    digitalWrite(BUZZER_PIN, HIGH);  // Alarm ON
  } else {
    digitalWrite(BUZZER_PIN, LOW);   // Alarm OFF
  }



   // Print status to Serial Monitor
  Serial.println("Sensor Data Sent:");
  Serial.println("Temp: " + tempStr);
  Serial.println("Humidity: " + humStr);
  Serial.println("Flame: " + flameStatus);
  Serial.println("Smoke: " + smokeStatus);
  Serial.println("-----------------------------");

  // Display on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1);
  lcd.print(" C    ");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity, 1);
  lcd.print("%     ");

  lcd.setCursor(0, 2);
  lcd.print("Flame: ");
  lcd.print(flameStatus);
  lcd.print("    ");

  lcd.setCursor(0, 3);
  lcd.print("Smoke: ");
  lcd.print(smokeStatus);
  lcd.print("    ");

  delay(1000);  // Update every 1 seconds

    

}