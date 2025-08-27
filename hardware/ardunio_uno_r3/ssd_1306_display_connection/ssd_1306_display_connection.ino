#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Fake fitness data
int steps = 1024;
int heartRate = 75;
int battery = 85; // percent

// === Battery Icon ===
void drawBattery(int x, int y, int level) {
  display.drawRect(x, y, 20, 10, SSD1306_WHITE);      // battery box
  display.fillRect(x+20, y+3, 2, 4, SSD1306_WHITE);   // tip
  int width = map(level, 0, 100, 0, 18);
  display.fillRect(x+1, y+1, width, 8, SSD1306_WHITE);
}

// === Heart Icon (tiny) ===
void drawHeart(int x, int y) {
  display.fillCircle(x, y, 3, SSD1306_WHITE);
  display.fillCircle(x+4, y, 3, SSD1306_WHITE);
  display.fillTriangle(x-3, y, x+7, y, x+2, y+6, SSD1306_WHITE);
}

// === Steps Icon (shoe-like) ===
void drawSteps(int x, int y) {
  display.drawRoundRect(x, y, 12, 6, 2, SSD1306_WHITE);
  display.fillRect(x+2, y+2, 6, 2, SSD1306_WHITE);
}

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }
  display.clearDisplay();
}

void loop() {
  display.clearDisplay();

  // === Top Bar (time + battery) ===
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("12:45"); // fake clock
  drawBattery(100, 0, battery);

  // === Steps Section ===
  drawSteps(0, 20);
  display.setCursor(20, 20);
  display.setTextSize(1);
  display.print("Steps:");
  display.setTextSize(2);
  display.setCursor(20, 32);
  display.print(steps);

  // === Heart Rate Section ===
  drawHeart(0, 55);
  display.setCursor(20, 50);
  display.setTextSize(1);
  display.print("HR: ");
  display.setTextSize(2);
  display.print(heartRate);
  display.setTextSize(1);
  display.print(" bpm");

  // Update screen
  display.display();

  // Fake updates
  steps += random(1, 5);
  heartRate = 70 + random(0, 10);
  battery = max(0, battery - (random(0, 100) < 5 ? 1 : 0)); // drain slowly

  delay(1000);
}
