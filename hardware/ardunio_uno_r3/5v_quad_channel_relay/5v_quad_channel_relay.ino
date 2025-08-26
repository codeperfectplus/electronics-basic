// 5V Dual Channel Relay Module with Arduino
// Define relay control pins
const int relay1Pin = 2;  // IN1 connected to digital pin 7
const int relay2Pin = 3;  // IN2 connected to digital pin 8
const int relay3Pin = 4;  // IN3 connected to digital pin 9
const int relay4Pin = 5;  // IN4 connected to digital pin 10

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set relay pins as outputs
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(relay3Pin, OUTPUT);
  pinMode(relay4Pin, OUTPUT);
  
  // Initialize relays as OFF (HIGH = OFF for most relay modules)
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  digitalWrite(relay3Pin, HIGH);
  digitalWrite(relay4Pin, HIGH);
  Serial.println("Quad Channel Relay Module Ready");
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

  // Turn on Relay 3
  Serial.println("Relay 3 ON");
  digitalWrite(relay3Pin, LOW);
  delay(2000);

  // Turn off Relay 3
  Serial.println("Relay 3 OFF");
  digitalWrite(relay3Pin, HIGH);
  delay(1000);

  // Turn on Relay 4
  Serial.println("Relay 4 ON");
  digitalWrite(relay4Pin, LOW);
  delay(2000);

  // Turn off Relay 4
  Serial.println("Relay 4 OFF");
  digitalWrite(relay4Pin, HIGH);
  delay(1000);

  // Turn on all relays
  Serial.println("All Relays ON");
  digitalWrite(relay1Pin, LOW);
  digitalWrite(relay2Pin, LOW);
  digitalWrite(relay3Pin, LOW);
  digitalWrite(relay4Pin, LOW);
  delay(2000);

  // Turn off all relays
  Serial.println("All Relays OFF");
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  digitalWrite(relay3Pin, HIGH);
  digitalWrite(relay4Pin, HIGH);
  delay(2000);

  // Add a delay before the next loop iteration
  delay(1000);
}