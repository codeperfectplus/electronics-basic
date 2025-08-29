#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ===== WiFi Settings =====
const char* ssid = "wifi";
const char* password = "passwod";

// ===== OLED Settings =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); b 

// ===== DHT Settings =====
#define DHTPIN D4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===== Web Server =====
ESP8266WebServer server(80);

// ===== Global Variables =====
float t = 0.0;
float h = 0.0;

void setup() {
  Serial.begin(115200);

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());

  // Web API endpoints
  server.on("/", []() {
    server.send(200, "text/plain", "ESP8266 Weather API - /weather");
  });

  server.on("/weather", []() {
    String json = "{";
    json += "\"temperature\":" + String(t, 1) + ",";
    json += "\"humidity\":" + String(h, 1);
    json += "}";
    server.send(200, "application/json", json);
  });

  server.begin();

  // OLED init
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // DHT init
  dht.begin();
  delay(2000);
}

void showValues(float t, float h) {
  display.clearDisplay();

  // Refresh icon
  display.drawCircle(120, 5, 3, SSD1306_WHITE);
  display.drawLine(120, 2, 122, 4, SSD1306_WHITE);
  display.drawLine(122, 4, 120, 6, SSD1306_WHITE);

  // Temperature line
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print("T: ");
  display.print(t, 1);
  display.print("C");

  // Humidity line
  display.setTextSize(2);
  display.setCursor(0, 35);
  display.print("H: ");
  display.print(h, 1);
  display.print("%");

  display.display();
}

void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    showValues(t, h);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  server.handleClient();   // Handle API requests
  delay(2000);
}
