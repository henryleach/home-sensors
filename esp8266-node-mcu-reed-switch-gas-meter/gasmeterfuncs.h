#ifndef GASMETERFUNCS_H
#define GASMETERFUNCS_H

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
