# 🌐 IoT Sensor Data Upload using ESP8266 & ThingSpeak

## 📌 Project Overview
This project demonstrates an **IoT-based system** that collects real-time environmental data using sensors and uploads it to the **ThingSpeak cloud platform** using the ESP8266 (NodeMCU).

The system measures:
- 🌡️ Temperature  
- 💧 Humidity  
- 🌬️ Pressure  
- 📏 Distance  

All data is visualized on the cloud for **real-time monitoring and analysis**.

---

## 🎯 Aim
To design and implement a system using ESP8266 that collects sensor data and uploads it to ThingSpeak for real-time monitoring.

---

## 🧠 Theory
- **ESP8266 (NodeMCU)**: WiFi-enabled microcontroller used for IoT applications.
- **BME280 Sensor**: Measures temperature, humidity, and pressure.
- **HC-SR04 Ultrasonic Sensor**: Measures distance using ultrasonic waves.
- **ThingSpeak Cloud**: Platform used to store and visualize sensor data.

---

## ⚙️ Components Required
- ESP8266 NodeMCU  
- BME280 Sensor  
- HC-SR04 Ultrasonic Sensor  
- Jumper Wires  
- Breadboard  
- WiFi Connection  

---

## 🔌 Working Principle
1. ESP8266 connects to a WiFi network.
2. Reads data from:
   - BME280 → Temperature, Humidity, Pressure
   - HC-SR04 → Distance
3. Sends data to ThingSpeak via HTTP.
4. ThingSpeak stores and displays data in graphical format.

---

## 💻 Code

```cpp
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

// WiFi
const char* ssid = "wifi";
const char* password = "alphabet01";

// ThingSpeak
String apiKey = "YOUR_API_KEY";
const char* server = "api.thingspeak.com";

// BME280
Adafruit_BME280 bme;

// Ultrasonic Pins
#define TRIG D5
#define ECHO D6

WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Wire.begin(D2, D1);

  if (!bme.begin(0x76)) {
    Serial.println("BME280 not found!");
    while (1);
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
}

float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2;
}

void loop() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0;
  float altitude = bme.readAltitude(1013.25);
  float distance = getDistance();

  Serial.println("---- DATA ----");
  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("Pressure: "); Serial.println(pressure);
  Serial.print("Distance: "); Serial.println(distance);

  if (client.connect(server, 80)) {
    String url = "/update?api_key=" + apiKey +
      "&field1=" + String(temperature) +
      "&field2=" + String(humidity) +
      "&field3=" + String(pressure) +
      "&field4=" + String(altitude) +
      "&field5=" + String(distance);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("Uploaded!");
  }

  client.stop();
  delay(15000);
}
```

---

## 📊 Output
- Data is successfully uploaded to ThingSpeak.
- Graphs are generated for:
  - Temperature
  - Humidity
  - Pressure
  - Altitude
  - Distance  

---

## 📁 Sample Data

| Temp (°C) | Humidity (%) | Pressure (hPa) | Altitude (m) | Distance (cm) |
|----------|-------------|---------------|--------------|--------------|
| 27.86 | 48.3 | 964.96 | 410.07 | 4.25 |
| 28.21 | 46.61 | 964.96 | 410.02 | 11.22 |
| 28.27 | 46.84 | 964.94 | 410.23 | 10.17 |

---

## 📈 Results
- Sensor data was successfully collected and transmitted.
- Real-time monitoring achieved via cloud visualization.
- System works reliably with periodic updates (~15 sec).

---

## ✅ Conclusion
This project demonstrates a practical implementation of IoT using ESP8266 for environmental monitoring. It provides a strong foundation for building advanced smart systems.

---

## 🚀 Future Improvements
- Add mobile app integration 📱  
- Implement alerts (SMS/Email) 🚨  
- Use more sensors (gas, motion, etc.)  
- Add data analytics & predictions  

---

## 👨‍💻 Author
**Aahan (2023BTech001)**  
B.Tech Systems  
JK Lakshmipat University  
