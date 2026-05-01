#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

// WiFi
const char* ssid = "wifi";
const char* password = "alphabet01";

// ThingSpeak
String apiKey = "VEN55UMHIRXD50DK";
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

  // BME280 init
  if (!bme.begin(0x76)) {
    Serial.println("BME280 not found!");
    while (1);
  }

  // WiFi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
}

// Ultrasonic function
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

  // ===== BME280 =====
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0;
  float altitude = bme.readAltitude(1013.25);

  // ===== Ultrasonic =====
  float distance = getDistance();

  Serial.println("---- DATA ----");
  Serial.print("Temp: "); Serial.println(temperature);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("Pressure: "); Serial.println(pressure);
  Serial.print("Distance: "); Serial.println(distance);

  // ===== ThingSpeak Upload =====
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