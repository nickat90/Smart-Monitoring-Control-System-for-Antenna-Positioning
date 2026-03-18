#include <esp_now.h>
#include <WiFi.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -------- COMPONENTS --------
Servo servoAz;
Servo servoEl;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// -------- PINS --------
#define SERVO_AZ 18
#define SERVO_EL 19
#define TRIG 5
#define ECHO 17
#define BUTTON 4

// -------- RECEIVER MAC --------
uint8_t receiverMAC[] = {0x24, 0x6F, 0x28, 0xXX, 0xXX, 0xXX};

// -------- DATA STRUCT --------
typedef struct {
  char data[32];
} Message;

Message msg;

// -------- FUNCTIONS --------
long getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);

  servoAz.attach(SERVO_AZ);
  servoEl.attach(SERVO_EL);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  WiFi.mode(WIFI_STA);
  esp_now_init();
}

void loop() {

  if (digitalRead(BUTTON) == LOW) {

    for (int angle = 0; angle <= 180; angle += 10) {

      servoAz.write(angle);
      servoEl.write(180 - angle);

      long distance = getDistance();
      int rssi = WiFi.RSSI();

      // Format: angle:rssi:distance
      sprintf(msg.data, "%d:%d:%ld", angle, rssi, distance);

      // Send via ESP-NOW
      esp_now_send(receiverMAC, (uint8_t *)&msg, sizeof(msg));

      // LCD DISPLAY
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ang:");
      lcd.print(angle);

      lcd.setCursor(0, 1);
      lcd.print("R:");
      lcd.print(rssi);
      lcd.print(" D:");
      lcd.print(distance);

      delay(300);
    }
  }
}