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

  // Refresh icon
  display.drawCircle(120, 5, 3, SSD1306_WHITE); // Outer circle
  display.drawLine(120, 2, 122, 4, SSD1306_WHITE); // Arrow part 1
  display.drawLine(122, 4, 120, 6, SSD1306_WHITE); // Arrow part 2

  // Temperature line
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
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

void bootingAnimation() {
  // Booting animation with improved readability and icons
  display.clearDisplay();
  display.setTextSize(1.2);
  display.setTextColor(SSD1306_WHITE);

  // Display 'Developed by' with icon
  display.drawCircle(10, 10, 5, SSD1306_WHITE); // Icon
  display.setCursor(20, 5);
  display.print("Developed by");
  display.setCursor(20, 15);
  display.print("codeperfectplus");
  display.display();
  delay(1500); // Show for 1.5 seconds

  // Display 'Powered by' with icon
  display.clearDisplay();
  display.drawRect(5, 5, 10, 10, SSD1306_WHITE); // Icon
  display.setCursor(20, 5);
  display.print("Powered by");
  display.setCursor(20, 15);
  display.print("Arduino");
  display.display();
  delay(1500); // Show for 1.5 seconds

  // Progress bar animation with text
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print("Loading Sensors...");
  display.display();

   for (int i = 0; i <= 100; i += 10) {
    display.fillRect(10, 40, i, 10, SSD1306_WHITE);
    display.display();
    delay(300); // 300ms * 10 = 3 seconds
  }
}

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  // bootingAnimation();
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
  delay(2000);  // Update every 2 seconds
}