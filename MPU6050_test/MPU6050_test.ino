/*
    MPU6050 Triple Axis Gyroscope & Accelerometer. Pitch & Roll Accelerometer Example.
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-zyroskop-i-akcelerometr-mpu6050.html
    GIT: https://github.com/jarzebski/Arduino-MPU6050
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <basicMPU6050.h>

basicMPU6050<> mpu;
char state = 0;

void setup() 
{
  Serial.begin(9600);

  // Set registers - Always required
  mpu.setup();

  // Initial calibration of gyro
  mpu.setBias();
}

void loop()
{
  // Read normalized values 
  int16_t accelX = mpu.rawAx();
  int16_t accelY = mpu.rawAy();
  int16_t accelZ = mpu.rawAz();


  int magnitude = sqrt(sq(accelX) + sq(accelY) + sq(accelZ));

  if (magnitude == magnitude % 2) {
    mpu.setup();
  }
  else {
    if (state == 0) {
      if (magnitude <= 1638) { // 10% error margin
        state = 1;
      }
    }
    else {
      if (magnitude >= 1638) {
      Serial.println("Fall detected");
      state = 0;
      }
    }
  }
  
  Serial.print(accelX);
  Serial.print(" , ");
  Serial.print(accelY);
  Serial.print(" , ");
  Serial.print(accelZ);
  Serial.print(" , ");
  Serial.print(magnitude);
  Serial.println();

  // Calculate Pitch & Roll

  // Output
  
  delay(100);
}
