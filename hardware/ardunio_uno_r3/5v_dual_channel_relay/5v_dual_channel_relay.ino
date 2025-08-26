// 5V Dual Channel Relay Module with Arduino
// Define relay control pins
const int relay1Pin = 7;  // IN1 connected to digital pin 7
const int relay2Pin = 8;  // IN2 connected to digital pin 8

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set relay pins as outputs
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  
  // Initialize relays as OFF (HIGH = OFF for most relay modules)
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  
  Serial.println("Dual Channel Relay Module Ready");
}

void loop() {
  // Turn on Relay 1
  Serial.println("Relay 1 ON");
  digitalWrite(relay1Pin, LOW);  // LOW = ON for most relay modules
  delay(2000);
  
  // Turn off Relay 1
  Serial.println("Relay 1 OFF");
  digitalWrite(relay1Pin, HIGH); // HIGH = OFF for most relay modules
  delay(1000);
  
  // Turn on Relay 2
  Serial.println("Relay 2 ON");
  digitalWrite(relay2Pin, LOW);
  delay(2000);
  
  // Turn off Relay 2
  Serial.println("Relay 2 OFF");
  digitalWrite(relay2Pin, HIGH);
  delay(1000);
  
  // Turn on both relays
  Serial.println("Both Relays ON");
  digitalWrite(relay1Pin, LOW);
  digitalWrite(relay2Pin, LOW);
  delay(2000);
  
  // Turn off both relays
  Serial.println("Both Relays OFF");
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  delay(2000);
}