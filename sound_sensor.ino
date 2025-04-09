/*
   This sketch demonstrates how to read from a KY-037 sound sensor using both
   its analog and digital outputs. All ultrasonic code has been removed.

   - The KY-037 sensor typically has:
       A0 = Analog output
       D0 = Digital output (threshold-based)
       G  = Ground
       +  = 5V
*/

//
// === Pin Definitions ===
//

// === Propulsion Motor 1 ===
const int MOTOR1_INA = 2;   // D2
const int MOTOR1_INB = 4;   // D4
const int MOTOR1_ENA = 5;   // D5 (PWM)

// === Propulsion Motor 2 ===
const int MOTOR2_INA = 7;   // D7
const int MOTOR2_INB = 8;   // D8
const int MOTOR2_ENB = 6;   // D6 (PWM)

// === Servo Motor ===
const int SERVO_PIN = 9;    // D9 (PWM)

// === WiFi Module (SoftwareSerial) ===
const int WIFI_RX = 10;     // D10 (Arduino receives from module TX)
const int WIFI_TX = 11;     // D11 (Arduino transmits to module RX)

// === Analog Sensors ===
const int SOUND_SENSOR_ANALOG_PIN = A0;  // KY-037 Analog output
const int PRESSURE_SENSOR_PIN     = A1;  // Placeholder for another analog sensor

// === Sound Sensor Digital Pin ===
const int SOUND_SENSOR_DIGITAL_PIN = 13; // Choose an available digital pin for KY-037 D0

void setup() {
  Serial.begin(9600);

  // Initialize propulsion motor pins
  pinMode(MOTOR1_INA, OUTPUT);
  pinMode(MOTOR1_INB, OUTPUT);
  pinMode(MOTOR1_ENA, OUTPUT);
  pinMode(MOTOR2_INA, OUTPUT);
  pinMode(MOTOR2_INB, OUTPUT);
  pinMode(MOTOR2_ENB, OUTPUT);

  // Initialize servo motor pin
  pinMode(SERVO_PIN, OUTPUT);

  // Sound sensor digital pin
  pinMode(SOUND_SENSOR_DIGITAL_PIN, INPUT);

  // No pinMode required for analog inputs (A0, A1) by default
}

void loop() {
  // Read from KY-037 analog pin (range: 0 to 1023)
  int soundAnalogValue = analogRead(SOUND_SENSOR_ANALOG_PIN);

  // Read from KY-037 digital pin (HIGH or LOW depending on threshold)
  int soundDigitalValue = digitalRead(SOUND_SENSOR_DIGITAL_PIN);

  if (soundAnalogValue > 40){
    while (soundDigitalValue == 1){
      Serial.print("Plastic material.");
    }
  }

  // Print the measurements to Serial Monitor
  Serial.print("Sound Sensor (Analog) = ");
  Serial.println(soundAnalogValue);

  Serial.print("Sound Sensor (Digital) = ");
  Serial.println(soundDigitalValue);

  Serial.println(); // Blank line for readability

  // Simple delay between readings
  delay(50);
}
