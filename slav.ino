#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define BUZZER_PIN D7
#define DISTANCE_THRESHOLD 20

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* serverIP = "192.168.1.X";

void setup() {
    Serial.begin(115200);
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://" + String(serverIP) + "/distance");
        int httpCode = http.GET();

        if (httpCode == 200) {
            float distance = http.getString().toFloat();
            digitalWrite(BUZZER_PIN, distance < DISTANCE_THRESHOLD ? HIGH : LOW);
        }

        http.end();
    }

    delay(500);
}
