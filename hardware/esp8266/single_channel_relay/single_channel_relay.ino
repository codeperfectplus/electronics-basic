#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi credentials
const char* ssid     = "wifi";
const char* password = "passwod";

// Web server
ESP8266WebServer server(80);

// Relay connected to D1 (GPIO5)
const int relayPin = D1;
String relayState = "OFF";

// API Handlers
void handleRoot() {
  server.send(200, "text/plain", "ESP8266 REST API Relay Controller");
}

void handleStatus() {
  String json = "{";
  json += "\"chip_id\":\"" + String(ESP.getChipId()) + "\",";
  json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
  json += "\"relay\":\"" + relayState + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

void handleRelayOn() {
  digitalWrite(relayPin, LOW);   // Active LOW relay
  relayState = "ON";
  server.send(200, "application/json", "{\"relay\":\"on\"}");
}

void handleRelayOff() {
  digitalWrite(relayPin, HIGH);
  relayState = "OFF";
  server.send(200, "application/json", "{\"relay\":\"off\"}");
}

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Start OFF

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
  server.on("/relay/on", handleRelayOn);
  server.on("/relay/off", handleRelayOff);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
