#include <ctype.h>

#define DELAY 1000

#define TX 16
#define RX 6

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX, TX);
  delay(2000);
  Serial.println("05_serial2");
}

void loop() {
  char c;

/*
  if (Serial.available()) {
    c = Serial.read();
    // Serial1.print(c);
    Serial.print(c); //local echo
  }
*/
  if (Serial2.available()) {
    c = Serial2.read();
    Serial.print(c);
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= DELAY) {
    previousMillis = currentMillis;
    Serial2.write('A');
  }
}
