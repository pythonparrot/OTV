/*
  FS5103B Servo Test – pin 9
  ---------------------------------
  Wiring
    • Servo orange (signal)  -> Arduino D9
    • Servo red    (Vcc)     -> 5 V supply*
    • Servo brown  (GND)     -> Arduino GND

  *If the servo stalls it can draw up to ~1 A; use a separate 5–6 V source and
   join the grounds.

  Open the Serial Monitor at 9600 baud for the interactive test.
*/

#include <Servo.h>

constexpr uint8_t SERVO_PIN = 9;
constexpr int BASIC_DELAY   = 15;      // ms between steps
constexpr int US_MIN        = 600;     // datasheet min pulse for ~0° servo_datasheet.pdf](file-service://file-TZenhACwi1vbCf8c3hR1Fs)
constexpr int US_MAX        = 2400;    // datasheet max pulse for ~180° servo_datasheet.pdf](file-service://file-TZenhACwi1vbCf8c3hR1Fs)

Servo motor;
bool firstLoopDone = false;

void setup() {
  motor.attach(SERVO_PIN, US_MIN, US_MAX); // lock pulse range
  Serial.begin(9600);
  Serial.println(F("\nFS5103B Servo tester"));
  Serial.println(F("Commands: A<angle 0-180>   or   P<pulse 500-2500>"));
  Serial.println(F("----- Starting automatic tests -----"));
}

void loop() {
  if (!firstLoopDone) {
    basicSweep();
    extendedSweep();
    firstLoopDone = true;
    Serial.println(F("----- Automatic tests complete – enter commands -----"));
  }
  interactiveControl();
}

// Test 1 – basic 0-180 sweep using standard write()
void basicSweep() {
  for (int pos = 0; pos <= 180; pos++) {
    motor.write(pos);
    delay(BASIC_DELAY);
  }
  for (int pos = 180; pos >= 0; pos--) {
    motor.write(pos);
    delay(BASIC_DELAY);
  }
}

// Test 2 – full-range sweep with writeMicroseconds()
void extendedSweep() {
  for (int us = US_MIN; us <= US_MAX; us += 10) {
    motor.writeMicroseconds(us);
    delay(5);
  }
  for (int us = US_MAX; us >= US_MIN; us -= 10) {
    motor.writeMicroseconds(us);
    delay(5);
  }
}

// Test 3 – interactive: read Serial for “A###” or “P####”
void interactiveControl() {
  if (!Serial.available()) return;

  char type = Serial.read();
  long value = Serial.parseInt(); // reads the following number
  if (type == 'A' && value >= 0 && value <= 180) {
    motor.write(value);
    Serial.print(F("Angle set to "));
    Serial.println(value);
  } else if (type == 'P' && value >= 500 && value <= 2500) {
    motor.writeMicroseconds(value);
    Serial.print(F("Pulse set to "));
    Serial.print(value);
    Serial.println(F(" µs"));
  } else {
    Serial.println(F("Invalid command."));
  }
}
