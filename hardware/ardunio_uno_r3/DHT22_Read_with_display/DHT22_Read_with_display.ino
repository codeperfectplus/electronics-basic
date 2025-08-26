#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


void showValues(float t, float h) {
  display.clearDisplay();
  
  // Temperature line
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("T: ");
  display.print(t, 1);
  display.print("C");

  // Humidity line
  display.setTextSize(2);
  display.setCursor(0, 28);
  display.print("H: ");
  display.print(h, 1);
  display.print("%");



  // Signature
  display.setTextSize(1);
  display.setCursor(0, 54);
  display.print("by codeperfectplus");

  display.display();
}

void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.display();
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 25);
    display.println(" Sensor");
    display.println("  Error!");
    display.display();
    delay(5000);
    return;
  }

  showValues(t, h);
  delay(5000); // Update every 5 seconds
}