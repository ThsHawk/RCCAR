/*
 * @file: ServoJoystick.ino
 * @date: July 02 2022
 * @author: Thales Alves
 * @brief: Control servo through web joystick
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

#include <Arduino.h>
#include <WebSocketsServer.h>
#include <Hash.h>

#include <String.h>
#include <Servo.h>

Servo servo;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void control(char * data){

  int nA = strcspn(data, "/");
  char *pAngle = data;
  pAngle[nA] = '\0';
  char *pThrust = &data[nA + 1];
  int nT = strcspn(pThrust, "/");
  pThrust[nT] = '\0';
  char *pLights = &pThrust[nT + 1];

  int angle = atoi(pAngle);
  int thrust = atoi(pThrust);
  bool lights = atoi(pLights);
  
  servo.write(angle);

  Serial.printf("%d", angle);
  Serial.printf("|");

  Serial.printf("%d", thrust);
  Serial.printf("|");
  
  Serial.printf("%d\n", lights);

}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  if (type == WStype_TEXT){
    control((char *)payload);
  }
  
}//webSocketEvent

bool handleFileRead(String path){
  Serial.println("handleFileRead: " + path);
  if(path.endsWith("/")) path += "joystick.html";

  if( SPIFFS.exists(path)){
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, "text/html");
    file.close();
    return true;
  }
  return false;
}

const char *ssid = "Esp_AP";
const char *password = "senha_dificil";

void setup(void){

  servo.attach(D4, 500, 2400); //D4
  
  Serial.begin(115200);
  Serial.print("\n");
  Serial.setDebugOutput(true);

  SPIFFS.begin();

  Serial.print("Configuring access point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started.");

  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
  webSocket.loop();
}
