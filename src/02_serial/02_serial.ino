#define DELAY 1000

void setup() {
  Serial.begin(115200);
  Serial.println("02_serial setup");
}

void loop() {
  Serial.println(millis());
  delay(DELAY);
}
