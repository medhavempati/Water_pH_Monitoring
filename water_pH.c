#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"
#include <EEPROM.h>

char* wigi_ssid = "name";
char* wifi_pwd = "pswd";
String cse_ip = "139.59.42.21";

String cse_port = "8080";
String server = "http://" + cse_ip + ":" + cse_port + "/~/in-cse/in-name";
String createCI(String server, String ae, String cnt, String val){
  HTTPClient http;
  http.begin(server + ae + "/" + cnt + "/");
  http.addHeader("X-M2M-Origin", "admin:admin");
   http.addHeader("Content-Type", "application/json;ty=4");
   http.addHeader("Content-Length", "100");
   http.addHeader("Connection", "close");
   int code = http.POST("{\"m2m:cin\":{\"cnf\":\"text/plain:0\",\"con\": " + String(val) + "}}");
   http.end();
   Serial.print(code);
   delay(300);
}
void setup(){
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(wifi_ssid, wifi_pwd);

  int ConnectionTime = 0;
  
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    ConnectionTime++;
    Serial.println("Connecting to WiFi....");

    if(ConnectionTime > 500){
      ESP.restart();
      }
  }
  Serial.println("Connected to WiFi");

  EEPROM.begin(8);
  
  pinMode(2, INPUT);
  Serial.println("Setup done");
}

void loop(){

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Wifi disconnected");
    Serial.println("Reconnecting...");  
    ESP.restart();
      
  }

  int outputValue;
  outputValue = EEPROM.readInt(0);
  
  //Serial.println(outputValue);
  String sensor_string = String(outputValue);
  sensor_string = "\"" + sensor_string + "\"";
  createCI(server, "Team46_Water_pH_monitoring", "node_1", sensor_string);
  delay(5000);

  outputValue = analogRead(2);
  EEPROM.writeInt(0, outputValue);
  EEPROM.commit();
}