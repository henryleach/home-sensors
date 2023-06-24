/* henryleach.com
 * Script for temperture and humidity sensor (HTU21), run on a Wemos D1 Mini ESP8266,
 * that connects to an MQTT broker and transmits the readings
 * www.henryleach.com
 * 
 * In the Arduino IDE select the board 'Generic ESP8266 Module'.
 *
 * Connections:
 * | D1 Mini      | HTU21 |
 * |--------------+-------|
 * | 5V or 3.3V   | VIN   |
 * | GND (pin 15) | GND   |
 * | D3 (pin 0)   | SCL   |
 * | D2 (pin 4)   | SDA   |
 * 
 */
 
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "passwords.h"
#include "minitempfuncs.h"

const int HTU21ADDRESS = 0x40;
float tempr, humidity;
char tempbuff[6], humbuff[5];
WiFiClient espClient;
PubSubClient client(espClient);
char client_id[40] = "D1-mini-";
char temptopic[60] = "env/temp/";
char humtopic[60] = "env/humidity/";

void reconnect_mqtt(const char* client_id, const char* mqttusername, const char* mqttpassword) {
  while (!client.connected()) {
    Serial.println("Attempting to connect to broker");

    if (client.connect(client_id, mqttusername, mqttpassword)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 sec.");
      delay(5000);
    }
  }
}

void setup() {

  // Turn the LED off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED is on pin 2.

  Serial.begin(9600);
  delay(10);

  connect_wifi(SSID, WIFIPASSWORD);
  sensor_init(HTU21ADDRESS);

  // Create a (probably) unique name with the mac address
  strcat(client_id, WiFi.macAddress().c_str());

  strcat(temptopic, client_id);
  strcat(humtopic, client_id);
  Serial.println(temptopic);
  Serial.println(humtopic);
  client.setServer(mqttserver, 1883);
  client.setKeepAlive(80); // transmitting every ~minute, so don't disconnect in between.
  
}

void loop() {

    if (!client.connected()) {
    reconnect_mqtt(client_id, MQTTUSER, MQTTPASSWORD);
  }

  client.loop();

  tempr = read_temp(HTU21ADDRESS);
  humidity = read_humidity(HTU21ADDRESS);
  
  dtostrf(tempr, 0, 1, tempbuff);
  Serial.print(tempbuff);
  Serial.print("˚C, ");

  client.publish(temptopic, tempbuff);

  dtostrf(humidity, 0, 0, humbuff);
  Serial.print(humbuff);
  Serial.println("%");
  
  client.publish(humtopic, humbuff);

  delay(56e3); // miliseconds

  
}
