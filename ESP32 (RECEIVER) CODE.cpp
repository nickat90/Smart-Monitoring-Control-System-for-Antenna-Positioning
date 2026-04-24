#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -------- LCD --------
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// -------- SERVOS --------
Servo servo1;
Servo servo2;

// -------- ULTRASONIC --------
#define TRIG 5
#define ECHO 18

// -------- DATA STRUCT --------
typedef struct struct_message {
  int id;   // dummy (from transmitter)
} struct_message;

struct_message incomingData;

// -------- VARIABLES --------
int bestAngle = 0;
int bestRSSI = -100;

// -------- RECEIVE CALLBACK (UPDATED FOR ESP32 v3.x) --------
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingDataRaw, int len) {
  memcpy(&incomingData, incomingDataRaw, sizeof(incomingData));

  Serial.print("Packet received from: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", info->src_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  // WiFi mode
  WiFi.mode(WIFI_STA);

  // ESP-NOW init
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  // -------- SERVO SETUP (SAFE PINS) --------
  servo1.attach(25);
  servo2.attach(26);

  // -------- ULTRASONIC --------
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // -------- LCD --------
  lcd.init();
  lcd.backlight();
  lcd.print("Scanning...");
  delay(1500);
  lcd.clear();
}

void loop() {

  bestRSSI = -100;

  for (int angle = 0; angle <= 180; angle++) {

    // Move servos
    servo1.write(angle);
    servo2.write(180 - angle);

    delay(100); // stabilization

    // -------- GET RSSI --------
    int rssi = WiFi.RSSI();

    // -------- ULTRASONIC --------
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    long duration = pulseIn(ECHO, HIGH, 30000);
    float distance = duration * 0.034 / 2;

    // -------- TRACK BEST ANGLE --------
    if (rssi > bestRSSI) {
      bestRSSI = rssi;
      bestAngle = angle;
    }

    // -------- SERIAL OUTPUT --------
    Serial.print(angle);
    Serial.print(":");
    Serial.print(rssi);
    Serial.print(":");
    Serial.println(distance);

    // -------- LCD DISPLAY --------
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("A:");
    lcd.print(angle);
    lcd.print(" R:");
    lcd.print(rssi);

    lcd.setCursor(0, 1);
    lcd.print("D:");
    lcd.print(distance);
  }

  // -------- MOVE TO BEST ANGLE --------
  servo1.write(bestAngle);
  servo2.write(180 - bestAngle);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BEST ANGLE:");
  lcd.setCursor(0, 1);
  lcd.print(bestAngle);

  delay(10000);
}
