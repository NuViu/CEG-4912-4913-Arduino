#include <SoftwareSerial.h>

SoftwareSerial xbee(3, 2);

int ledR1 = 5;
int ledR2 = 6;
int ledG1 = 7;
int ledG2 = 8;
int ledB1 = 10;
int ledB2 = 12;

int received = 0;

void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
}

void loop() {
 
   if (xbee.available()) {
    received = xbee.read();
    Serial.println(received);
 
    if (received >= 75) {
      digitalWrite(ledB2, HIGH);
    }
    else {
      digitalWrite(ledB2, LOW);
    }
    
    if (received >= 60) {
      digitalWrite(ledB1, HIGH);
    }
    else {
      digitalWrite(ledB1, LOW);
    }
    
    if (received >= 45) {
      digitalWrite(ledG2, HIGH);
    }
    else {
      digitalWrite(ledG2, LOW);
    }

    if (received >= 30) {
      digitalWrite(ledG1, HIGH);
    }
    else {
      digitalWrite(ledG1, LOW);
    }
    
    if (received >= 15) {
      digitalWrite(ledR2, HIGH);
    }
    else {
      digitalWrite(ledR2, LOW);
    }
    
    if (received >= 0) {
      digitalWrite(ledR1, HIGH);
    }
    else {
      digitalWrite(ledR1, LOW);
    }

  }
  
}
