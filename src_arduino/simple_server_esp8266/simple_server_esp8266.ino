//
// A simple server implementation showing how to:
//  * serve a website embedded in flash (PROGMEM)
//  * expose a JSON endpoint for the current numeric value
//  * update the numeric value from newline-terminated JSON over UART
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

float displayValue = 0.0f;
String uartBuffer;
unsigned long lastStatusMs = 0;
unsigned long lastWifiAttemptMs = 0;
bool serverStarted = false;

String valueJson() {
    String json = "{\"value\":";
    json += String(displayValue, 2);
    json += "}";
    return json;
}

bool updateValueFromJson(const String& json) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);

    if (error || doc["code"] != "data" || !doc["value"].is<float>()) {
        return false;
    }

    displayValue = doc["value"].as<float>();
    return true;
}

void printAck() {
    Serial.println("{\"code\":\"ack\"}");
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
                if (updateValueFromJson(uartBuffer)) {
                    printAck();
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

void notFound() {
    server.send(404, "text/plain", "Not found");
}

void setup() {

    Serial.begin(115200);
    uartBuffer.reserve(96);

    Serial.printf("\n\n");

    lastStatusMs = millis() - 1000;
    lastWifiAttemptMs = millis() - 5000;

    server.on("/", HTTP_GET, handleRoot);

    server.on("/value", HTTP_GET, handleValue);

    server.onNotFound(notFound);

    handleWifiConnection();
    printStatus();
}

void loop() {
    handleUartInput();
    handleWifiConnection();
    if (serverStarted) {
        server.handleClient();
    }
    printStatus();
}
