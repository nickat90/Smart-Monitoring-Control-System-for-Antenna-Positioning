# 📡 Smart Monitoring and Control for Antenna Positioning System

## 👥 Project Members

* Nicole Mbodze – ENE221-0134/2021
* Redempta Mwaniki – ENE221-0109/2021

---

## 📌 Overview

This project develops a smart system for **automatic antenna alignment and monitoring** using an ESP32-based architecture. The system rotates an antenna to optimize signal strength (RSSI) while providing real-time feedback locally and remotely.

---

## ⚙️ System Architecture

### Transmitter (ESP32 - WROOM-32)

* Controls servo motors (azimuth & elevation)
* Reads ultrasonic sensor and RSSI
* Displays data on LCD
* Sends data via ESP-NOW

### Receiver (ESP32-CAM)

* Receives data
* Hosts a web interface for real-time monitoring

---

## 🔧 Components

* ESP32 (WROOM-32)
* ESP32-CAM
* 2 × Servo Motors (MG90S)
* Ultrasonic Sensor (HC-SR04)
* LCD 16x2 (I2C)
* Push Button
* Breadboard & Jumper Wires
* Resistors (220Ω, 1kΩ, 10kΩ)
* 5V Power Supply

---

## 🔄 Operation

* Button press initiates antenna sweep (0°–180°)
* At each angle:

  * RSSI and distance are measured
* Data format:

  ```
  angle:rssi:distance
  ```
* Data is displayed on LCD and sent to ESP32-CAM for web monitoring

---

## 📡 Communication

* Uses **ESP-NOW** for wireless communication between ESP32 modules

---

## 💻 Requirements

* Arduino IDE
* ESP32 board package
* Libraries: WiFi.h, esp_now.h, Servo.h, LiquidCrystal_I2C.h

---

## 🚀 Conclusion

The system improves antenna alignment by combining **automation, sensing, and wireless communication**, providing a reliable and scalable monitoring solution.

---
