#include <esp_now.h>
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

typedef struct {
  char data[32];
} Message;

Message incoming;

String latestData = "0:0:0";

// -------- RECEIVE FUNCTION --------
void onReceive(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incoming, incomingData, sizeof(incoming));
  latestData = String(incoming.data);

  Serial.println(latestData);
}

// -------- WEB PAGE --------
void handleRoot() {
  String page = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Radar System</title>
    <meta http-equiv="refresh" content="1">
  </head>
  <body>
    <h2>Radar Data</h2>
    <p>)rawliteral";

  page += latestData;

  page += R"rawliteral(</p>
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", page);
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin("YOUR_SSID", "YOUR_PASSWORD");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println(WiFi.localIP());

  esp_now_init();
  esp_now_register_recv_cb(onReceive);

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}