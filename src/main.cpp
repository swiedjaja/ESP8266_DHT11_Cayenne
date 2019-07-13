#include <Arduino.h>
#include <DHT.h>
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include "wifi_id.h"

char username[] = "4eb04580-7518-11e8-81a0-d9ce919f53fe";
char password[] = "508bc6a98c77493c86d020a0e02db06882d5742e";
char clientID[] = "fd69e470-a521-11e9-9636-f9904f7b864b";

#define LED D4

DHT dht(D3, DHT11);
void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Serial.begin(115200);
  delay(700);
  dht.begin();
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
  Cayenne.loop();
}

CAYENNE_OUT_DEFAULT()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.printf("Humidity: %4.2f % Temperature: %4.2f °C\n", h, t);
	Cayenne.celsiusWrite(1, t);
	Cayenne.virtualWrite(2, h, TYPE_RELATIVE_HUMIDITY, UNIT_PERCENT);
}

CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %d", request.channel, getValue.asInt());
  if (request.channel==3)
  {
    digitalWrite(LED, !getValue.asInt());
  }
}