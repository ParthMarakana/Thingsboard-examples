#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "TestECT";
const char* password = "07012023";

const char* serverName = "http://192.168.0.111:8080/api/v1/Cred100/telemetry";

unsigned long lastTime = 0;

unsigned long timerDelay = 100;
int i = 1;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.status());
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      // Your Domain name with URL path or IP address with path
      http.begin(serverName);
      DynamicJsonDocument doc(2048);
      doc["id"] = i;
      doc["DN"] = 0;
      doc["DID"] = "943CC625AE24";
      doc["DS"] = 1;
      doc["SSR"] = 0;
      doc["RPM"] = 666;
      doc["PSI"] = 60;
      doc["CT"] = "20220922154042";
      doc["PV"] = "1.0";
      doc["HV"] = "1.2";
      doc["FV"] = "1.10";

      char buff[256];
      serializeJson(doc, buff );
      int x = http.POST(buff);
      i++;
      Serial.println(x);
      Serial.print("HTTP Response code: ");
      Serial.println(buff);

      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
