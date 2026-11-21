
#include <ESP8266WiFi.h>
#include <KiwisIoT.h>
#include <PulseSensorPlayground.h>

// WiFi Credentials
const char* ssid = "wifi name ";
const char* pass = "password";

// KiwisIoT Topic ID
const char* topic = "dash_1791367866871";

// Pulse Sensor
#define PULSE_PIN A0

int Threshold = 550;

PulseSensorPlayground pulseSensor;

// KiwisIoT Initialization
KiwisIoT kiwisiot(ssid, pass, topic);

// BPM Variables
int BPM = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 2000;

void setup() {
  Serial.begin(115200);

  // Initialize Pulse Sensor
  pulseSensor.analogInput(PULSE_PIN);
  pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
    Serial.println("Pulse Sensor Initialized");
  } else {
    Serial.println("Pulse Sensor Initialization Failed");
  }

  // Initialize KiwisIoT
  kiwisiot.begin();

  Serial.println("Heart Rate Monitoring Started");
}

void loop() {

  // Run KiwisIoT
  kiwisiot.run();

  // Detect heartbeat
  if (pulseSensor.sawNewSample()) {

    if (pulseSensor.sawStartOfBeat()) {

      BPM = pulseSensor.getBeatsPerMinute();

      if (BPM > 40 && BPM < 200) {
        Serial.print("Heart Rate: ");
        Serial.print(BPM);
        Serial.println(" BPM");
      }
    }
  }

  // Send BPM to KiwisIoT every 2 seconds
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;

    if (BPM > 40 && BPM < 200) {

      kiwisiot.send("0", String(BPM));

      Serial.print("Sent to KiwisIoT: ");
      Serial.print(BPM);
      Serial.println(" BPM");

    } else {
      Serial.println("Waiting for valid heartbeat...");
    }
  }
}
