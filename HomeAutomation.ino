#ifdef ENABLE_DEBUG
   #define DEBUG_ESP_PORT Serial
   #define NODEBUG_WEBSOCKETS
   #define NDEBUG
#endif 

#include <Arduino.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32) || defined(ARDUINO_ARCH_RP2040)
  #include <WiFi.h>
#endif

#include "SinricPro.h"
#include "SinricProSwitch.h"
#include "SinricProTemperaturesensor.h"
#include <DHT.h>

// WiFi and SinricPro credentials
#define WIFI_SSID         "GG"
#define WIFI_PASS         "nahidunga"
#define APP_KEY           "a276340a-e1a7-41f6-b8dd-0bd97e3f71c2"
#define APP_SECRET        "8781737b-6f7b-43c2-9920-8c16a3cd7a31-8146abaf-ab2a-4616-a794-a6ea7f153f4d"

// Device IDs for SinricPro and relay pins
#define SWITCH_ID_1       "671b97e81fbebdc87b4771d3"   // LED
#define SWITCH_ID_2       "671b98071fbebdc87b47721a"   // Fan
#define TEMPERATURE_SENSOR_ID "671bad4d3770073bf63e2c02" // DHT11 Temperature Sensor

// Relay pin setup
#define RELAYPIN_1        26    // LED
#define RELAYPIN_2        25    // Fan

// DHT sensor setup
#define DHTPIN            4      // GPIO pin for DHT11 sensor
#define DHTTYPE           DHT11
DHT dht(DHTPIN, DHTTYPE);

#define BAUD_RATE         115200

// Callback function for the LED switch (active low)
bool onPowerState1(const String &deviceId, bool &state) {
  Serial.printf("Device 1 (LED) turned %s\n", state ? "on" : "off");
  digitalWrite(RELAYPIN_1, state ? LOW : HIGH);  // active-low relay
  return true; // Request handled properly
}

// Callback function for the fan switch (active low)
bool onPowerState2(const String &deviceId, bool &state) {
  Serial.printf("Device 2 (Fan) turned %s\n", state ? "on" : "off");
  digitalWrite(RELAYPIN_2, state ? LOW : HIGH);  // active-low relay
  return true; // Request handled properly
}

// Function to read temperature and humidity and send it to SinricPro
void sendTemperatureData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  if (!isnan(temperature) && !isnan(humidity)) {
    SinricProTemperaturesensor& myTempSensor = SinricPro[TEMPERATURE_SENSOR_ID];
    myTempSensor.sendTemperatureEvent(temperature, humidity);  // Send event to SinricPro
    Serial.printf("Temperature sent to SinricPro: %.1f°C\n", temperature);
    Serial.printf("Humidity sent to SinricPro: %.1f%%\n", humidity);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }
}

// Setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\n[WiFi]: Connecting");

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(250);
  }

  Serial.printf("\n[WiFi]: Connected! IP Address: %s\n", WiFi.localIP().toString().c_str());
}

// Setup function for SinricPro
void setupSinricPro() {
  // Initialize relay pins as OUTPUT and set to HIGH (off, as active low)
  pinMode(RELAYPIN_1, OUTPUT);
  digitalWrite(RELAYPIN_1, HIGH);
  
  pinMode(RELAYPIN_2, OUTPUT);
  digitalWrite(RELAYPIN_2, HIGH);

  // Attach switch devices and callbacks to SinricPro
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);

  SinricProSwitch& mySwitch2 = SinricPro[SWITCH_ID_2];
  mySwitch2.onPowerState(onPowerState2);

  // Attach the Temperature Sensor device
  SinricProTemperaturesensor& temperatureSensor = SinricPro[TEMPERATURE_SENSOR_ID];

  // Connection status handlers
  SinricPro.onConnected([]() { Serial.println("Connected to SinricPro"); });
  SinricPro.onDisconnected([]() { Serial.println("Disconnected from SinricPro"); });

  SinricPro.begin(APP_KEY, APP_SECRET);
}

// Main setup function
void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("\nStarting...");

  setupWiFi();
  setupSinricPro();
  
  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  SinricPro.handle();
  
  // Read and send DHT11 values every 2 seconds
  static unsigned long lastDHTReadTime = 0;
  if (millis() - lastDHTReadTime >= 2000) {
    lastDHTReadTime = millis();
    sendTemperatureData();
  }
}
