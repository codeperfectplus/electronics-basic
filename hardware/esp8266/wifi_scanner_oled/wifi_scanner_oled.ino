#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_SDA      D2  // GPIO4
#define OLED_SCL      D1  // GPIO5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button settings
#define BUTTON_PIN D3 // GPIO0

// WiFi network struct
struct WiFiNetwork {
  String ssid;
  int32_t rssi;
  String encryption;
};

WiFiNetwork topNetworks[3];
int screen = 0; // 0: list, 1: details

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button: D3 to GND
  Wire.begin(OLED_SDA, OLED_SCL);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Ensure we're not connected

  delay(100);
  display.println("Scanning WiFi...");
  display.display();

  scanAndShow();
}

void loop() {
  static bool lastButton = HIGH;
  bool buttonState = digitalRead(BUTTON_PIN);

  // Button pressed: go to next screen
  if (lastButton == HIGH && buttonState == LOW) {
    screen = (screen + 1) % 2;
    showScreen();
    delay(300); // Debounce
  }
  lastButton = buttonState;
}

void scanAndShow() {
  int n = WiFi.scanNetworks();
  if (n == 0) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("No WiFi found");
    display.display();
    return;
  }

  // Save top 3 by RSSI
  for (int i = 0; i < 3; i++) {
    int bestIdx = -1;
    int bestRssi = -1000;
    for (int j = 0; j < n; j++) {
      bool alreadyChosen = false;
      for (int k = 0; k < i; k++) {
        if (topNetworks[k].ssid == WiFi.SSID(j)) {
          alreadyChosen = true;
          break;
        }
      }
      if (!alreadyChosen && WiFi.RSSI(j) > bestRssi) {
        bestRssi = WiFi.RSSI(j);
        bestIdx = j;
      }
    }
    if (bestIdx >= 0) {
      topNetworks[i].ssid = WiFi.SSID(bestIdx);
      topNetworks[i].rssi = WiFi.RSSI(bestIdx);
      topNetworks[i].encryption = (WiFi.encryptionType(bestIdx) == ENC_TYPE_NONE) ? "Open" : "Encrypted";
    } else {
      topNetworks[i].ssid = "";
      topNetworks[i].rssi = 0;
      topNetworks[i].encryption = "";
    }
  }
  showScreen();
}

void showScreen() {
  display.clearDisplay();
  display.setCursor(0,0);

  if (screen == 0) {
    display.println("Top 3 WiFi:");
    for (int i=0; i<3; i++) {
      if (topNetworks[i].ssid.length() > 0) {
        display.print(i+1);
        display.print(": ");
        display.print(topNetworks[i].ssid);
        display.print(" (");
        display.print(topNetworks[i].rssi);
        display.println("dBm)");
      }
    }
    display.println();
    display.println("Press D3 for details");
  } else {
    display.println("Details:");
    for (int i=0; i<3; i++) {
      if (topNetworks[i].ssid.length() > 0) {
        display.print(i+1);
        display.print(": ");
        display.println(topNetworks[i].ssid);
        display.print("RSSI: ");
        display.print(topNetworks[i].rssi);
        display.print("dBm\nType: ");
        display.println(topNetworks[i].encryption);
        display.println();
      }
    }
    display.println("Press D3 for list");
  }
  display.display();
}