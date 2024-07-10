#include <WiFi.h>
#include "ThingsBoard.h"

#define CURRENT_FIRMWARE_TITLE    "ESP_OTA"
#define CURRENT_FIRMWARE_VERSION  "0.0.2"

#define WIFI_SSID           "TestECT"
#define WIFI_PASSWORD       "07012023"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token

#define TOKEN               "Cred15"
#define THINGSBOARD_SERVER  "192.168.0.111"

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
//ThingsBoard tb(espClient);
ThingsBoardSized<512> tb(espClient); //increased size to allow read shared attributes

// the Wifi radio's status
int status = WL_IDLE_STATUS;

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}

void UpdatedCallback(const bool& success) {

  Serial.println("UpdatedCallback");

  if (success) {
    Serial.println("Done, Reboot now");
    esp_restart();
  } else {
    Serial.println("No new firmware");
  }
}

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  Serial.println();

  Serial.println(CURRENT_FIRMWARE_TITLE);
  Serial.println(CURRENT_FIRMWARE_VERSION);

  InitWiFi();

  delay(1000);
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }

    Serial.println("Firwmare Update...");
    tb.Start_Firmware_Update(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION, UpdatedCallback);
  }

  tb.loop();
  delay(10);

}
