#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
 
#include "DHT.h"
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
 
#define AWS_IOT_PUBLISH_TOPIC   "dht11/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "dht11/sub"
 
float h;
float t;
 
DHT dht(DHTPIN, DHTTYPE);
 
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
 
void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");

  // Ladda certifikat och verifiera
  if (!net.setCACert(AWS_CERT_CA)) {
    Serial.println("Failed to load CA Certificate");
    return;
  }
  if (!net.setCertificate(AWS_CERT_CRT)) {
    Serial.println("Failed to load Device Certificate");
    return;
  }
  if (!net.setPrivateKey(AWS_CERT_PRIVATE)) {
    Serial.println("Failed to load Private Key");
    return;
  }

  // Konfigurera MQTT-server
  client.setServer(AWS_IOT_ENDPOINT, 8883);
  client.setCallback(messageHandler);

  Serial.println("Connecting to AWS IOT...");
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(500);
  }

  if (!client.connected())
  {
    Serial.println("\nAWS IoT Connection Failed! Check credentials and policy.");
    return;
  }

  Serial.println("\nAWS IoT Connected!");
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
}

 
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["humidity"] = h;
  doc["temperature"] = t;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Incoming message on topic: ");
  Serial.println(topic);

  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0'; // Lägg till null-terminator för att skapa en sträng

  Serial.print("Message: ");
  Serial.println(message);

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error) {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.f_str());
    return;
  }

  const char* msg = doc["message"];
  if (msg) {
    Serial.print("Parsed message: ");
    Serial.println(msg);
  } else {
    Serial.println("No 'message' field found in JSON.");
  }
}
 
void setup()
{
  Serial.begin(115200);
  connectAWS();
  dht.begin();
}
 
void ensureConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    connectAWS(); // Återanslut Wi-Fi och AWS
  }

  if (!client.connected()) {
    Serial.println("MQTT disconnected. Reconnecting...");
    while (!client.connected()) {
      if (client.connect(THINGNAME)) {
        Serial.println("Reconnected to MQTT!");
        client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC); // Prenumerera igen
      } else {
        Serial.print("Failed to reconnect to MQTT. State: ");
        Serial.println(client.state());
        delay(500);
      }
    }
  }
}

void loop() {
  ensureConnection(); // Säkerställ att både Wi-Fi och MQTT är anslutna

  h = dht.readHumidity();
  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) { // Kontrollera om sensorläsningen misslyckades
    Serial.println(F("Failed to read from DHT sensor! Retrying..."));
    delay(2000); // Vänta lite innan nästa försök
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C"));

  publishMessage(); // Publicera data till AWS IoT
  client.loop();    // Hantera MQTT-händelser
  delay(1000);      // Vänta innan nästa loop
}

