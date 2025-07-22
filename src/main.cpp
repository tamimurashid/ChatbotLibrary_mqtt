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
  float temperate = dht.readTemperature();
  float humidity = dht.readHumidity();
  int smoke = digitalRead(SMOKE_PIN);
  int flame = digitalRead(FLAME_PIN);

  /* ==== Prepare data for chatbot transmission ====
    In this section, sensor values are formatted as strings 
    and appended with their respective units. This ensures 
    the data is properly structured for display and interpretation 
    in the chatbot interface.
  */
  String tempStr = String(temperate ) + "°C";
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

  if(flameStatus = LOW){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);
  }else{
    digitalWrite(BUZZER_PIN, LOW);
  }
  if(smokeStatus = HIGH){
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
    delay(1000);
  }

    

}