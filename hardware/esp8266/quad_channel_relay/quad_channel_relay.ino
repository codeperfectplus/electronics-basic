#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ===== WiFi credentials =====
const char* ssid     = "wifi";
const char* password = "passwod";

// ===== Web server =====
ESP8266WebServer server(80);

// ===== Relay pins =====
const int relay1Pin = D1;  // GPIO5
const int relay2Pin = D2;  // GPIO4
const int relay3Pin = D5;  // GPIO14
const int relay4Pin = D6;  // GPIO12

String relay1State = "OFF";
String relay2State = "OFF";
String relay3State = "OFF";
String relay4State = "OFF";

// ===== Helpers =====
void updateRelay(int pin, String &state, bool turnOn) {
  digitalWrite(pin, turnOn ? LOW : HIGH);  // Active LOW relay
  state = turnOn ? "ON" : "OFF";
}

// ===== API Handlers =====
void handleRoot() {
  server.send(200, "text/plain", "ESP8266 REST API 4-Channel Relay Controller");
}

void handleStatus() {
  String json = "{";
  json += "\"chip_id\":\"" + String(ESP.getChipId()) + "\",";
  json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
  json += "\"relay1\":\"" + relay1State + "\",";
  json += "\"relay2\":\"" + relay2State + "\",";
  json += "\"relay3\":\"" + relay3State + "\",";
  json += "\"relay4\":\"" + relay4State + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

// ===== Relay control endpoints =====
void handleRelay1On()  { updateRelay(relay1Pin, relay1State, true);  server.send(200, "application/json", "{\"relay1\":\"on\"}"); }
void handleRelay1Off() { updateRelay(relay1Pin, relay1State, false); server.send(200, "application/json", "{\"relay1\":\"off\"}"); }

void handleRelay2On()  { updateRelay(relay2Pin, relay2State, true);  server.send(200, "application/json", "{\"relay2\":\"on\"}"); }
void handleRelay2Off() { updateRelay(relay2Pin, relay2State, false); server.send(200, "application/json", "{\"relay2\":\"off\"}"); }

void handleRelay3On()  { updateRelay(relay3Pin, relay3State, true);  server.send(200, "application/json", "{\"relay3\":\"on\"}"); }
void handleRelay3Off() { updateRelay(relay3Pin, relay3State, false); server.send(200, "application/json", "{\"relay3\":\"off\"}"); }

void handleRelay4On()  { updateRelay(relay4Pin, relay4State, true);  server.send(200, "application/json", "{\"relay4\":\"on\"}"); }
void handleRelay4Off() { updateRelay(relay4Pin, relay4State, false); server.send(200, "application/json", "{\"relay4\":\"off\"}"); }

// ===== All relays together =====
void handleAllOn() {
  updateRelay(relay1Pin, relay1State, true);
  updateRelay(relay2Pin, relay2State, true);
  updateRelay(relay3Pin, relay3State, true);
  updateRelay(relay4Pin, relay4State, true);
  server.send(200, "application/json", "{\"relay1\":\"on\", \"relay2\":\"on\", \"relay3\":\"on\", \"relay4\":\"on\"}");
}

void handleAllOff() {
  updateRelay(relay1Pin, relay1State, false);
  updateRelay(relay2Pin, relay2State, false);
  updateRelay(relay3Pin, relay3State, false);
  updateRelay(relay4Pin, relay4State, false);
  server.send(200, "application/json", "{\"relay1\":\"off\", \"relay2\":\"off\", \"relay3\":\"off\", \"relay4\":\"off\"}");
}

// ===== Setup =====
void setup() {
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(relay3Pin, OUTPUT);
  pinMode(relay4Pin, OUTPUT);

  digitalWrite(relay1Pin, HIGH); // Start OFF
  digitalWrite(relay2Pin, HIGH);
  digitalWrite(relay3Pin, HIGH);
  digitalWrite(relay4Pin, HIGH);

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // API routes
  server.on("/", handleRoot);
  server.on("/status", handleStatus);

  server.on("/relay1/on", handleRelay1On);
  server.on("/relay1/off", handleRelay1Off);

  server.on("/relay2/on", handleRelay2On);
  server.on("/relay2/off", handleRelay2Off);

  server.on("/relay3/on", handleRelay3On);
  server.on("/relay3/off", handleRelay3Off);

  server.on("/relay4/on", handleRelay4On);
  server.on("/relay4/off", handleRelay4Off);

  server.on("/relays/on", handleAllOn);
  server.on("/relays/off", handleAllOff);

  server.begin();
  Serial.println("HTTP server started");
}

// ===== Loop =====
void loop() {
  server.handleClient();
}
