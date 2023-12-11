#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "Credentials.h"

#define LED_PIN 26
#define VOLTAGE_SENSE 39
#define CURRENT_SENSE 36

// Measurement variables
uint16_t totalTime = 0;
uint32_t currentTime = 0;
uint32_t lastEventTime = 0;
uint32_t lastMeasurementTime = 0;
uint8_t elapsedTimeSinceLastMeasurement = 0;
uint16_t elapsedTimeSinceLastEvent = 0;
uint8_t measurementInterval = 1;
uint16_t eventInterval = 1000;

AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Create variables to store measurements
float voltageRMS;
float currentRMS;

float previousVoltage;

double realEnergy;
double apparentEnergy;

float voltage;
float current;
float power;
float powerFactor;

double voltageIntegral;
double currentIntegral;

// put function declarations here:
void notFound(AsyncWebServerRequest *request);
String ledOn();
String ledOff();

void setup() {
  // put your setup code here, to run once:
  pinMode(CURRENT_SENSE,INPUT);
  pinMode(VOLTAGE_SENSE,INPUT);
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

  // Route for root bulbOffBaby.png 
  server.on("/bulbOffBaby.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/bulbOffBaby.png", "image/png"); });
  
  // Route for root bulbOffBaby.png
  server.on("/bulbOnBaby.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/bulbOnBaby.png", "image/png"); });          

  // Route for turning on the LED
  server.on("/on", HTTP_ANY, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", ledOn().c_str());});
  
  // Route for turning off the LED
  server.on("/off", HTTP_ANY, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", ledOff().c_str());});

  // Event handling for client connection
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }

    // Send a welcome message to the client
    client->send("hi!", NULL, millis(), 10000);
  });  

  // Set up notFound handler and add event handler
  server.onNotFound(notFound);
  server.addHandler(&events);

  // Start the server
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  currentTime = millis();
  elapsedTimeSinceLastMeasurement = (currentTime - lastMeasurementTime);
  elapsedTimeSinceLastEvent = (currentTime - lastEventTime);

  if(elapsedTimeSinceLastEvent > eventInterval){
    // Send Events to the Web Server with the Sensor Readings:
    events.send(String(voltageRMS).c_str(),"voltage",millis());
    events.send(String(currentRMS).c_str(),"current",millis());
    events.send(String(power).c_str(),"power",millis());
    events.send(String(powerFactor).c_str(),"power_factor",millis());
    lastEventTime = millis();
    Serial.print("Power : ");
    Serial.print(power);
    Serial.print(" ");
    Serial.print("Power factor : ");
    Serial.print(powerFactor);
    Serial.print(" ");
    Serial.print("Current : ");
    Serial.print(currentRMS);
    Serial.print(" ");
    Serial.print("Voltage : ");
    Serial.print(voltageRMS);
    Serial.println(" ");

  }
  // Perform measurements
  if (elapsedTimeSinceLastMeasurement > measurementInterval) {
    previousVoltage = voltage;
    voltage = map(analogRead(VOLTAGE_SENSE),0,4095,-510,+510);
    current = map(analogRead(CURRENT_SENSE),0,4095,-2.5,+2.5);
    

    // Calculate RMS values and power factors
    if(previousVoltage < 0 && voltage >=0){
      voltageRMS = 0.5 * voltageRMS + 0.5 * sqrtf(voltageIntegral / totalTime);
      currentRMS = 0.9 * currentRMS + 0.1 * sqrtf(currentIntegral / totalTime);
      powerFactor = realEnergy/apparentEnergy;
      power = realEnergy / totalTime;

      // Reset variables for the next measurement cycle
      totalTime = 0;
      voltageIntegral = 0;
      currentIntegral = 0;
      realEnergy = 0;
      apparentEnergy = 0;
    }

    // Update cumulative values for measurements
    totalTime += elapsedTimeSinceLastMeasurement;
    voltageIntegral += voltage*voltage * elapsedTimeSinceLastMeasurement;
    currentIntegral += current*current * elapsedTimeSinceLastMeasurement;
    realEnergy += current*voltage * elapsedTimeSinceLastMeasurement;
    apparentEnergy += abs(current*voltage) * elapsedTimeSinceLastMeasurement;

    // Update the time of the last measurement
    lastMeasurementTime = millis();
  }

}

// put function definitions here:
void notFound(AsyncWebServerRequest *request){
  request->send(404, "text/plain", "Not found");
}

// Function to turn on the LED and return a status message
String ledOn(){
  digitalWrite(LED_PIN,HIGH);
  return "led on";
}

// Function to turn off the LED and return a status message
String ledOff(){
  digitalWrite(LED_PIN,LOW);
  return "led off";
}