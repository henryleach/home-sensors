/*
 * Functions for simple MQTT temperature and humidty sensor 
 * based on Wemos D1 Mini board and ESP8266 chip
*/

#ifndef minitempfuncs
#define minitempfuncs

void sensor_init(const int addr) {
  Wire.begin(4, 0); // SDA, SCL https://arduino-esp8266.readthedocs.io/en/latest/libraries.html#i2c-wire-library
  delay(100);
  Wire.beginTransmission(addr);
  Wire.endTransmission();
}

double read_temp(const int addr) {
  double temperature;
  int low_byte, high_byte, raw_data;
  // Send command to get temp
  Wire.beginTransmission(addr);
  Wire.write(0xE3);
  Wire.endTransmission();
  // Read the temp
  Wire.requestFrom(addr, 2);
  if (Wire.available() <= 2) {
    high_byte = Wire.read();
    low_byte = Wire.read();
    high_byte = high_byte << 8;
    raw_data = high_byte + low_byte;
  }
  temperature = (175.72 * raw_data) / 65536; 
  temperature = temperature - 46.85;
  return temperature;
}

float read_humidity(const int addr) {
  float humidity, raw_data_1, raw_data_2;
  int low_byte, high_byte, container;
  
  Wire.beginTransmission(addr);
  Wire.write(0xE5);
  Wire.endTransmission();
  
  Wire.requestFrom(addr, 2);
  if (Wire.available() <= 2) {
    high_byte = Wire.read();
    container = high_byte / 100;
    high_byte = high_byte % 100;
    low_byte = Wire.read();
    raw_data_1 = container * 25600;
    raw_data_2 = high_byte * 256 + low_byte;
  }
  raw_data_1 = (125 * raw_data_1) / 65536;
  raw_data_2 = (125 * raw_data_2) / 65536;
  humidity = raw_data_1 + raw_data_2;
  humidity = humidity - 6;
  return humidity;
}

void connect_wifi(const char* ssid, const char* wifipassword){
  WiFi.begin(ssid, wifipassword);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.println(WiFi.macAddress());

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    // Wait for the connection
    delay(1000);
    Serial.print(++i); Serial.print(" ");
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());  
}


#endif
