#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Thi?t l?p th�ng tin k?t n?i WiFi
const char* ssid = "iPhone";
const char* password = "20022001";

// Thi?t l?p th�ng tin k?t n?i MQTT
const char* mqtt_server = "172.20.10.7";
const int mqtt_port = 1883;
const char* mqtt_topic = "sensor/data";
const char* mqtt_lightbulb1_topic = "sensor/Light_bulb1";
const char* mqtt_lightbulb2_topic = "sensor/Light_bulb2";

// Thi?t l?p ch�n di?u khi?n d�n
const int lightPin1 = 13;
const int lightPin2 = 12;

// Thi?t l?p ch�n k?t n?i c?m bi?n DHT11
const int dhtPin = 4;
const int dhtType = DHT22;

// Kh?i t?o d?i tu?ng WiFiClient d? k?t n?i WiFi
WiFiClient wifiClient;

// Kh?i t?o d?i tu?ng PubSubClient
PubSubClient client(wifiClient);

// Kh?i t?o d?i tu?ng c?m bi?n DHT22
DHT dht(dhtPin, dhtType);

// H�m callback du?c g?i khi nh?n du?c tin nh?n t? m�y ch? MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  // Chuy?n d?i payload th�nh chu?i k� t?
  char payloadStr[length + 1];
  memcpy(payloadStr, payload, length);
  payloadStr[length] = '\0';

  // Ki?m tra topic c?a tin nh?n
  if (strcmp(topic, mqtt_lightbulb1_topic) == 0) {
    // Ki?m tra gi� tr? c?a payload d? di?u khi?n d�n 1
    if (strcmp(payloadStr, "1") == 0) {
      digitalWrite(lightPin1, HIGH);  // B?t d�n 1
    } else if (strcmp(payloadStr, "0") == 0) {
      digitalWrite(lightPin1, LOW);   // T?t d�n 1
    }
  } else if (strcmp(topic, mqtt_lightbulb2_topic) == 0) {
    // Ki?m tra gi� tr? c?a payload d? di?u khi?n d�n 2
    if (strcmp(payloadStr, "1") == 0) {
      digitalWrite(lightPin2, HIGH);  // B?t d�n 2
    } else if (strcmp(payloadStr, "0") == 0) {
      digitalWrite(lightPin2, LOW);   // T?t d�n 2
    }
  }
}

void setup() {
  // Kh?i t?o ch�n di?u khi?n d�n l� OUTPUT
  pinMode(lightPin1, OUTPUT);
  pinMode(lightPin2, OUTPUT);
  Serial.begin(9600);
  // K?t n?i d?n m?ng WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Thi?t l?p m�y ch? MQTT v� callback
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // K?t n?i t?i m�y ch? MQTT
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT server");
      // �ang k� d? nh?n tin nh?n t? c�c topic c?n theo d�i
      client.subscribe(mqtt_lightbulb1_topic);
      client.subscribe(mqtt_lightbulb2_topic);
    } else {
      Serial.print("Failed to connect to MQTT server, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying...");
      delay(5000);
    }
  }

  // Kh?i t?o c?m bi?n DHT11
  dht.begin();
}

void loop() {
  // Ki?m tra k?t n?i d?n m�y ch? MQTT
  if (!client.connected()) {
    Serial.println("Lost connection to MQTT server. Reconnecting...");
   // Th? k?t n?i l?i
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT server");
      // �ang k� d? nh?n tin nh?n t? c�c topic c?n theo d�i
      client.subscribe(mqtt_lightbulb1_topic);
      client.subscribe(mqtt_lightbulb2_topic);
    } else {
      Serial.print("Failed to connect to MQTT server, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying...");
      delay(5000);
    }
  }

  // X? l� th�ng di?p MQTT
  client.loop();

  // �?c nhi?t d? v� d? ?m t? c?m bi?n DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Ki?m tra tr?ng th�i d�n v� t?o chu?i JSON
  int lightState1 = digitalRead(lightPin1);
  int lightState2 = digitalRead(lightPin2);
  Serial.println(temperature);
  Serial.println(humidity);
  String jsonMessage = "{\"temperature\":" + String(temperature) +
                       ",\"humidity\":" + String(humidity) +
                       ",\"lightbulb1\":" + String(lightState1) +
                       ",\"lightbulb2\":" + String(lightState2) + "}";

  // G?i tin nh?n JSON qua MQTT
  client.publish(mqtt_topic, jsonMessage.c_str());

  // Ch? 2 gi�y tru?c khi g?i tin nh?n ti?p theo
  delay(2000);
}
