//CODIGO PARA ARAÑA LIDER COM6
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
//#include <WebServer.h>
#include "ESPAsyncWebServer.h"
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>
#include <iostream>
#include <string>
#include <cstdlib>


Servo servoL;
Servo servoR;
int pinServoL=15;
int pinServoR=2;
float angleRoll;
float anglePitch;
String rollRequest;
String pitchRequest;

AsyncWebServer server(80);

const char* ssid = "ESP32 - Access Point";
const char* password = "123456789";

const char* ssid2 = "Lider Access Point";
const char* password2 = "123456789";

//Your IP address or domain name with URL path
const char* serverNameRoll = "http://192.168.4.1/obtenerRoll";
const char* serverNamePitch = "http://192.168.4.1/obtenerPitch";

//direcciones macAdress de las 2 arañas seguidoras
uint8_t receiverAddress2[]={0x44, 0x17, 0x93, 0xF9, 0x34, 0x10};
uint8_t receiverAddress3[]={0xE8, 0x31, 0xCD, 0xD7, 0x1F, 0x60};

String sentRollAngle(){
  String response = String(angleRoll);
  return response;
  //server.send(200,"text/plain",response);
}

String sentPitchAngle(){
  String response = String(anglePitch);
  return response;
  //server.send(200,"text/plain", response);
}


void girarDerecha(){
  servoR.writeMicroseconds(1500);
  servoL.writeMicroseconds(1700); // 1.7 ms -> counterclockwis
 // Serial.print(" Pitch Angle [°] ");
 // Serial.print(KalmanAnglePitch);
  Serial.println(" La araña gira hacia la derecha.....");
}

void girarIzquierda(){
  servoL.writeMicroseconds(1500);
  servoR.writeMicroseconds(1300);
 // Serial.print(" Pitch Angle [°] ");
 // Serial.print(KalmanAnglePitch);
  Serial.println(" La araña gira hacia la izquierda.....");
}

void atras(){
  servoR.writeMicroseconds(1700); // 1.7 ms -> counterclockwise
  servoL.writeMicroseconds(1300); // 1.3 ms -> clockwise
 // Serial.print("Roll Angle [°] ");
//  Serial.print(KalmanAngleRoll);
  Serial.println(" La araña se mueve hacia atrás.....");
}

void adelante(){
  servoR.writeMicroseconds(1300); // 1.7 ms -> clockwise
  servoL.writeMicroseconds(1700); // 1.3 ms -> counterclockwise
 // Serial.print("Roll Angle [°] ");
 // Serial.print(KalmanAngleRoll);
  Serial.println(" La araña se mueve hacia adelante.....");
}

void stop(){
  servoR.writeMicroseconds(1500);
  servoL.writeMicroseconds(1500);
 // Serial.print("Roll Angle [°] ");
 // Serial.print(KalmanAngleRoll);
 // Serial.print(" Pitch Angle [°] ");
 // Serial.println(KalmanAnglePitch);
  Serial.println(" La araña no se mueve.....");
}

String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid2, password2);

  Serial.print("Leader AP Ip address:");
  Serial.println(WiFi.softAPIP());

  server.on("/rollLider",HTTP_GET, [] (AsyncWebServerRequest *request){
    request->send_P(200, "text/plain",sentRollAngle().c_str());
  });
  server.on("/pitchLider",HTTP_GET,[](AsyncWebServerRequest *request){
    request->send_P(200,"text/plain",sentPitchAngle().c_str());
  });

  server.begin();

  WiFi.begin(ssid, password);
  Serial.println("Connecting to the glove AP network...");
  int cnt=0;
  while(WiFi.status() != WL_CONNECTED and cnt<8) { 
    delay(1000);
    Serial.print(".");
    cnt++;
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  servoR.attach(pinServoR);
  servoL.attach(pinServoL);
 // servoR.writeMicroseconds(1500); // 1500 -> stay-still
 // servoL.writeMicroseconds(1500);
}


void loop() {
  if(WiFi.status()== WL_CONNECTED ){
    rollRequest= httpGETRequest(serverNameRoll);
    pitchRequest = httpGETRequest(serverNamePitch);

    angleRoll = strtof(rollRequest.c_str(), nullptr);
    std::cout << angleRoll << std::endl;


    anglePitch = strtof(pitchRequest.c_str(), nullptr);
    std::cout << anglePitch << std::endl;

    Serial.print("Roll angle received: ");
    Serial.println(angleRoll);
    Serial.print("Pitch angle received: ");
    Serial.println(anglePitch);
  }

  else {
      Serial.println("WiFi Disconnected");
    }
  
  if((angleRoll>10)&(angleRoll<80)){
    adelante();
  }
  else if((angleRoll<-10)&(angleRoll>-80)){
    atras();
  }
  else if((anglePitch>10)&(anglePitch<80)){
    girarIzquierda();
  }
  else if((anglePitch<-10)&(anglePitch>-80)){
    girarDerecha();
  }

  else{
    stop();
  }

  //delay(50);
}

