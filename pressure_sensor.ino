/*
  FSR502 / FA502 Pressure-Sensor Tester – analog pin A1
  ----------------------------------------------------
  Wiring (voltage-divider):
        5 V ──> FSR ─┬──> A1 (signal to ADC)
                     └──> 10 kΩ to GND

  Hardware notes
    • Sensor range ≈ 100 g … 10 kg, off-resistance > 2 MΩ.     (datasheet)
    • 10 kΩ gives usable resolution through that span.           (Adafruit app-note)
    • Readings are *trend* values; absolute calibration
      requires your own reference weights.

  Open Serial Monitor at 9600 baud.
*/

constexpr uint8_t  FSR_PIN     = A1;     // analog input
constexpr uint32_t R_PULLDOWN  = 10000;  // 10 kΩ in ohms
constexpr uint16_t VCC_MV      = 5000;   // supply in millivolts

void setup() {
  Serial.begin(9600);
  while (!Serial);        // wait for native USB boards
  Serial.println(F("\nFSR502 / FA502 test (A1)"));
  Serial.println(F("Format: adc  mV  ohms  µmho  N  kg"));
  Serial.println(F("----------------------------------------------------------------"));
}

void loop() {
  int adc = analogRead(FSR_PIN);           // 0-1023
  uint32_t vout = static_cast<uint32_t>(adc) * VCC_MV / 1023;   // mV

  if (adc < 2) {                           // essentially open (>~2 MΩ)
    Serial.println(F("0\t0\topen\t0\t0\t0"));
    delay(250);
    return;
  }

  // FSR resistance from voltage divider formula
  uint32_t r_fsr = (static_cast<uint32_t>(VCC_MV) - vout) * R_PULLDOWN / vout;

  // Conductance in micro-mhos (µɱ) for Adafruit’s piecewise model
  uint32_t conduct = 1000000UL / r_fsr;

  // Empirical force estimate (Adafruit guide)
  float force_N;
  if (conduct <= 1000)
    force_N = conduct / 80.0;          // low-force curve
  else
    force_N = (conduct - 1000) / 30.0; // high-force curve

  float weight_kg = force_N * 0.10197; // 1 N ≈ 0.10197 kg

  Serial.print(adc);         Serial.print('\t');
  Serial.print(vout);        Serial.print('\t');
  Serial.print(r_fsr);       Serial.print('\t');
  Serial.print(conduct);     Serial.print('\t');
  Serial.print(force_N, 1);  Serial.print('\t');
  Serial.println(weight_kg, 3);

  delay(250);
}
