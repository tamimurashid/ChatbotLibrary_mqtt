#ifndef CHATBOT_API_H
#define CHATBOT_API_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>



class ChatbotAPI{
    private:
        String auth_token;
        String device_id;

    public:
        ChatbotAPI();
        void begin(String id, String token);
        String updateData(String pin, String value);

};

#endif