//Generated Date: Wed, 17 Jan 2024 14:41:09 GMT

#include <DHT.h>

float t = 0;
float h = 0;
DHT dht11_p16(16, DHT11);

void setup()
{
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  dht11_p16.begin();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  t = dht11_p16.readTemperature();
  h = dht11_p16.readHumidity();
  Serial.println((String("Temperature: ")+String(t)+String(", Humidity: ")+String(h)));
  if (digitalRead(LED_BUILTIN)) {
    Serial.println("LED On");
  } else {
    Serial.println("LED Off");
  }
  digitalWrite(2, (!digitalRead(LED_BUILTIN)));
  delay(5000);
}
