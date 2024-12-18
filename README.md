# AWS IoT DHT11 Sensor Project

This project demonstrates how to use an ESP32 microcontroller to read temperature and humidity data from a DHT11 sensor and send this data to AWS IoT Core using MQTT. The project is implemented using Arduino IDE.

## Features

- Connects an ESP32 to AWS IoT Core using secure MQTT.
- Reads temperature and humidity values from a DHT11 sensor.
- Publishes sensor data to an AWS IoT topic.
- Listens for incoming MQTT messages from AWS IoT.

## Folder Structure

```
ESP32_to_AWS/
├── ESP32_to_AWS.ino   # Main Arduino sketch
├── secrets.h          # Wi-Fi and AWS IoT credentials (not uploaded to GitHub)
├── .gitignore         # To exclude secrets.h and other sensitive files
```

## Requirements

### Hardware

- ESP32 Development Board
- DHT11 Temperature and Humidity Sensor
- Breadboard and jumper wires

### Software

- Arduino IDE
- ESP32 Board Manager installed in Arduino IDE
- Required Arduino libraries:
  - `WiFi.h` (built-in for ESP32)
  - `WiFiClientSecure.h` (built-in for ESP32)
  - `PubSubClient` (install via Library Manager)
  - `ArduinoJson` (install via Library Manager)
  - `DHT` (install via Library Manager)

## Setup

1. **Hardware Setup:**

   - Connect the DHT11 sensor to the ESP32:
     - VCC: 3.3V on ESP32
     - GND: GND on ESP32
     - DATA: GPIO 2 (can be changed in the code)

2. **Software Setup:**

   - Clone this repository to your local machine.
   - Open `ESP32_to_AWS.ino` in Arduino IDE.
   - Configure your `secrets.h` file with the following details:
     ```cpp
     #ifndef SECRETS_H
     #define SECRETS_H

     #define WIFI_SSID "YourWiFiSSID"
     #define WIFI_PASSWORD "YourWiFiPassword"
     #define AWS_IOT_ENDPOINT "YourAWSEndpoint"

     static const char AWS_CERT_CA[] PROGMEM = R"EOF(
     -----BEGIN CERTIFICATE-----
     YourRootCACertificate
     -----END CERTIFICATE-----
     )EOF";

     static const char AWS_CERT_CRT[] PROGMEM = R"EOF(
     -----BEGIN CERTIFICATE-----
     YourDeviceCertificate
     -----END CERTIFICATE-----
     )EOF";

     static const char AWS_CERT_PRIVATE[] PROGMEM = R"EOF(
     -----BEGIN PRIVATE KEY-----
     YourPrivateKey
     -----END PRIVATE KEY-----
     )EOF";

     #define THINGNAME "ESP32_DHT11"

     #endif
     ```

3. **Upload Code:**

   - Select your ESP32 board and port in Arduino IDE.
   - Compile and upload the code to your ESP32.

## Usage

1. Monitor the Serial Output:

   - Open the Serial Monitor in Arduino IDE at 115200 baud rate.
   - Verify connection to Wi-Fi and AWS IoT Core.
   - Observe sensor data being published to AWS IoT Core.

2. Test MQTT Functionality:

   - Use the AWS IoT Core MQTT Test Client to:
     - Subscribe to `dht11/pub` to view published sensor data.
     - Publish test messages to `dht11/sub` to observe incoming messages on the Serial Monitor.

## Troubleshooting

### Common Issues

1. **Stuck on "Connecting to AWS IOT":**

   - Verify the AWS IoT endpoint in `secrets.h`.
   - Check your AWS IoT certificates and ensure they are valid and activated.
   - Confirm that port 8883 is open on your network.

2. **No Sensor Data:**

   - Ensure the DHT11 sensor is correctly connected.
   - Replace the sensor if it is faulty.

### Debugging Steps

- Use the Serial Monitor for detailed logs.
- Ensure the ESP32 has a stable power supply.

## License

This project is licensed under the MIT License. See the LICENSE file for details.

---

Feel free to contribute or open an issue if you encounter any problems. Happy coding!

