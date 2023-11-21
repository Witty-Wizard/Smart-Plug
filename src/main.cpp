#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "Credentials.h"

#define LED_PIN 26

unsigned long total_time = 0;
unsigned long current_time = 0;
unsigned long last_time_1 = 0;
unsigned long last_time = 0;
unsigned long dt = 0;
unsigned long Delay = 1;

AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Create variables to store measurements
float voltage_rms;
float current_rms;

float voltage_previous;

float real_energy;
float apparant_energy;

float voltage;
float current;
float power;
float power_factor;

float voltage_i;
float current_i;

// put function declarations here:
void notFound(AsyncWebServerRequest *request);
String ledOn();
String ledOff();

void setup() {
  // put your setup code here, to run once:
  pinMode(36,INPUT);
  pinMode(39,INPUT);
  pinMode(LED_PIN,OUTPUT);
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

  server.on("/bulbOffBaby.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/bulbOffBaby.png", "image/png"); });

  server.on("/bulbOnBaby.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/bulbOnBaby.png", "image/png"); });          

  server.on("/on", HTTP_ANY, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", ledOn().c_str());});
  
  server.on("/off", HTTP_ANY, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", ledOff().c_str());});

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
  current_time = millis();
  dt = (current_time - last_time);
  if(current_time - last_time_1 > 7000){
    // Send Events to the Web Server with the Sensor Readings:
    events.send(String(voltage_rms).c_str(),"voltage",millis());
    events.send(String(current_rms).c_str(),"current",millis());
    events.send(String(power).c_str(),"power",millis());
    events.send(String(power_factor).c_str(),"power_factor",millis());
    last_time_1 = millis();
  }
  if (dt > Delay) {
    voltage_previous = voltage;
    voltage = map(analogRead(36),0,4095,-510,+510);
    current = map(analogRead(39),0,4095,-5,+5);
    Serial.print(voltage);
    Serial.print("  ");
    Serial.println(current);

    if(voltage_previous < 0 && voltage >=0){
      voltage_rms = sqrt(voltage_i / total_time);
      current_rms = sqrt(current_i / total_time);
      power_factor = real_energy/apparant_energy;
      power = real_energy / total_time;
      

      total_time = 0;
      voltage_i = 0;
      current_i = 0;
      real_energy = 0;
      apparant_energy = 0;
    }

    total_time += dt;
    voltage_i += voltage*voltage * dt;
    current_i += current*current * dt;
    real_energy += current*voltage * dt;
    apparant_energy += abs(current*voltage) * dt;

    last_time = millis();
  }

}

// put function definitions here:
void notFound(AsyncWebServerRequest *request){
  request->send(404, "text/plain", "Not found");
}

String ledOn(){
  digitalWrite(LED_PIN,HIGH);
  return "led on";
}

String ledOff(){
  digitalWrite(LED_PIN,LOW);
  return "led off";
}