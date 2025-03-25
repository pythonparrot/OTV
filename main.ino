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

// Function to get distance (in centimeters) from a specified ultrasonic sensor.
// sensorID: 1 for Sensor 1, 2 for Sensor 2.
float getUltrasonicDistance(int sensorID) {
  int triggerPin, echoPin;
  if (sensorID == 1) {
    triggerPin = US1_TRIGGER_PIN;
    echoPin = US1_ECHO_PIN;
  } else if (sensorID == 2) {
    triggerPin = US2_TRIGGER_PIN;
    echoPin = US2_ECHO_PIN;
  } else {
    // Return a negative value for an invalid sensor ID.
    return -1.0;
  }
  
  // Make sure the trigger pin is LOW before starting
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  
  // Send a 10 microsecond HIGH pulse to trigger the sensor
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  // Read the duration of the incoming pulse on the echo pin (timeout 30ms)
  long duration = pulseIn(echoPin, HIGH, 30000);
  
  // Calculate the distance in centimeters:
  // Sound speed ~ 0.034 cm/microsecond; divide by 2 for round-trip.
  float distance = (duration * 0.034) / 2.0;
  
  return distance;
}

void setup() {
  Serial.begin(9600);  // Initialize Serial Monitor

  // Initialize ultrasonic sensor pins
  pinMode(US1_TRIGGER_PIN, OUTPUT);
  pinMode(US1_ECHO_PIN, INPUT);
  pinMode(US2_TRIGGER_PIN, OUTPUT);
  pinMode(US2_ECHO_PIN, INPUT);

  // Initialize propulsion motor pins
  pinMode(MOTOR1_INA, OUTPUT);
  pinMode(MOTOR1_INB, OUTPUT);
  pinMode(MOTOR1_ENA, OUTPUT);
  pinMode(MOTOR2_INA, OUTPUT);
  pinMode(MOTOR2_INB, OUTPUT);
  pinMode(MOTOR2_ENB, OUTPUT);

  // Initialize servo motor pin
  pinMode(SERVO_PIN, OUTPUT);

  // Note: WiFi module and analog sensor pins don't require additional pinMode calls here.
}

void loop() {
  // Get distances from both ultrasonic sensors
  float distance1 = getUltrasonicDistance(1);
  float distance2 = getUltrasonicDistance(2);

  // Print the measurements to the Serial Monitor
  Serial.print("Ultrasonic Sensor 1 Distance: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Ultrasonic Sensor 2 Distance: ");
  Serial.print(distance2);
  Serial.println(" cm");

  // Example known distance to compare with (modify this value as needed)
  float expectedDistance = 50.0; // in centimeters

  // Compare sensor 1 reading with the expected distance
  if (abs(distance1 - expectedDistance) < 5.0) {
    Serial.println("Sensor 1 reading is within the expected range.");
  } else {
    Serial.println("Sensor 1 reading is NOT within the expected range.");
  }

  // Compare sensor 2 reading with the expected distance
  if (abs(distance2 - expectedDistance) < 5.0) {
    Serial.println("Sensor 2 reading is within the expected range.");
  } else {
    Serial.println("Sensor 2 reading is NOT within the expected range.");
  }

  Serial.println(); // Blank line for readability

  delay(1000); // Wait 1 second before taking the next reading
}
