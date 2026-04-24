#include <WiFi.h>
#include <esp_now.h>

// RECEIVER MAC ADDRESS
uint8_t receiverMAC[] = {0x1C, 0xC3, 0xAB, 0xF9, 0x47, 0xB0};

// Simple packet
typedef struct struct_message {
  int id;
} struct_message;

struct_message data;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);

  data.id = 1; // dummy value
}

void loop() {
  esp_now_send(receiverMAC, (uint8_t *) &data, sizeof(data));

  Serial.println("Beacon sent");

  delay(50); // continuous beaconing
}