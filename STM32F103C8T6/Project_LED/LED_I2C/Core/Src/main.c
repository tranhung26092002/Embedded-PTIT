#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Thi?t l?p thông tin k?t n?i WiFi
const char* ssid = "iPhone";
const char* password = "20022001";

// Thi?t l?p thông tin k?t n?i MQTT
const char* mqtt_server = "172.20.10.7";
const int mqtt_port = 1883;
const char* mqtt_topic = "sensor/data";
const char* mqtt_lightbulb1_topic = "sensor/Light_bulb1";
const char* mqtt_lightbulb2_topic = "sensor/Light_bulb2";

// Thi?t l?p chân di?u khi?n dèn
const int lightPin1 = 13;
const int lightPin2 = 12;

// Thi?t l?p chân k?t n?i c?m bi?n DHT11
const int dhtPin = 4;
const int dhtType = DHT22;

// Kh?i t?o d?i tu?ng WiFiClient d? k?t n?i WiFi
WiFiClient wifiClient;

// Kh?i t?o d?i tu?ng PubSubClient
PubSubClient client(wifiClient);

// Kh?i t?o d?i tu?ng c?m bi?n DHT22
DHT dht(dhtPin, dhtType);

// Hàm callback du?c g?i khi nh?n du?c tin nh?n t? máy ch? MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  // Chuy?n d?i payload thành chu?i ký t?
  char payloadStr[length + 1];
  memcpy(payloadStr, payload, length);
  payloadStr[length] = '\0';

  // Ki?m tra topic c?a tin nh?n
  if (strcmp(topic, mqtt_lightbulb1_topic) == 0) {
    // Ki?m tra giá tr? c?a payload d? di?u khi?n dèn 1
    if (strcmp(payloadStr, "1") == 0) {
      digitalWrite(lightPin1, HIGH);  // B?t dèn 1
    } else if (strcmp(payloadStr, "0") == 0) {
      digitalWrite(lightPin1, LOW);   // T?t dèn 1
    }
  } else if (strcmp(topic, mqtt_lightbulb2_topic) == 0) {
    // Ki?m tra giá tr? c?a payload d? di?u khi?n dèn 2
    if (strcmp(payloadStr, "1") == 0) {
      digitalWrite(lightPin2, HIGH);  // B?t dèn 2
    } else if (strcmp(payloadStr, "0") == 0) {
      digitalWrite(lightPin2, LOW);   // T?t dèn 2
    }
  }
}

void setup() {
  // Kh?i t?o chân di?u khi?n dèn là OUTPUT
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

  // Thi?t l?p máy ch? MQTT và callback
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // K?t n?i t?i máy ch? MQTT
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT server");
      // Ðang ký d? nh?n tin nh?n t? các topic c?n theo dõi
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
  // Ki?m tra k?t n?i d?n máy ch? MQTT
  if (!client.connected()) {
    Serial.println("Lost connection to MQTT server. Reconnecting...");
   // Th? k?t n?i l?i
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT server");
      // Ðang ký d? nh?n tin nh?n t? các topic c?n theo dõi
      client.subscribe(mqtt_lightbulb1_topic);
      client.subscribe(mqtt_lightbulb2_topic);
    } else {
      Serial.print("Failed to connect to MQTT server, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying...");
      delay(5000);
    }
  }

  // X? lý thông di?p MQTT
  client.loop();

  // Ð?c nhi?t d? và d? ?m t? c?m bi?n DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Ki?m tra tr?ng thái dèn và t?o chu?i JSON
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

  // Ch? 2 giây tru?c khi g?i tin nh?n ti?p theo
  delay(2000);
}
