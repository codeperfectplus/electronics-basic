#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi credentials
const char* ssid     = "wifi";
const char* password = "passwod";

// Web server
ESP8266WebServer server(80);

// Relay connected to D1 (GPIO5)
const int relayPin = D1;
String relayState = "OFF";

void showMessage(String line1, String line2 = "", String line3 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(line1);
  if (line2 != "") display.println(line2);
  if (line3 != "") display.println(line3);
  display.display();
}

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
  showMessage("Relay State:", "ON");
  server.send(200, "application/json", "{\"relay\":\"on\"}");
}

void handleRelayOff() {
  digitalWrite(relayPin, HIGH);
  relayState = "OFF";
  showMessage("Relay State:", "OFF");
  server.send(200, "application/json", "{\"relay\":\"off\"}");
}

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Start OFF

  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  showMessage("Booting...", "Connecting WiFi");

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
  showMessage("WiFi Connected!", "IP:", ip);

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
