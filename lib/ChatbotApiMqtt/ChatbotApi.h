#ifndef CHATBOT_API
#define CHATBOT_API

#include <Arduino.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>


class ChatbotAPi{
    private:
        String auth_token;
        String device_id;

    public:
        ChatbotAPi();
        void begin(String id, String token);
        String updateData(String pin, String value);

};

#endif