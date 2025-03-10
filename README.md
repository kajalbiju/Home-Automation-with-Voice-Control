# SinricPro Demo for ESP

This project demonstrates how to control a LED and a fan using the SinricPro API and monitor temperature/humidity using a DHT11 sensor. It is designed for ESP-based boards (such as ESP8266, ESP32, or Arduino RP2040) and leverages the SinricPro cloud platform to manage smart devices.

## Features

- **Device Control:**  
  Control an LED and a fan via SinricPro.
  
- **Temperature & Humidity Monitoring:**  
  Read and send temperature and humidity data from a DHT11 sensor.

- **Multiple Board Support:**  
  Compatible with ESP8266, ESP32, and Arduino RP2040 boards.

## Requirements

- **Arduino IDE** (or PlatformIO)
- **SinricPro Library:**  
  Includes support for SinricProSwitch and SinricProTemperaturesensor.
- **ArduinoJson Library:**  
  Ensure you have version 6.x installed.
- **DHT Sensor Library:**  
  The DHT sensor library by Adafruit for interfacing with the DHT11 sensor.
- **WiFi Libraries:**  
  - ESP8266: Uses `ESP8266WiFi.h`
  - ESP32 / Arduino RP2040: Uses `WiFi.h`

## Setup and Installation

1. **Clone or Download the Repository**  
   Clone this repository or download the source code and extract it to your local machine.

2. **Install Required Libraries**  
   Open the Arduino IDE and navigate to **Sketch > Include Library > Manage Libraries...**  
   Search for and install the following libraries:
   - **SinricPro**
   - **ArduinoJson**
   - **DHT sensor library** (by Adafruit)

3. **Configure Your Credentials**  
   Open the `SinricProDemo.ino` file and update the following placeholders with your actual credentials:
   - **WiFi Credentials:**  
     Replace `WIFI_SSID` and `WIFI_PASS` with your network name and password.
   - **SinricPro Credentials:**  
     Replace `APP_KEY` and `APP_SECRET` with your SinricPro API key and secret.
   - **Device IDs:**  
     Update `SWITCH_ID_1`, `SWITCH_ID_2`, and `TEMPERATURE_SENSOR_ID` with your SinricPro device IDs.

4. **Select Your Board**  
   In the Arduino IDE, go to **Tools > Board** and select your appropriate board (ESP8266, ESP32, or Arduino RP2040).

5. **Upload the Sketch**  
   Connect your board via USB and click the **Upload** button in the Arduino IDE.

## How It Works

- **Device Control:**  
  The sketch uses callback functions to control the state of the LED and fan. When a command is received from SinricPro, it toggles the corresponding relay (active-low logic is used).

- **Sensor Data:**  
  Every 2 seconds, the sketch reads temperature and humidity data from the DHT11 sensor and sends the data to SinricPro. The serial monitor will display these readings for debugging purposes.

## Troubleshooting

- **Connection Issues:**  
  Make sure your board is within range of your WiFi network and that the credentials are correct.

- **Compilation Errors:**  
  Verify that all required libraries are installed and that you have selected the correct board in the Arduino IDE.

- **Serial Monitor:**  
  Open the serial monitor at a baud rate of 115200 to view debug messages and sensor readings.

## License

This project is open-source. You are free to modify and use the code for personal or commercial projects.

## Acknowledgments

- [SinricPro](https://sinric.pro/) for the smart home API.
- [Adafruit](https://www.adafruit.com/) for the DHT sensor library.
