#include <SoftwareSerial.h>
#include <GSON.h>

#define RX_PIN 8
#define TX_PIN 9

#define LIGHT_PIN 10
#define TRIGGER_CHANNEL 5

SoftwareSerial serial(RX_PIN, TX_PIN);
gson::Parser p;

void setup() {
  serial.begin(57600);
}

void parseRC() {
  auto rc = p["data"]["channels"];
  digitalWrite(LIGHT_PIN, rc[TRIGGER_CHANNEL].toInt16() > INT16_MAX / 2);
}

void parseJSON(String str) {
  p.parse(str);
  auto t = p["type"];
  if (t.compare("rc")) {
    parseRC();
  }
}

void loop() {
  if (serial.available()) {
    parseJSON(serial.readString());
  }
  delay(100);
}
