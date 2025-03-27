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

// ---------------------------------------
// UltrasonicSensor Class (single sensor)
// ---------------------------------------
class UltrasonicSensor {
  private:
    int triggerPin;
    int echoPin;
  
  public:
    // Constructor: set the trigger and echo pins and initialize them.
    UltrasonicSensor(int trig, int echo) {
      triggerPin = trig;
      echoPin = echo;
      pinMode(triggerPin, OUTPUT);
      pinMode(echoPin, INPUT);
    }
    
    // Method to get the distance in centimeters.
    float getDistance() {
      // Ensure trigger pin is LOW before starting
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
};

// ---------------------------------------
// Struct to hold two sensor distances
// ---------------------------------------
struct SensorDistances {
  float distance1;
  float distance2;
};

// ---------------------------------------
// UltrasonicSensors Class (aggregates two sensors)
// ---------------------------------------
class UltrasonicSensors {
  private:
    UltrasonicSensor sensor1;
    UltrasonicSensor sensor2;
    
  public:
    // Constructor: initialize both ultrasonic sensors.
    UltrasonicSensors(int trig1, int echo1, int trig2, int echo2)
      : sensor1(trig1, echo1), sensor2(trig2, echo2) {
      // Sensors are initialized via their constructors.
    }
    
    // triggerSensors method: calls getDistance() on both sensors with a small delay between them.
    SensorDistances triggerSensors() {
      SensorDistances distances;
      distances.distance1 = sensor1.getDistance();
      delay(50); // Small delay between sensor triggers (50ms)
      distances.distance2 = sensor2.getDistance();
      return distances;
    }
};

// ---------------------------------------
// Create an UltrasonicSensors object
// ---------------------------------------
UltrasonicSensors ultrasonicSensors(US1_TRIGGER_PIN, US1_ECHO_PIN,
                                      US2_TRIGGER_PIN, US2_ECHO_PIN);

void setup() {
  Serial.begin(9600);  // Initialize Serial Monitor

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
  // Use the triggerSensors() method to get distances from both ultrasonic sensors.
  SensorDistances distances = ultrasonicSensors.triggerSensors();

  // Print the measurements to the Serial Monitor
  Serial.print("Ultrasonic Sensor 1 Distance: ");
  Serial.print(distances.distance1);
  Serial.println(" cm");

  Serial.print("Ultrasonic Sensor 2 Distance: ");
  Serial.print(distances.distance2);
  Serial.println(" cm");

  Serial.println(); // Blank line for readability

  delay(1000); // Wait 1 second before taking the next reading
}
