/*
  Ultrasonic Sensor HC-SR04 and Arduino Tutorial

  by Dejan Nedelkovski,
  www.HowToMechatronics.com

*/

// Accelerometer
#include <Wire.h>
#include <basicMPU6050.h>

basicMPU6050<> mpu;
char accelState = 0;
int fallDetected = 0;
int fallTimer = 0;
int fallOutput = 0;

int16_t accelX;
int16_t accelY;
int16_t accelZ;
int accelMagnitude;

const int ACCEL_THRESHOLD = 10000; // 10% error margin for freefall
const int FALL_TIMER_SEC = 5;

// Ultrasonic
// defines pins numbers
const int trigPin1 = 7;  // left
const int echoPin1 = 8;  // left
const int trigPin2 = 9;  // right
const int echoPin2 = 10; // right
// number of samples to average
const int num_samples = 10;
// defines variables
long duration;
long duration2;
long durations_sum;
long durations_sum2;
int distance;
int distance2;
int intensity = 0;
int intensity2 = 0;
char dict[64];

//Buttons
// push button pins
const int streetMode = 4;
const int readSignButton = 5;
const int navigationMode = 6;
const int fallCancelButton = 3;

// push button states
int streetModeState = 0;
int readSignState = 0;
int navigationModeState = 0;
int fallCancelState = 0;

// mode state
bool streetModeOn = false;
bool readSignOn = false;
bool navigationModeOn = false;


void setup() {
  //Accelerometer
  mpu.setup(); // Set registers - Always required
  mpu.setBias(); // Initial calibration
  //Ultrasonic
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  //Buttons
  pinMode(streetMode, INPUT);
  pinMode(navigationMode, INPUT);
  pinMode(readSignButton, INPUT);
  pinMode(fallCancelButton, INPUT);
  Serial.begin(9600); // Starts the serial communication
  Serial1.begin(115200); // Starts the serial communication
}
void loop() {

//Buttons
  // read the state of the pushbutton value:
  streetModeState = digitalRead(streetMode);
  readSignState = digitalRead(readSignButton);
  navigationModeState = digitalRead(navigationMode);
  fallCancelState = digitalRead(fallCancelButton);

  if (streetModeState == HIGH){
    streetModeOn = !streetModeOn;
    if (streetModeOn) {
      readSignOn = false;
    }
    delay(1000);
  }

  else if (navigationModeState == HIGH) {
    navigationModeOn = !navigationModeOn;
    delay(1000);
  }

  if (readSignState == HIGH) {
    readSignOn = !readSignOn;
    if (readSignOn) {
      streetModeOn = false;
    }
    delay(1000);
  }

  if (fallCancelState == HIGH) {
    fallDetected = 0;
    delay(1000);
  }
//---------------------------

  //Average num_samples
  durations_sum = 0;
  durations_sum2 = 0;
  for (int i = 0; i < num_samples; i++) {
    // Clears the trigPin 1
    digitalWrite(trigPin1, LOW);
    delayMicroseconds(2);
    // Sets the trigPin1 on HIGH state for 10 micro seconds
    digitalWrite(trigPin1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin1, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds - Time out at 2 second
    duration = pulseIn(echoPin1, HIGH, 2000);

//    // Clears the trigPin 2
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);
    // Sets the trigPin 2 on HIGH state for 10 micro seconds
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);
    duration2 = pulseIn(echoPin2, HIGH, 2000);
    
    if (duration == 0) {
      duration = 2000;
    }

    if (duration2 == 0) {
      duration2 = 2000;
    }
    // Add all durations to calculate average
    durations_sum = durations_sum + duration;
    durations_sum2 = durations_sum2 + duration2;
    // Delay to have equal sample times
    //    delay(2000-duration);

  }

  // Calculating the distance with average duration
  distance = (durations_sum / num_samples) * 0.034 / 2;

  // Calculating the distance2 with average duration
  distance2 = (durations_sum2 / num_samples) * 0.034 / 2;
 
  
//  Serial.print("Distance: ");
//  Serial.println(distance);
//
//  //Distance 2
//  Serial.print("Distance2: ");
//  Serial.println(distance2);


  if (range_intensity1(distance)) {
    intensity = 1;
  } else if (range_intensity2(distance)) {
    intensity = 2;
  } else if (range_intensity3(distance)) {
    intensity = 3;
  } else {
    //Assume intensity 0
    intensity = 0;
    //Serial.println("No vibration on LS");
  }

  if (range_intensity1(distance2)) {
    intensity2 = 1;
  } else if (range_intensity2(distance2)) {
    intensity2 = 2;
  } else if (range_intensity3(distance2)) {
    intensity2 = 3;
  } else {
    //Assume intensity 0
    intensity2 = 0;
    //Serial.println("No vibration on RS");
  }

  // Accelerometer
  accelX = mpu.rawAx();
  accelY = mpu.rawAy();
  accelZ = mpu.rawAz();
  accelMagnitude = sqrt(sq(accelX) + sq(accelY) + sq(accelZ));

  if (accelMagnitude == accelMagnitude % 2) {
    mpu.setup();
  }
  else {
    if (fallDetected == 0) {
      fallOutput = 0;
      if (accelState == 0) {
        if (accelMagnitude <= ACCEL_THRESHOLD) {
          accelState = 1;
        }
      }
      else {
        if (accelMagnitude >= ACCEL_THRESHOLD) {
          fallDetected = 1;
          Serial.println("Fall Detected");
          fallTimer = millis();
          accelState = 0;
        }
      }
    }
    else {
      if (millis() - fallTimer >= 1000 * FALL_TIMER_SEC) {
        fallOutput = 1;
        fallDetected = 0;
      }
    }
  }
  

  /* Writes intensity to Python Dictionairy Directly for Demo
  lh -> left vibration (0-3)
  rh -> right vibration (0-3)
  nm -> navigation mode (0-1) top button toggle
  sm -> street mode (0-1) third button toggle / mutex with rs
  rs -> read sign (0-1) second button toggle  / mutex with sm
  fd -> fall detected (0-1) sends text message
  fa -> fall acknowledged (0-1) cancelled with bottom button
  */
  
  sprintf(dict, "{\"lh\": %d, \"rh\": %d, \"nm\": %d, \"sm\": %d, \"rs\": %d, \"fd\": %d, \"fa\": %d}", intensity, intensity2, navigationModeOn, streetModeOn, readSignOn, fallOutput, fallDetected);
  Serial.println(dict);
  Serial1.println(dict);

  delay(100);

}

int range_intensity1(long distance) {
  // distance between 30cm & 40cm
  return (distance >= 30 && distance < 40);
}

int range_intensity2(long distance) {
  // distance between 20cm & 30cm
  return (distance >= 20 && distance < 30 );

}

int range_intensity3(long distance) {
  // distance less than 20cm
  return (distance < 20);
}
