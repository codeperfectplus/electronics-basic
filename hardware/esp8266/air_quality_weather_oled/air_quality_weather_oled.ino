#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define MQ135_PIN A0
#define DHTPIN D5
#define DHTTYPE DHT22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// WiFi Credentials
const char* ssid = "SSID";
const char* password = "passwword";

// Web Server on port 80
ESP8266WebServer server(80);

// DHT object
DHT dht(DHTPIN, DHTTYPE);

// OLED object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Global sensor variables
int airValue;
float airVoltage;
String airQuality;
float temperature;
float humidity;

unsigned long lastDisplaySwitch = 0;
bool showAir = true;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Start OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(10, 20);
  display.println("MQ135+DHT");
  display.display();
  delay(2000);

  // WiFi connect
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected, IP: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("WiFi Connected!");
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(WiFi.localIP());
  display.display();
  delay(2000);

  // REST API endpoints
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

  // Switch display every 2 sec
  if (millis() - lastDisplaySwitch > 2000) {
    showAir = !showAir;
    lastDisplaySwitch = millis();
  }

  // Update OLED
  display.clearDisplay();

  if (showAir) {
    display.setTextSize(1.5);
    display.setCursor(0, 0);
    display.println("Air Quality (MQ135)");

    display.setTextSize(1.5);
    display.setCursor(0, 14);
    display.print("Raw: ");
    display.println(airValue);
    display.setCursor(0, 26);
    display.print("Voltage: ");
    display.print(airVoltage, 2);
    display.println(" V");
    display.setCursor(0, 38);
    display.print("Status: ");
    display.println(airQuality);
  } else {
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("DHT22 Sensor");

    display.setTextSize(1.5);
    display.setCursor(0, 17);
    display.print("Temp: ");
    display.print(temperature, 1);
    display.println(" C");
    display.setCursor(0, 30);
    display.print("Humidity: ");
    display.print(humidity, 1);
    display.println(" %");
  }

  display.display();
}
