#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define relayA D3
#define relayB D4

#define WLAN_SSID "RH"
#define WLAN_PASS "makassar92"

#define IO_SERVER "io.adafruit.com"
#define IO_SERVERPORT 1883
#define IO_USERNAME "rintoharahap"
#define IO_KEY "aio_ZgKS25j63EH5gwsC2xVXDF5DdBU1"

WiFiClient client;

Adafruit_MQTT_Client mqtt(client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);

Adafruit_MQTT_Subscribe light1 = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME "/feeds/saklar1");
Adafruit_MQTT_Subscribe light2 = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME "/feeds/saklar2");

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(12, OUTPUT);
  pinMode(relayA, OUTPUT);
  pinMode(relayB, OUTPUT);

  // connect wifi
  Serial.print("Menghubungkan ke  ");
  Serial.println(WLAN_SSID);
  digitalWrite(12, LOW);

  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("=");
  }

  Serial.println("Wifi terhubung");

  mqtt.subscribe(&light1);
  mqtt.subscribe(&light2);

  digitalWrite(12, HIGH);
}

void loop()
{
  // put your main code here, to run repeatedly:

  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription))
  {

    // relay 1
    if (subscription == &light1)
    {
      Serial.print("light1: ");
      Serial.println((char *)light1.lastread);
      if (!strcmp((char *)light1.lastread, "ON"))
      {
        digitalWrite(relayA, HIGH);
      }
      else if (!strcmp((char *)light1.lastread, "OFF"))
      {
        digitalWrite(relayA, LOW);
      }
    }

    // relay 2
    if (subscription == &light2)
    {
      Serial.print("light2: ");
      Serial.println((char *)light2.lastread);
      if (!strcmp((char *)light2.lastread, "ON"))
      {
        digitalWrite(relayB, HIGH);
      }
      else if (!strcmp((char *)light2.lastread, "OFF"))
      {
        digitalWrite(relayB, LOW);
      }
    }
  }
}

// menghubungkan MQTT Server

void MQTT_connect()
{
  int8_t ret;

  if (mqtt.connected())
  {
    return;
  }

  Serial.println("MQTT Connected");
}
