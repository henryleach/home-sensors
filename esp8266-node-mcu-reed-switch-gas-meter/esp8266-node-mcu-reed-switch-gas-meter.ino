/* henryleach.com
   ESP8266 with a KY-025 Reed sensor 'module'

  You will need to create a file mypasswords.h, and add
  your passwords to it:
  Copy and update the file mypasswords.h.template file

   Wiring for Node MCU 1.0 ESP-12E module
  | ESP8266 | KY-025     |
  |---------+------------|
  | 3v3     | VCC        |
  | GND     | GND        |
  | D1      | D0         |

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "passwords.h"
#include "gasmeterfuncs.h"

// Ribbon switch connections
#define REED_D_PIN 5 // GPIO5, is labelled D1?
int reedValue = LOW;
int previousReading = LOW;
int rotated = 0;

int interval = 500; // miliseconds

WiFiClient espClient;
PubSubClient client(espClient);
char client_id[40] = "Node-ESP8266-";
char gastopic[60] = "utility/gas/";
char messagebuf[6];

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

  // Reed switch in
  pinMode(REED_D_PIN, INPUT);

  // For the built in LED, setup and set to HIGH for off
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  // Flash twice when starting, to differentiate from a
  // recording flash
  delay(200);
  digitalWrite(16, HIGH);
  delay(200);
  digitalWrite(16, LOW);
  delay(200);
  digitalWrite(16, HIGH); // and off

  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect

  }

  connect_wifi(SSID, WIFIPASSWORD);

  // Create a (probably) unique name with the mac address
  strcat(client_id, WiFi.macAddress().c_str());
  strcat(gastopic, client_id);

  client.setServer(mqttserver, 1883);
  // Will have to test keepalive, the gas readings
  // can be sporadic, with long periods of no transmission.
  client.setKeepAlive(120);

}


void loop() {

  if (!client.connected()) {
    reconnect_mqtt(client_id, MQTTUSER, MQTTPASSWORD);
  }

  client.loop();

  previousReading = reedValue;
  reedValue = digitalRead(REED_D_PIN);

  if ((previousReading == HIGH) and (reedValue == LOW)) {

    rotated = 10; //set to 10 litres per rotation
    // Flash the LED on
    digitalWrite(16, LOW);

    dtostrf(rotated, 0, 0, messagebuf);

    client.publish(gastopic, messagebuf);
    Serial.println("Rotation detected, MQTT Message sent");
    // shorten delay in sampling, as we've spent time doing other
    // things instead.
    interval = 1;

  }


  delay(interval);

  if (rotated != 0) {
    // Reset rotation marker so it's not kept over
    // multiple cycles.
    rotated = 0;
    interval = 500;
    digitalWrite(16, HIGH);
  }
}
