// === Ultrasonic Sensor 1 ===
const int US1_TRIGGER_PIN = 3;   // D3
const int US1_ECHO_PIN    = 12;  // D12

// === Ultrasonic Sensor 2 ===
const int US2_TRIGGER_PIN = A2;  // A2 used as digital
const int US2_ECHO_PIN    = A3;  // A3 used as digital

// === Propulsion Motor 1 ===
const int MOTOR1_INA = 2;  // D2
const int MOTOR1_INB = 4;  // D4
const int MOTOR1_ENA = 5;  // D5 (PWM)

// === Propulsion Motor 2 ===
const int MOTOR2_INA = 7;  // D7
const int MOTOR2_INB = 8;  // D8
const int MOTOR2_ENB = 6;  // D6 (PWM)

// === Servo Motor ===
const int SERVO_PIN = 9;  // D9 (PWM)

// === WiFi Module (SoftwareSerial) ===
const int WIFI_RX = 10;  // D10 (Arduino receives from module TX)
const int WIFI_TX = 11;  // D11 (Arduino transmits to module RX)

// === Analog Sensors ===
const int SOUND_SENSOR_PIN    = A0;
const int PRESSURE_SENSOR_PIN = A1;

void setup() {
  // Example: Initialize pins (as needed)
  pinMode(US1_TRIGGER_PIN, OUTPUT);
  pinMode(US1_ECHO_PIN, INPUT);

  pinMode(US2_TRIGGER_PIN, OUTPUT);
  pinMode(US2_ECHO_PIN, INPUT);

  pinMode(MOTOR1_INA, OUTPUT);
  pinMode(MOTOR1_INB, OUTPUT);
  pinMode(MOTOR1_ENA, OUTPUT);

  pinMode(MOTOR2_INA, OUTPUT);
  pinMode(MOTOR2_INB, OUTPUT);
  pinMode(MOTOR2_ENB, OUTPUT);

  pinMode(SERVO_PIN, OUTPUT);

  // WiFi handled via SoftwareSerial (initialize in actual logic)
  
  // No need to set pinMode for analog inputs (A0, A1)
}

void loop() {
  // Your main logic goes here
}
