#include "ChatbotAPI.h"
#include <WiFi.h>
#include <WiFiClient.h>


WiFiClient wifiClient;

PubSubClient mqttClient(wifiClient); 

// Constructor
ChatbotAPI::ChatbotAPI() {
    // No action required
}

void ChatbotAPI::begin(String id, String token) {
    device_id = id;
    auth_token = token;

    // Set MQTT Broker IP and port
    mqttClient.setServer("192.168.10.120", 1883);  // 👈 Your MQTT broker IP
}

String ChatbotAPI::updateData(String pin, String value) {
    // Ensure MQTT is connected
    if (!mqttClient.connected()) {
        while (!mqttClient.connected()) {
            Serial.print("Connecting to MQTT broker...");
            if (mqttClient.connect("ESP32_Client", device_id.c_str(), auth_token.c_str())) {
                Serial.println("connected!");
            } else {
                Serial.print("failed, rc=");
                Serial.println(mqttClient.state());
                delay(2000);
            }
        }
    }

    mqttClient.loop();  // Always call this in your loop or right after publish

    StaticJsonDocument<256> jsonDoc;
    jsonDoc["device_id"] = device_id;
    jsonDoc["auth_token"] = auth_token;
    jsonDoc["virtualPin"] = pin;
    jsonDoc["value"] = value;

    char buffer[256];
    serializeJson(jsonDoc, buffer);

    bool success = mqttClient.publish("device/update", buffer);

    if (success) {
        return "MQTT Publish Success";
    } else {
        return "MQTT Publish Failed";
    }
}
