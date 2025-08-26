## DHT22 Sensor and 0.96" OLED Display Connections to Arduino Uno R3

### **1. DHT22 Sensor Connections**
- **VCC** (Power): Connect to **5V** on Arduino
- **GND** (Ground): Connect to **GND** on Arduino
- **DATA**: Connect to **Digital Pin 2** on Arduino  
  _(You may need a 10K resistor between DATA and VCC for stability)_

### **2. 0.96" OLED Display (Assuming I2C, SSD1306)**
- **VCC**: Connect to **3.3V** or **5V** on Arduino (check OLED voltage specification)
- **GND**: Connect to **GND** on Arduino
- **SCL**: Connect to **A5** on Arduino
- **SDA**: Connect to **A4** on Arduino

**Diagram:**

| Component | Arduino Uno Pin |
|-----------|----------------|
| DHT22 VCC | 5V             |
| DHT22 GND | GND            |
| DHT22 DATA| 2              |
| OLED VCC  | 3.3V/5V        |
| OLED GND  | GND            |
| OLED SCL  | A5             |
| OLED SDA  | A4             |
