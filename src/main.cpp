#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "Credentials.h"

AsyncWebServer server(80);

// put function declarations here:
void notFound(AsyncWebServerRequest *request);

void setup() {
  // put your setup code here, to run once:
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
  server.on("/static/css/main.073c9b0a.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/static/css/main.073c9b0a.css", "text/css"); });

  // Route for root index.js
  server.on("/static/js/main.65a72d31.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/static/js/main.65a72d31.js", "text/javascript"); });
            
  server.onNotFound(notFound);
  
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
void notFound(AsyncWebServerRequest *request){
  request->send(404, "text/plain", "Not found");
}