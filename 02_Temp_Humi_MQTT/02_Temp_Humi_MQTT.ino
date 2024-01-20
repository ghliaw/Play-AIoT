//Generated Date: Thu, 18 Jan 2024 02:15:44 GMT

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define MQTT_SERVER_IP "mqttgo.io"
#define MQTT_SERVER_PORT 1883
#define MQTT_ID "ghliaw-240117"
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#include <DHT.h>

float t = 0;
float h = 0;
char _lwifi_ssid[] = "DaBear";
char _lwifi_pass[] = "100godbl";
String receivedTopic="";
String receivedMsg="";
bool waitForE=true;
bool ended=true;
bool pubCtrl=false;

WiFiClient mqttClient;
PubSubClient myClient(mqttClient);

DHT dht11_p0(0, DHT11);
unsigned long timeEvent=0;
void connectMQTT(){
  while (!myClient.connected()){
    if (!myClient.connect(MQTT_ID,MQTT_USERNAME,MQTT_PASSWORD))
    {
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length){
  receivedTopic=String(topic);
  receivedMsg="";
  for (unsigned int myIndex = 0; myIndex < length; myIndex++)
  {
    receivedMsg += (char)payload[myIndex];
  }
  receivedMsg.trim();
  Serial.println((String("Received: ")+String(receivedTopic)+String(", ")+String(receivedMsg)));
  if (receivedTopic == "ghliaw240117/led") {
    receivedMsg.toLowerCase();
  }
  if (receivedMsg == "on") {
    digitalWrite(2, HIGH);
    Serial.println("LED on");
  } else if (receivedMsg == "off") {
    digitalWrite(2, LOW);
    Serial.println("LED off");
  }

}

void setup()
{
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  myClient.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  myClient.setCallback(mqttCallback);

  digitalWrite(2, LOW);
  dht11_p0.begin();
}

void loop()
{
  myClient.loop();
  connectMQTT();
  myClient.subscribe(String("ghliaw240117/led").c_str());
  if (timeEvent==0){
    timeEvent=millis();
    t = dht11_p0.readTemperature();
    h = dht11_p0.readHumidity();
    Serial.println((String("Temperature: ")+String(t)+String(", Humidity: ")+String(h)));
    myClient.publish(String("ghliaw240117/temp").c_str(),String(t).c_str());
    myClient.publish(String("ghliaw240117/humi").c_str(),String(h).c_str());
  }
  if (millis()-timeEvent>=5000)
    timeEvent=0;
}
