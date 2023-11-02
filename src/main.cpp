#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "Credentials.h"

unsigned long last_time = 0;  
unsigned long Delay = 100;

AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Create variables to store measurements
float voltage;
float current;
float power;
float power_factor;

// put function declarations here:
void notFound(AsyncWebServerRequest *request);

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

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }

    client->send("hi!", NULL, millis(), 10000);
  });  
                 
  server.onNotFound(notFound);
  server.addHandler(&events);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((millis() - last_time) > Delay) {

    // Send Events to the Web Server with the Sensor Readings
    events.send("ping",NULL,millis());
    events.send(String(voltage).c_str(),"voltage",millis());
    events.send(String(current).c_str(),"current",millis());
    events.send(String(power).c_str(),"power",millis());
    events.send(String(power_factor).c_str(),"power_factor",millis());
    
    last_time = millis();
  }
}

// put function definitions here:
void notFound(AsyncWebServerRequest *request){
  request->send(404, "text/plain", "Not found");
}