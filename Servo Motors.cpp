#include <ESP32Servo.h>
#include <analogWrite.h>
#include <tone.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

#include <ESP32Servo.h>

Servo myServo;  // Create servo object
int servoPin = 13; // GPIO pin connected to orange wire

void setup() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  myServo.setPeriodHertz(50);    // Standard 50hz servo
  myServo.attach(servoPin, 500, 2400); // Attach servo with min/max pulse width
}

void loop() {
  myServo.write(0);   // Move to 0 degrees
  delay(1000);
  myServo.write(90);  // Move to 90 degrees
  delay(1000);
  myServo.write(180); // Move to 180 degrees
  delay(1000);
}
