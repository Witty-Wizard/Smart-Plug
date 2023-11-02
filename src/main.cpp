#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "Credentials.h"

AsyncWebServer server(80);

float voltage;
float current;
float power;
float power_factor;

// put function declarations here:
void notFound(AsyncWebServerRequest *request);
String sendVoltage();
String sendCurrent();
String sendPower();
String sendPowerFactor();

void setup() {
  // put your setup code here, to run once:
  pinMode(36,OUTPUT);
  Serial.begin(115200);
  LittleFS.begin();

  // Begin Wifi access point
  if (!WiFi.softAP(SSID, PASSWORD)) {
    log_e("Soft AP creation failed.");
    while(1);
  }

  Serial.println("Access Point started");
  Serial.println(WiFi.softAPIP());

  // Route for root index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html"); });
  
  // Route for root index.css
  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.css", "text/css"); });

  // Route for root index.js
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.js", "text/javascript"); });

  server.on("/voltage", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", sendVoltage().c_str()); });

  server.on("/current", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", sendCurrent().c_str()); });

  server.on("/power", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", sendPower().c_str()); });

  server.on("/power_factor", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", sendPowerFactor().c_str()); });     
                 
  server.onNotFound(notFound);
  
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  voltage = analogRead(36);
}

// put function definitions here:
void notFound(AsyncWebServerRequest *request){
  request->send(404, "text/plain", "Not found");
}

String sendVoltage(){
  return String(voltage);
}

String sendCurrent(){
  current++;
  return String(current);
}

String sendPower(){
  power = voltage * current;
  return String(power);
}

String sendPowerFactor(){
  power_factor++;
  return String(power_factor);
}