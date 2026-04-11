#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -------- SERVOS --------
Servo servo1;
Servo servo2;

// -------- LCD --------
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// -------- ULTRASONIC --------
#define TRIG_PIN 5
#define ECHO_PIN 18

long duration;
float distance;

void setup() {
  Serial.begin(115200);

  // -------- SERVO SETUP --------
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);

  servo1.attach(12, 500, 2400); // Azimuth
  servo2.attach(13, 500, 2400); // Elevation

  // -------- ULTRASONIC SETUP --------
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // -------- LCD SETUP --------
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("System Starting");
  delay(2000);
  lcd.clear();
}

void loop() {

  // -------- SERVO SWEEP --------
  for (int angle = 0; angle <= 180; angle += 30) {

    servo1.write(angle);
    servo2.write(180 - angle);

    // -------- ULTRASONIC READ --------
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH, 30000);
    distance = duration * 0.034 / 2;

    // -------- SERIAL OUTPUT --------
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" | Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // -------- LCD DISPLAY --------
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ang:");
    lcd.print(angle);

    lcd.setCursor(0, 1);
    lcd.print("Dist:");
    lcd.print(distance);

    delay(1000);
  }
}
