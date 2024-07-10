#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include <HTTPClient.h>
#include <ESP32httpUpdate.h>

#define USE_SERIAL Serial

TaskHandle_t Task1;

String gsCurr_Title = "ESP_OTA";
String gsCurr_Version = "0.0.3";

String giServerName = "http://192.168.0.111:8080";
void setup() {

  USE_SERIAL.begin(115200);
  pinMode(2, OUTPUT);
  // USE_SERIAL.setDebugOutput(true);
  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();


  gvconnectwifi();
  OTA_Update();
}

void gvconnectwifi() {
  WiFi.begin("TestECT", "07012023");
  Serial.println("Connected to wifi:");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
}

void OTA_Update() {


  String tsPayload;
  if ((WiFi.status() == WL_CONNECTED)) {

    HTTPClient http;
    String tsServerPath_getinfo = giServerName + "/api/v1/Cred1/attributes?sharedKeys=fw_title,fw_version";
    http.begin(tsServerPath_getinfo.c_str());

    int tiHttpResponseCode = http.GET();

    if (tiHttpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(tiHttpResponseCode);
      tsPayload = http.getString();
    }
    else {
      Serial.print("Error code: ");
      Serial.println(tiHttpResponseCode);
    }
    http.end();

    StaticJsonDocument<2048> doc;
    deserializeJson(doc, tsPayload);

    String tsTitle = doc["shared"]["fw_title"];
    String tsVersion = doc["shared"]["fw_version"];

    Serial.println(tsTitle);
    Serial.println(tsVersion);
    String tsServerPath_Firmware = giServerName + "/api/v1/Cred1/firmware?title=" + tsTitle + "&version=" + tsVersion;

    if (gsCurr_Version != tsVersion) {
      t_httpUpdate_return ret = ESPhttpUpdate.update(tsServerPath_Firmware.c_str());

      switch (ret) {
        case HTTP_UPDATE_FAILED:
          USE_SERIAL.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
          Serial.println("");
          gvsend_info(tsTitle, tsVersion, "FAILED");
          OTA_Update();
          break;

        case HTTP_UPDATE_OK:
          USE_SERIAL.println("HTTP_UPDATE_OK");
          gvsend_info(tsTitle, tsVersion, "UPDATED");
          break;
      }
    } else {
      gvsend_info(gsCurr_Title, gsCurr_Version, "UPDATED");
    }
  }
}

void gvsend_info(String tsTitle, String tsVersion, String State) {

  HTTPClient http;
  String tsServerPath_sendinfo = giServerName + "/api/v1/Cred1/telemetry";
  http.begin(tsServerPath_sendinfo.c_str());
  DynamicJsonDocument doc_SendInfo(2048);

  if (State == "UPDATED") {
    doc_SendInfo["current_fw_title"] = tsTitle;
    doc_SendInfo["current_fw_version"] = tsVersion;
  } else {
    doc_SendInfo["fw_error"] = "Firmwate Update Failed";
  }

  doc_SendInfo["fw_state"] = State;

  char buff[256];
  serializeJson(doc_SendInfo, buff );

  int tiHttpResponseCode_send = http.POST(buff);


  Serial.print("HTTP_send Response code: ");
  Serial.println(tiHttpResponseCode_send);
  Serial.print("Sending buffer: ");
  Serial.println(buff);

  http.end();
}

void loop() {
  digitalWrite(2, HIGH);
  USE_SERIAL.println("HIGH");
  delay(300);

  digitalWrite(2, LOW);
  USE_SERIAL.println("LOW");
  delay(300);

}
