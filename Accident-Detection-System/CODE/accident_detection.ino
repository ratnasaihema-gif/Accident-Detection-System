#include <Wire.h>

const int MPU = 0x68; // MPU6050 I2C address
int buzzer = 9;

int16_t ax, ay, az;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(buzzer, OUTPUT);

  // Wake up MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  Serial.println("System Ready...");
}

void loop() {
  // Read acceleration data
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();

  // Convert values
  float x = ax / 16384.0;
  float y = ay / 16384.0;
  float z = az / 16384.0;

  float total = sqrt(x*x + y*y + z*z);

  Serial.print("Acceleration: ");
  Serial.println(total);

  // Accident detection
  if (total > 2.5) {
    digitalWrite(buzzer, HIGH);
    Serial.println("🚨 Accident Detected!");
    delay(3000);
    digitalWrite(buzzer, LOW);
  }

  delay(500);
}
