//Generated Date: Mon, 22 Jan 2024 17:00:31 GMT

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#include <DHT.h>

boolean ledStatus = false;
const char* mqttLEDControlTopic = "xxxxxxxx/led_control";
const char* mqttLEDStatusTopic = "xxxxxxxx/led_status";
char _lwifi_ssid[] = "3715";
char _lwifi_pass[] = "12345678";
const char* mqtt_server = "mqttgo.io";
const unsigned int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient mqtt_client(espClient);
String mqtt_data = "";

void mqtt_sendText(String topic, String text) {
    String clientId = "(unique ID)";
    if (mqtt_client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      mqtt_client.publish(topic.c_str(), text.c_str());
    }
}

void reconnect() {
  while (!mqtt_client.connected()) {
    String mqtt_clientId = "(unique ID)";
    if (mqtt_client.connect(mqtt_clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      mqtt_client.subscribe(mqttLEDControlTopic);
    } else {
      delay(5000);
    }
  }
}

DHT dht11_p16(16, DHT11);
unsigned long timeEvent=0;

void setup()
{
  Serial.begin(115200);

  randomSeed(micros());
  mqtt_client.setServer(mqtt_server,mqtt_port);
  mqtt_client.setCallback(callback);
  //mqtt_client.setBufferSize(1024);

  WiFi.begin(_lwifi_ssid, _lwifi_pass);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  delay(300);
  Serial.println((String("WiFi connected to: ")+String("3715")+String(", IP Address: ")+String(WiFi.localIP().toString())));
  timeEvent=0;
  pinMode(2, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  dht11_p16.begin();
}

void loop()
{
  if ((WiFi.status() != WL_CONNECTED)) {
    Serial.println("WiFi connection is lost, reconnecting ....");
    WiFi.disconnect();
    WiFi.begin(_lwifi_ssid, _lwifi_pass);
    while (WiFi.status() != WL_CONNECTED) { delay(500); }
    delay(300);
    Serial.println((String("WiFi connected to: ")+String("xxxxxxxx")+String(", IP address: ")+String(WiFi.localIP().toString())));
  }
  if (!mqtt_client.connected()) {
    reconnect();
  }
  mqtt_client.loop();
  if (ledStatus) {
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
  } else {
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
  }
  if (timeEvent==0){
    timeEvent=millis();
    float t = dht11_p16.readTemperature();
    float h = dht11_p16.readHumidity();
    Serial.println((String("Temperature: ")+String(t)+String("°C, Humidity: ")+String(h)+String("%")));
    mqtt_sendText("xxxxxxxx/temp", String(t));
    mqtt_sendText("xxxxxxxx/humi", String(h));
  }
  if (millis()-timeEvent>=5000)
    timeEvent=0;
}

void callback(char* topic, byte* payload, unsigned int length) {
  mqtt_data = "";
  for (int ci = 0; ci < length; ci++) {
    char c = payload[ci];
    mqtt_data+=c;
  }
  if (String(topic)==mqttLEDControlTopic&&mqtt_data!="[]") {
    (mqtt_data).toLowerCase();
    if ((mqtt_data) == "on") {
      ledStatus = true;
      mqtt_sendText(mqttLEDStatusTopic, "on");
      Serial.println("LED on");
    } else if ((mqtt_data) == "off") {
      ledStatus = false;
      mqtt_sendText(mqttLEDStatusTopic, "off");
      Serial.println("LED off");
    }
  }
}
