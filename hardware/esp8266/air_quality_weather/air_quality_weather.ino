#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define MQ135_PIN A0
#define DHTPIN D5
#define DHTTYPE DHT22

// WiFi Credentials
const char* ssid = "wifi";
const char* password = "passwod";

// Web Server on port 80
ESP8266WebServer server(80);

// DHT object
DHT dht(DHTPIN, DHTTYPE);

// Global sensor variables
int airValue;
float airVoltage;
String airQuality;
float temperature;
float humidity;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // WiFi connect
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected, IP: ");
  Serial.println(WiFi.localIP());

  // REST API endpoint
  server.on("/", []() {
    String json = "{";
    json += "\"air_raw\":" + String(airValue) + ",";
    json += "\"air_voltage\":" + String(airVoltage, 3) + ",";
    json += "\"air_quality\":\"" + airQuality + "\",";
    json += "\"temperature\":" + String(temperature, 1) + ",";
    json += "\"humidity\":" + String(humidity, 1);
    json += "}";
    server.send(200, "application/json", json);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // Read MQ135
  airValue = analogRead(MQ135_PIN);
  airVoltage = airValue * (1.0 / 1023.0);

  // Improved air quality labels
  if (airValue < 200) airQuality = "Fresh air";
  else if (airValue < 400) airQuality = "Moderate";
  else if (airValue < 600) airQuality = "Poor";
  else if (airValue < 800) airQuality = "Very Poor";
  else airQuality = "Hazardous";

  // Read DHT22
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
}
