//
// A simple server implementation showing how to:
//  * serve a website embedded in flash (PROGMEM)
//  * expose a JSON endpoint for the current numeric value
//  * update the numeric value from newline-terminated JSON over UART
//  * chat over an MQTT "/general" topic, relayed through the web page and UART
//    ({"code":"chat","value":"..."} over UART <-> MQTT, both directions)
//  * handle missing pages / 404s
//

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#endif
#include "PubSubClient.h"
#include "ArduinoJson-v7.4.3.h"
#include "index_html.h"

#ifdef ESP32
WebServer server(80);
#elif defined(ESP8266)
ESP8266WebServer server(80);
#endif

// const char* ssid = "YOUR_SSID";
// const char* password = "YOUR_PASSWORD";

const char* ssid = "DIGI-ehds";
const char* password = "5bfPc4AFzA";

const char* mqttHost = "isilab.cloud.shiftr.io";
const uint16_t mqttPort = 1883;
const char* mqttUser = "isilab";
const char* mqttPassword = "oonhdB7qBZrPK2Lf";
const char* mqttChatTopic = "/general";

#define CHAT_HISTORY_SIZE   10
#define CHAT_MESSAGE_MAX_LEN 96

WiFiClient mqttNetClient;
PubSubClient mqttClient(mqttNetClient);

String chatHistory[CHAT_HISTORY_SIZE];
uint8_t chatHistoryCount = 0;
uint8_t chatHistoryNext = 0;

float displayValue = 0.0f;
String uartBuffer;
unsigned long lastStatusMs = 0;
unsigned long lastWifiAttemptMs = 0;
unsigned long lastMqttAttemptMs = 0;
bool serverStarted = false;

String valueJson() {
    String json = "{\"value\":";
    json += String(displayValue, 2);
    json += "}";
    return json;
}

bool updateValueFromJson(JsonDocument& doc) {
    if (doc["code"] != "data" || !doc["value"].is<float>()) {
        return false;
    }

    displayValue = doc["value"].as<float>();
    return true;
}

bool sendChatFromUartToMqtt(JsonDocument& doc) {
    if (doc["code"] != "chat" || !doc["value"].is<const char*>()) {
        return false;
    }

    String text = doc["value"].as<String>();
    text.trim();
    if (text.length() == 0) {
        return false;
    }
    if (text.length() > CHAT_MESSAGE_MAX_LEN) {
        text = text.substring(0, CHAT_MESSAGE_MAX_LEN);
    }

    return mqttClient.connected() && mqttClient.publish(mqttChatTopic, text.c_str());
}

String jsonEscape(const String& text) {
    String escaped = text;
    escaped.replace("\\", "\\\\");
    escaped.replace("\"", "\\\"");
    return escaped;
}

void printAck(const String& echo) {
    Serial.print("{\"code\":\"ack\",\"echo\":\"");
    Serial.print(jsonEscape(echo));
    Serial.println("\"}");
}

void addChatMessage(const String& message) {
    chatHistory[chatHistoryNext] = message;
    chatHistoryNext = (chatHistoryNext + 1) % CHAT_HISTORY_SIZE;
    if (chatHistoryCount < CHAT_HISTORY_SIZE) {
        chatHistoryCount++;
    }
}

String chatHistoryJson() {
    String json = "[";
    for (uint8_t i = 0; i < chatHistoryCount; i++) {
        uint8_t index = (chatHistoryNext + CHAT_HISTORY_SIZE - chatHistoryCount + i) % CHAT_HISTORY_SIZE;

        if (i > 0) {
            json += ",";
        }
        json += "\"";
        json += jsonEscape(chatHistory[index]);
        json += "\"";
    }
    json += "]";
    return json;
}

void onMqttMessage(char* topic, uint8_t* payload, unsigned int length) {
    (void)topic;

    String message;
    message.reserve(length);
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    addChatMessage(message);

    Serial.print("{\"code\":\"chat\",\"value\":\"");
    Serial.print(jsonEscape(message));
    Serial.println("\"}");
}

String mqttClientId() {
    String id = "esp8266-";
#ifdef ESP32
    id += String((uint32_t)(ESP.getEfuseMac() & 0xFFFFFFFFUL), HEX);
#elif defined(ESP8266)
    id += String(ESP.getChipId(), HEX);
#endif
    return id;
}

void handleMqttConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        return;
    }

    if (mqttClient.connected()) {
        mqttClient.loop();
        return;
    }

    unsigned long now = millis();
    if (now - lastMqttAttemptMs < 5000) {
        return;
    }
    lastMqttAttemptMs = now;

    if (mqttClient.connect(mqttClientId().c_str(), mqttUser, mqttPassword)) {
        mqttClient.subscribe(mqttChatTopic);
    }
}

void handleUartInput() {
    while (Serial.available() > 0) {
        char c = Serial.read();

        if (c == '\r') {
            continue;
        }

        if (c == '\n') {
            uartBuffer.trim();
            if (uartBuffer.length() > 0) {
                JsonDocument doc;
                DeserializationError error = deserializeJson(doc, uartBuffer);
                if (!error) {
                    updateValueFromJson(doc);
                    sendChatFromUartToMqtt(doc);
                    printAck(uartBuffer);
                }
            }
            uartBuffer = "";
            continue;
        }

        if (uartBuffer.length() < 96) {
            uartBuffer += c;
        }
    }
}

void handleRoot() {
    server.send_P(200, "text/html", INDEX_HTML);
}

void printStatus() {
    unsigned long now = millis();
    if (now - lastStatusMs < 1000) {
        return;
    }

    lastStatusMs = now;
    bool wifiReady = WiFi.status() == WL_CONNECTED;
    Serial.print("{\"code\":\"status\",\"data\":\"");
    Serial.print(WiFi.localIP());
    Serial.print("\",\"wifiReady\":");
    Serial.print(wifiReady ? "true" : "false");
    Serial.println("}");
}

void startWifiConnection() {
    WiFi.mode(WIFI_STA);

    if (password == nullptr || password[0] == '\0') {
        WiFi.begin(ssid);
    } else {
        WiFi.begin(ssid, password);
    }
}

void handleWifiConnection() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!serverStarted) {
            server.begin();
            serverStarted = true;
        }
        return;
    }

    unsigned long now = millis();
    if (now - lastWifiAttemptMs < 5000) {
        return;
    }

    lastWifiAttemptMs = now;
    startWifiConnection();
}

void handleValue() {
    server.send(200, "application/json", valueJson());
}

void handleChatGet() {
    server.send(200, "application/json", chatHistoryJson());
}

void handleChatPost() {
    String text = server.arg("plain");
    text.trim();

    if (text.length() == 0) {
        server.send(400, "text/plain", "empty message");
        return;
    }

    if (text.length() > CHAT_MESSAGE_MAX_LEN) {
        text = text.substring(0, CHAT_MESSAGE_MAX_LEN);
    }

    if (!mqttClient.connected() || !mqttClient.publish(mqttChatTopic, text.c_str())) {
        server.send(503, "text/plain", "mqtt not connected");
        return;
    }

    server.send(200, "text/plain", "ok");
}

void notFound() {
    server.send(404, "text/plain", "Not found");
}

void setup() {

    Serial.begin(115200);
    uartBuffer.reserve(96);

    Serial.printf("\n\n");

    lastStatusMs = millis() - 1000;
    lastWifiAttemptMs = millis() - 5000;
    lastMqttAttemptMs = millis() - 5000;

    mqttClient.setServer(mqttHost, mqttPort);
    mqttClient.setCallback(onMqttMessage);

    server.on("/", HTTP_GET, handleRoot);

    server.on("/value", HTTP_GET, handleValue);

    server.on("/chat", HTTP_GET, handleChatGet);
    server.on("/chat", HTTP_POST, handleChatPost);

    server.onNotFound(notFound);

    handleWifiConnection();
    printStatus();
}

void loop() {
    handleUartInput();
    handleWifiConnection();
    handleMqttConnection();
    if (serverStarted) {
        server.handleClient();
    }
    printStatus();
}
