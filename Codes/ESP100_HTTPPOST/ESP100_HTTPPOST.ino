#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "TestECT";
const char* password = "07012023";

//String inputString = "";      // a String to hold incoming data
//bool stringComplete = false;  // whether the string is complete
//
//long int giDevice_num = 1;

TaskHandle_t Task1;
TaskHandle_t Task2;



unsigned long lastTime = 0;
int giTemp2 = 0;
unsigned long timerDelay = 500;
//int giTemp1 = 1;
//int giTemp2 = 1;

void setup() {
  Serial.begin(115200);
  //inputString.reserve(200);
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

  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);

  delay(500);

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
//  xTaskCreatePinnedToCore(
//    Task2code,   /* Task function. */
//    "Task2",     /* name of task. */
//    10000,       /* Stack size of task */
//    NULL,        /* parameter of the task */
//    1,           /* priority of the task */
//    &Task2,      /* Task handle to keep track of created task */
//    1);          /* pin task to core 1 */
}

void Task1code( void * pvParameters ) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
int giTemp1 = 0;
  for (;;) {
    vTaskDelay(1000 / portTICK_RATE_MS);
    DynamicJsonDocument doc(2048);
    doc["id"] = giTemp1;
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

    char buff[2048];
    serializeJson(doc, buff );
    String serverName1 = "http://192.168.0.111:8080/api/v1/oVm4qHmEzG1vgFn7MXrl/telemetry";
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      // Your Domain name with URL path or IP address with path
      http.begin(serverName1);
      int x1 = http.POST(buff);
      Serial.println(x1);
      //      Serial.print("HTTP Response code: ");
      //      Serial.println(buff);
      http.end();
    }
    giTemp1++;
  }
}

//void Task2code( void * pvParameters ) {
//  Serial.print("Task2 running on core ");
//  Serial.println(xPortGetCoreID());
//int giTemp2 = 0;
//  for (;;) {
//    vTaskDelay(1000 / portTICK_RATE_MS);
//    DynamicJsonDocument doc(2048);
//    doc["id"] = giTemp2;
//    doc["DN"] = 0;
//    doc["DID"] = "943CC625AE24";
//    doc["DS"] = 1;
//    doc["SSR"] = 0;
//    doc["RPM"] = 666;
//    doc["PSI"] = 60;
//    doc["CT"] = "20220922154042";
//    doc["PV"] = "1.0";
//    doc["HV"] = "1.2";
//    doc["FV"] = "1.10";
//
//    char buff[2048];
//    serializeJson(doc, buff );
//    String serverName2 = "http://192.168.0.111:8080/api/v1/91YKa3PakSIGQMRuJG4n/telemetry";
//    if (WiFi.status() == WL_CONNECTED) {
//      WiFiClient client;
//      HTTPClient http;
//      // Your Domain name with URL path or IP address with path
//      http.begin(serverName2);
//      int x1 = http.POST(buff);
//      Serial.println(x1);
//      //      Serial.print("HTTP Response code: ");
//      //      Serial.println(buff);
//      http.end();
//    }
//    giTemp2++;
//  }
//}

 void loop() {
    vTaskDelay(1000 / portTICK_RATE_MS);
    DynamicJsonDocument doc(2048);
    doc["id"] = giTemp2;
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

    char buff[2048];
    serializeJson(doc, buff );
    String serverName2 = "http://192.168.0.111:8080/api/v1/91YKa3PakSIGQMRuJG4n/telemetry";
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      // Your Domain name with URL path or IP address with path
      http.begin(serverName2);
      int x1 = http.POST(buff);
      Serial.println(x1);
      //      Serial.print("HTTP Response code: ");
      //      Serial.println(buff);
      http.end();
    }
    giTemp2++;
  //Send an HTTP POST request every 10 minutes
  //
  //  if (stringComplete) {
  //    Serial.println(inputString);
  //    // clear the string:
  //    giDevice_num = inputString.toInt();
  //    inputString = "";
  //    stringComplete = false;
  //  }
  //  for (int Temp = 1; Temp <= 10; Temp++) {
  //    String serverName1 = "http://192.168.0.111:8080/api/v1/" + Token1_10[Temp] + "/telemetry";
  //    String serverName2 = "http://192.168.0.111:8080/api/v1/" + Token11_20[Temp] + "/telemetry";
  //    String serverName3 = "http://192.168.0.111:8080/api/v1/" + Token21_30[Temp] + "/telemetry";
  //    String serverName4 = "http://192.168.0.111:8080/api/v1/" + Token31_40[Temp] + "/telemetry";
  //    String serverName5 = "http://192.168.0.111:8080/api/v1/" + Token41_50[Temp] + "/telemetry";
  //    String serverName6 = "http://192.168.0.111:8080/api/v1/" + Token51_60[Temp] + "/telemetry";
  //    String serverName7 = "http://192.168.0.111:8080/api/v1/" + Token61_70[Temp] + "/telemetry";
  //    String serverName8 = "http://192.168.0.111:8080/api/v1/" + Token71_80[Temp] + "/telemetry";
  //    String serverName9 = "http://192.168.0.111:8080/api/v1/" + Token81_90[Temp] + "/telemetry";
  //    String serverName10 = "http://192.168.0.111:8080/api/v1/" + Token91_100[Temp] + "/telemetry";
  //
  //
  //    DynamicJsonDocument doc(2048);
  //    doc["id"] = giTemp;
  //    doc["DN"] = 0;
  //    doc["DID"] = "943CC625AE24";
  //    doc["DS"] = 1;
  //    doc["SSR"] = 0;
  //    doc["RPM"] = 666;
  //    doc["PSI"] = 60;
  //    doc["CT"] = "20220922154042";
  //    doc["PV"] = "1.0";
  //    doc["HV"] = "1.2";
  //    doc["FV"] = "1.10";
  //
  //    char buff[2048];
  //    serializeJson(doc, buff );
  //
  //    //Check WiFi connection status
  //    if (WiFi.status() == WL_CONNECTED) {
  //      WiFiClient client;
  //      HTTPClient http;
  //      // Your Domain name with URL path or IP address with path
  //      http.begin(serverName1);
  //      int x1 = http.POST(buff);
  //      Serial.println(x1);
  //      //      Serial.print("HTTP Response code: ");
  //      //      Serial.println(buff);
  //      http.end();
  //      http.begin(serverName2);
  //      int x2 = http.POST(buff);
  //      Serial.println(x2);
  //      //      Serial.print("HTTP Response code: ");
  //      //      Serial.println(buff);
  //      http.end();
  //      http.begin(serverName3);
  //      int x3 = http.POST(buff);
  //      Serial.println(x3);
  //      //      Serial.print("HTTP Response code: ");
  //      //      Serial.println(buff);
  //      http.end();
  //      http.begin(serverName4);
  //      int x4 = http.POST(buff);
  //      Serial.println(x4);
  //      //      Serial.print("HTTP Response code: ");
  //      //      Serial.println(buff);
  //      http.end();
  //      http.begin(serverName5);
  //      int x5 = http.POST(buff);
  //      Serial.println(x5);
  //      //      Serial.print("HTTP Response code: ");
  //      //      Serial.println(buff);
  //      http.end();
  //      http.begin(serverName6);
  //      int x6 = http.POST(buff);
  //      Serial.println(x6);
  //      //      Serial.print("HTTP Response code: ");
  //      //      Serial.println(buff);
  //      http.end();
  //      http.begin(serverName7);
  //      int x7 = http.POST(buff);
  //      Serial.println(x7);
  //      //      Serial.print("HTTP Response code: ");
  //      //      Serial.println(buff);
  //      http.end();
  //      http.begin(serverName8);
  //      int x8 = http.POST(buff);
  //      Serial.println(x8);
  //      //      Serial.print("HTTP Response code: ");
  //      //      Serial.println(buff);
  //      http.end();
  //      http.begin(serverName9);
  //      int x9 = http.POST(buff);
  //      Serial.println(x9);
  //      //      Serial.print("HTTP Response code: ");
  //      //      Serial.println(buff);
  //      http.end();
  //      http.begin(serverName10);
  //      int x10 = http.POST(buff);
  //      Serial.println(x10);
  //      //      Serial.print("HTTP Response code: ");
  //      //      Serial.println(buff);
  //      http.end();
  //      if (Temp == 10) {
  //        giTemp++;
  //      }
  //    }
  //    else {
  //      Serial.println("WiFi Disconnected");
  //    }
  //    lastTime = millis();
  //  }
}

//void serialEvent() {
//  while (Serial.available()) {
//    // get the new byte:
//    char inChar = (char)Serial.read();
//    // add it to the inputString:
//    inputString += inChar;
//    // if the incoming character is a newline, set a flag so the main loop can
//    // do something about it:
//    if (inChar == '\n') {
//      stringComplete = true;
//    }
//  }
//}
