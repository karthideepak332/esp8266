#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define TRIG_PIN 12
#define ECHO_PIN 14
#define LED_PIN 2

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

ESP8266WebServer server(80);

#define SOUND_VELOCITY 0.034
#define DISTANCE_THRESHOLD 20

long duration;
float distanceCm;

float getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);
    return duration * SOUND_VELOCITY / 2;
}

void handleDistance() {
    distanceCm = getDistance();
    server.send(200, "text/plain", String(distanceCm));
}

void setup() {
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    server.on("/distance", handleDistance);
    server.begin();
}

void loop() {
    distanceCm = getDistance();
    digitalWrite(LED_PIN, distanceCm < DISTANCE_THRESHOLD ? HIGH : LOW);
    server.handleClient();
    delay(500);
}
