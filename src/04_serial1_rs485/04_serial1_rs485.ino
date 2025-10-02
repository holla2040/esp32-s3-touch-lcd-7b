#include <ctype.h>

#define DELAY 1000
#define LED 6

#define TXD1 16
#define RXD1 15

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  // Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);
  Serial1.begin(115200);
  pinMode(LED, OUTPUT);
  delay(2000);
  Serial.println("04_serial1_rs485");
  // Serial1.println("Serial1 TX test");
}

void loop() {
  char c;

/*
  if (Serial.available()) {
    c = Serial.read();
    // Serial1.print(c);
    Serial.print(c); //local echo
  }
  if (Serial1.available()) {
    c = Serial1.read();
    c = toupper(c);
    Serial.print(c);
  }
*/

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= DELAY) {
    previousMillis = currentMillis;
    digitalWrite(LED, !digitalRead(LED));
    Serial1.write(0xAB);
  }
}

