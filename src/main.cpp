#include <Arduino.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

StaticJsonDocument<100> dataJSON;
HTTPClient http;
String sensor;

const int moinstureSensor = 34;
const int relePin = 23;

const char* ssid = "";
const char* password = "";

int sensorId;
int humidity;

void setup() {
  WiFi.begin(ssid, password);
  http.addHeader("Content-Type", "application/json");
  pinMode(relePin, OUTPUT);
}

void loop() {
if (WiFi.isConnected()){
    if (!sensorId) {
      http.begin("https://backend-aquamole.azurewebsites.net/hygrometers/id");
      if (http.POST("") == 200) {
        deserializeJson(dataJSON, http.getString());
        sensorId = dataJSON["id"];
        dataJSON.clear();
      }
    }
    
    http.begin("https://backend-aquamole.azurewebsites.net/hygrometers");
    humidity = map(analogRead(moinstureSensor), 1000, 3000, 100, 0);
    if (humidity < 60) {
      digitalWrite(relePin, HIGH);
      delay(1500);
      digitalWrite(relePin, LOW);
    }
    dataJSON["id"] = sensorId;
    dataJSON["humidity"] = humidity;
    serializeJson(dataJSON, sensor);
    dataJSON.clear();
    http.POST(sensor);
    sensor.clear();
    delay(900000); 
  }
}
