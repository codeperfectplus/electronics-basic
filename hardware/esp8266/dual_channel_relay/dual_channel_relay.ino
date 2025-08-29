#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi credentials
const char* ssid     = "wifi";
const char* password = "passwod";

// Web server
ESP8266WebServer server(80);

// Relay pins
const int relay1Pin = D1;  // GPIO5
const int relay2Pin = D2;  // GPIO4

String relay1State = "OFF";
String relay2State = "OFF";

// API Handlers
void handleRoot() {
  server.send(200, "text/plain", "ESP8266 REST API Dual Relay Controller");
}

void handleStatus() {
  String json = "{";
  json += "\"chip_id\":\"" + String(ESP.getChipId()) + "\",";
  json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
  json += "\"relay1\":\"" + relay1State + "\",";
  json += "\"relay2\":\"" + relay2State + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

// Relay 1 controls
void handleRelay1On() {
  digitalWrite(relay1Pin, LOW);   // Active LOW
  relay1State = "ON";
  server.send(200, "application/json", "{\"relay1\":\"on\"}");
}

void handleRelay1Off() {
  digitalWrite(relay1Pin, HIGH);
  relay1State = "OFF";
  server.send(200, "application/json", "{\"relay1\":\"off\"}");
}

// Relay 2 controls
void handleRelay2On() {
  digitalWrite(relay2Pin, LOW);
  relay2State = "ON";
  server.send(200, "application/json", "{\"relay2\":\"on\"}");
}

void handleRelay2Off() {
  digitalWrite(relay2Pin, HIGH);
  relay2State = "OFF";
  server.send(200, "application/json", "{\"relay2\":\"off\"}");
}

// Both relays
void handleAllOn() {
  digitalWrite(relay1Pin, LOW);
  digitalWrite(relay2Pin, LOW);
  relay1State = "ON";
  relay2State = "ON";
  server.send(200, "application/json", "{\"relay1\":\"on\", \"relay2\":\"on\"}");
}

void handleAllOff() {
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  relay1State = "OFF";
  relay2State = "OFF";
  server.send(200, "application/json", "{\"relay1\":\"off\", \"relay2\":\"off\"}");
}

void setup() {
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  digitalWrite(relay1Pin, HIGH);  // Start OFF
  digitalWrite(relay2Pin, HIGH);

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected!");

  String ip = WiFi.localIP().toString();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // API endpoints
  server.on("/", handleRoot);
  server.on("/status", handleStatus);

  // Relay 1
  server.on("/relay1/on", handleRelay1On);
  server.on("/relay1/off", handleRelay1Off);

  // Relay 2
  server.on("/relay2/on", handleRelay2On);
  server.on("/relay2/off", handleRelay2Off);

  // Both relays
  server.on("/relays/on", handleAllOn);
  server.on("/relays/off", handleAllOff);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
