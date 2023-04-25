#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

StaticJsonDocument<100> dataJSON;
HTTPClient http;
String sensor;

const int moinstureSensor = 36;

const char* ssid = "";
const char* passowrd = "";
const String serverUrl = "";

int sensorId;
int sensorMoinsture;

void setup() {
  WiFi.begin(ssid, passowrd);
  http.addHeader("Content-Type", "application/json");
}

void loop() {
  if (WiFi.isConnected() && !sensorId) {
    http.begin(serverUrl);
    if (http.GET() == 200) {
      deserializeJson(dataJSON, http.getString());
      sensorId = dataJSON["id"];
      dataJSON.clear();
    }
  }
  
  http.begin(serverUrl);
  sensorMoinsture = map(analogRead(moinstureSensor), 1000, 3000, 100, 0);
  dataJSON["id"] = sensorId;
  dataJSON["moinsture"] = sensorMoinsture;
  serializeJson(dataJSON, sensor);
  dataJSON.clear();
  http.POST(sensor);
  sensor.clear();
  delay(900000);
}
