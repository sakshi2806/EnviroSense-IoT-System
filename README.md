# EnviroSense IoT System

This project demonstrates how to use an ESP32 microcontroller to capture motion and environmental data from sensors like the HTU21D and BH1750. The data is transmitted to a server via WiFi using WebSocket, enabling real-time monitoring.

## Table of Contents
- [Project Overview](#project-overview)
- [Features](#features)
- [Technologies Used](#technologies-used)
- [Components Required](#components-required)
- [Setup and Installation](#setup-and-installation)
   - [Hardware Setup](#hardware-setup)
   - [Software Setup](#software-setup)
- [Code Explanation](#code-explanation)
   - [ESP32 Code](#esp32-code)
   - [Python WebSocket Server Code](#python-websocket-server-code)
- [Usage Instructions](#usage-instructions)
- [Troubleshooting](#troubleshooting)
- [Author](#author)

## Project Overview
The **EnviroSense IoT System** is an IoT project that uses an ESP32 microcontroller to monitor environmental conditions like temperature, humidity, and light intensity. It sends the data to a WebSocket server for real-time monitoring and analysis, ideal for smart home environments or environmental monitoring applications.

## Features
* **Environmental Monitoring**: Measures temperature, humidity, and light levels.
* **WebSocket Communication**: Sends sensor data to a WebSocket server.
* **WiFi Connectivity**: Connects to a WiFi network to transmit data.

## Technologies Used
* ESP32 Microcontroller
* HTU21D Temperature and Humidity Sensor
* BH1750 Light Sensor
* Arduino IDE
* Python for WebSocket Server

## Components Required
* **Hardware**:
   * ESP32 microcontroller
   * HTU21D temperature and humidity sensor
   * BH1750 light sensor
   * Jumper wires
   * Breadboard

* **Software**:
   * Arduino IDE
   * Required Arduino libraries: `WiFi`, `WebSocketsClient`, `Wire`, `BH1750`, `ArduinoJson`
   * Python 3.x
   * Python libraries: `asyncio`, `websockets`, `json`

## Setup and Installation

### Hardware Setup
1. **Connect the HTU21D Sensor**:
   * **VCC** to 3.3V on ESP32
   * **GND** to GND on ESP32
   * **SDA** to GPIO 21 on ESP32
   * **SCL** to GPIO 22 on ESP32

2. **Connect the BH1750 Light Sensor**:
   * **VCC** to 3.3V on ESP32
   * **GND** to GND on ESP32
   * **SDA** to GPIO 21 on ESP32
   * **SCL** to GPIO 22 on ESP32

### Software Setup
1. **Install the Arduino IDE**: Download and install the Arduino IDE from the official Arduino website.
2. **Install Required Arduino Libraries**:
   * Open the Arduino IDE, go to **Sketch -> Include Library -> Manage Libraries**.
   * Search for and install the following libraries:
     - `WebSocketsClient` by Markus Sattler
     - `Wire` (usually pre-installed)
     - `BH1750` by Christopher Laws
     - `ArduinoJson` by Benoit Blanchon

3. **Configure the ESP32 Code**:
   * Update the WiFi credentials and WebSocket server details in the code:
   ```cpp
   const char* ssid = "YOUR_SSID";
   const char* password = "YOUR_PASSWORD";
   const char* ws_host = "YOUR_WS_SERVER_IP";
   const uint16_t ws_port = YOUR_WS_SERVER_PORT;
   const char* ws_path = "/YOUR_PATH";  

4. **Install Python and Required Libraries:**
   * Ensure Python 3.x is installed. You can download it from Python's official website.
   * Install required libraries using pip:
   ```cpp
   pip install websockets asyncio json
   ```
## Code Explanation
### ESP32 Code
The ESP32 code reads data from the HTU21D and BH1750 sensors and sends it to the WebSocket server via WiFi.
* **setup():** Initializes sensors, connects to WiFi, and establishes the WebSocket connection.
* **loop():** Maintains the WebSocket connection and calls functions to read sensor data.
* **sensorGatherAndSendTask():** Periodically reads sensor data (temperature, humidity, and light) and sends it over the WebSocket.

### Python WebSocket Server Code
#### Overview
The WebSocket server is implemented in Python and listens for incoming connections on port 8000. It handles incoming messages, processes JSON data, and prints out the temperature, humidity, and light intensity values.

#### Code Functions
* **handle_websocket(websocket, path):** Manages incoming WebSocket connections and messages, processes JSON data, and handles errors.
* **process_received_data(data):** Extracts temperature, humidity, and light intensity values from the received JSON data and prints them.
* **main():** Starts the WebSocket server and keeps it running indefinitely.

The server allows real-time data monitoring and logging from connected IoT devices.


## Usage Instructions
1. **Upload the ESP32 Code:**
    * Open the Arduino IDE.
    * Connect your ESP32 to your computer.
    * Select the correct board and port in the Arduino IDE.
    * Paste the updated code into the IDE.
    * Click **Upload** to flash the code to the ESP32.
2. **Run the WebSocket Server:**
    * Save the Python WebSocket server code to a file named websocket_server.py.
    * Run the server using Python:    
    ```cpp 
    python websocket_server.py
    ```
    * The server will start listening for incoming WebSocket connections on port 8000.
3. Monitor the Output:  
   * Open the Serial Monitor in the Arduino IDE to view sensor readings and WebSocket status messages.
   * The Python WebSocket server will print the received sensor data to the console.

## Troubleshooting
* **Connection Issues:**
   * Verify that the ESP32 is within the range of your WiFi network.
   * Confirm that the WiFi credentials are correct.
   * Ensure the WebSocket server IP, port, and path are correctly set.
* **Sensor Readings:**
   * Check sensor connections to ensure they are properly wired to the ESP32.
   * Verify sensor addresses match the code configuration.
* **WebSocket Server:**  
    * Ensure the Python WebSocket server is running and listening on the correct port.
    * Check the server logs for any connection or data handling errors.

## Author
Sakshi Mishra
