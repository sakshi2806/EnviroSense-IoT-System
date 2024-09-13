#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <Wire.h>
#include <BH1750.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "ssid";
const char* password = "password";

#define deviceId 1

#define HTU21D_ADDRESS 0x40  // I2C address of HTU21D sensor

// WebSocket server details
const char* ws_host = "YOUR_WS_SERVER_IP";  // Remove leading space
const uint16_t ws_port = YOUR_WS_SERVER_PORT;
const char* ws_path = "/YOUR_PATH";  // Uncomment and adjust if necessary

WebSocketsClient webSocket;
BH1750 lightMeter;

float temp, hum, lux;   // Variables to store sensor data
unsigned long lastTime = 0;
unsigned long timerDelay = 2000;  // Timer delay for sensor reading

// WebSocket event handler
void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("WebSocket Disconnected!");
      break;
    case WStype_CONNECTED:
      Serial.println("WebSocket Connected!");
      break;
    case WStype_TEXT:
      Serial.printf("Received text: %s\n", payload);
      break;
    case WStype_BIN:
      Serial.println("Binary message received");
      break;
    case WStype_ERROR:
      Serial.println("WebSocket Error!");
      break;
    case WStype_PING:
      Serial.println("WebSocket Ping!");
      break;
    case WStype_PONG:
      Serial.println("WebSocket Pong!");
      break;
  }
}

// Combined task for reading sensors and sending data over WebSocket
void sensorGatherAndSendTask(void* parameter) {
  while (1) {
    if ((millis() - lastTime) > timerDelay) {
      temp = roundf(readTemperature() * 100) / 100.0;  // Round to 2 decimal places
      hum = roundf(readHumidity() * 100) / 100.0;      // Round to 2 decimal places
      lux = roundf(lightMeter.readLightLevel() * 100) / 100.0;  // Round to 2 decimal places

      Serial.printf("Temperature: %.2f C, Humidity: %.2f%%, Lux: %.2f lx\n", temp, hum, lux);

      // Prepare JSON data
      DynamicJsonDocument doc(1024);
      doc["mac_address"] = WiFi.macAddress();

      JsonObject tempObj = doc.createNestedObject("temperature");
      tempObj["temp"] = temp;
      tempObj["sensor_id"] =  deviceId;

      JsonObject humObj = doc.createNestedObject("humidity");
      humObj["humi"] = hum;
      humObj["sensor_id"] =  deviceId;

      JsonObject luxObj = doc.createNestedObject("luminescence");
      luxObj["lux"] = lux;
      luxObj["sensor_id"] =  deviceId;

      // Serialize JSON to string
      String jsonData;
      serializeJson(doc, jsonData);

      // Send the data over WebSocket if connected
      if (WiFi.status() == WL_CONNECTED) {
        if (webSocket.isConnected()) {
          webSocket.sendTXT(jsonData);
        } else {
          Serial.println("WebSocket not connected");
        }
      } else {
        Serial.println("WiFi disconnected");
      }

      lastTime = millis();
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);  // Adjust the delay to avoid busy-looping
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize light sensor (BH1750)
  Wire.begin();
  lightMeter.begin();
  
  // Connect to WiFi
  connect_to_wifi(ssid, password);
  
  // Setup WebSocket connection
  webSocket.begin(ws_host, ws_port, ws_path);  // Use ws_path if needed
  webSocket.onEvent(webSocketEvent);

  // Create task for both sensor gathering and WebSocket sending
  xTaskCreatePinnedToCore(sensorGatherAndSendTask, "SensorGatherAndSendTask", 10000, NULL, 1, NULL, 0);
}

void loop() {
  // WebSocket loop
  webSocket.loop();
}

// Connect to WiFi function
void connect_to_wifi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  int wifi_counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    if (wifi_counter == 10) {
      ESP.restart();
    }
    wifi_counter++;
  }
  Serial.println("Connected to WiFi");
}

float readHumidity() {
  Wire.beginTransmission(HTU21D_ADDRESS);
  Wire.write(0xE5); 
  Wire.endTransmission();

  delay(50);  

  Wire.requestFrom(HTU21D_ADDRESS, 2);
  if (Wire.available() == 2) {
    uint16_t rawHumidity = Wire.read() << 8 | Wire.read();
    float humidity = -6.0 + 125.0 * (rawHumidity / 65536.0);
    return humidity;
  } else {
    return NAN; 
  }
}

float readTemperature() {
  Wire.beginTransmission(HTU21D_ADDRESS);
  Wire.write(0xE3); 
  Wire.endTransmission();

  delay(50); 

  Wire.requestFrom(HTU21D_ADDRESS, 2);
  if (Wire.available() == 2) {
    uint16_t rawTemperature = Wire.read() << 8 | Wire.read();
    float temperature = -46.85 + 175.72 * (rawTemperature / 65536.0);
    return temperature;
  } else {
    return NAN; 
  }
}
