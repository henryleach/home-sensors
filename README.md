# Home Sensors

Code for various microcontrollers that send sensor data via MQTT back to a home server running the [store-mqtt-data(https://github.com/henryleach/store-mqtt-data) Python script.

All sensors require a 'passwords.h' file, which contains the passwords and IDs of the WiFi network it needs to connect to, and the MQTT server.

Use the 'passwords.h.template' file, remove the '.template' part and save it in each of the folders of the projects below.

These can then be compiled and loaded using the [Ardino IDE](https://www.arduino.cc/en/software).

Beware, that to prevent accidentally syncing your passwords to a public repo, 'passwords.h' is included in this repository's .gitignore file. If you DO want to version control these in plain text (don't!) remove that entry.

## d1-mini-htu21-temp-hum-mqtt

Runs an WEMOS D1 Mini, with an ESP8266 Microcontroller and a HTU21 temperature and humidity sensor.

## esp8266-node-mcu-reed-switch-gas-meter

Reads a Elster gas metere with a ribbon switch
