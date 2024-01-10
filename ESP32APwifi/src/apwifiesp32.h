#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <WiFiClientSecure.h>
//#include <ESPAsyncWebServer.h>
#include <TimeLib.h>
#include "index.h"
#include "angles.h"

WebServer server(80);
//AsyncWebServer asyncServer(80);
float data1;
float data2;
String texto="";
String texto_send="";

unsigned long previousMillis = 0;
const long interval = 60000;

String leerStringDeEEPROM(int direccion) {
  String cadena = "";
  char caracter = EEPROM.read(direccion);
  int i = 0;
  while (caracter != '\0' && i < 100) {
    cadena += caracter;
    i++;
    caracter = EEPROM.read(direccion + i);
  }
  return cadena;
}

void escribirStringEnEEPROM(String cadena, int direccion) {
  int longitudCadena = cadena.length();
  for (int i = 0; i < longitudCadena; i++) {
    EEPROM.write(direccion + i, cadena[i]);
  }
  EEPROM.write(direccion + longitudCadena, '\0'); // Null-terminated string
  EEPROM.commit(); // Guardamos los cambios en la memoria EEPROM
}

void handleConexionAutomatica(){
    
    if(leerStringDeEEPROM(50) != " " and leerStringDeEEPROM(100) != " "){
      /*Serial.println("Las credenciales 1 guardadas en la EEPROM son:");
      Serial.println("Red Wi-Fi:"+leerStringDeEEPROM(50));
      Serial.println("Clave Wi-Fi:"+leerStringDeEEPROM(100));
      Serial.println("Se está empezando la conexion automática 1...");*/
      WiFi.disconnect(); // Desconectar la red Wi-Fi anterior, si se estaba conectado
      WiFi.begin(leerStringDeEEPROM(50).c_str(), leerStringDeEEPROM(100).c_str(),6);
      int cnt = 0;
      while (WiFi.status() != WL_CONNECTED and cnt < 8) {
        delay(1000);
        Serial.print(".");
        cnt++;
      }
      if (WiFi.status() == WL_CONNECTED)
      {
      
      String response = "Conexión automática establecida;"+leerStringDeEEPROM(50)+";"+WiFi.localIP().toString();
      server.send(200, "text/plain", response);
      /*Serial.println("Conexión automática 1 establecida");
      Serial.print("Ip asignada: ");
      Serial.println(WiFi.localIP().toString());*/
      /*WiFi.mode(WIFI_STA);
      Serial.println("Se cambió a modo Wifi Station");*/
      
      }
      else{
      //Serial.println("Conexión automática 1 no establecida");
      }
    }
    if (leerStringDeEEPROM(150) != " " and leerStringDeEEPROM(200) != " " and WiFi.status() != WL_CONNECTED){
      /*Serial.println("Las credenciales guardadas 2 en la EEPROM son:");
      Serial.println("Red Wi-Fi:"+leerStringDeEEPROM(150));
      Serial.println("Clave Wi-Fi:"+leerStringDeEEPROM(200));
      Serial.println("Se está empezando la conexion automática 2...");*/
      WiFi.disconnect(); // Desconectar la red Wi-Fi anterior, si se estaba conectado
      WiFi.begin(leerStringDeEEPROM(150).c_str(), leerStringDeEEPROM(200).c_str(),6);
      int cnt = 0;
      while (WiFi.status() != WL_CONNECTED and cnt < 8) {
        delay(1000);
        Serial.print(".");
        cnt++;
      }
      if (WiFi.status() == WL_CONNECTED)
      {
      
      String response = "Conexión automática establecida;"+leerStringDeEEPROM(150)+";"+WiFi.localIP().toString();
      server.send(200, "text/plain", response);
      /*Serial.println("Conexión automática 2 establecida");
      Serial.print("Ip asignada: ");
      Serial.println(WiFi.localIP().toString());*/
      /*WiFi.mode(WIFI_STA);
      Serial.println("Se cambió a modo Wifi Station");*/
      
      }
      else{
      //Serial.println("Conexión automática 2 no establecida");
      }
    }

    if(leerStringDeEEPROM(250) != " " and leerStringDeEEPROM(300) != " " and WiFi.status() != WL_CONNECTED){
      /*Serial.println("Las credenciales guardadas 3 en la EEPROM son:");
      Serial.println("Red Wi-Fi:"+leerStringDeEEPROM(250));
      Serial.println("Clave Wi-Fi:"+leerStringDeEEPROM(300));
      Serial.println("Se está empezando la conexion automática 3...");*/
      WiFi.disconnect(); // Desconectar la red Wi-Fi anterior, si se estaba conectado
      WiFi.begin(leerStringDeEEPROM(250).c_str(), leerStringDeEEPROM(300).c_str(),6);
      int cnt = 0;
      while (WiFi.status() != WL_CONNECTED and cnt < 8) {
        delay(1000);
        Serial.print(".");
        cnt++;
      }
      if (WiFi.status() == WL_CONNECTED)
      {
      String response = "Conexión automática establecida;"+leerStringDeEEPROM(250)+";"+WiFi.localIP().toString();
      server.send(200, "text/plain", response);
      /*Serial.println("Conexión automática 3 establecida");
      Serial.print("Ip asignada: ");
      Serial.println(WiFi.localIP().toString());*/
      /*WiFi.mode(WIFI_STA);
      Serial.println("Se cambió a modo Wifi Station");*/
      
      }
      else{
      //Serial.println("Conexión automática 3 no establecida");
      }
    }
    else{
      server.send(200, "text/plain", "No hay credenciales guardadas");
      //Serial.println("No hay credenciales guardadas para la conexión automática");
    }
}

void handleRedes(){

  int numRedes;
  int espacio = 50;
  String redes="obteniendo redes";
  for(numRedes=3;numRedes>0;numRedes--){
    redes+=";";
    redes+=leerStringDeEEPROM(espacio);
    espacio+=100;
  }
  server.send(200, "text/plain", redes);
  //Serial.println("Eviando redes: "+redes+" a servidor web");
}

void handleRoot() {

  if(WiFi.status() == WL_CONNECTED){
    String htmlweb = MAIN_page;
    server.send(200, "text/html", htmlweb);
  }

  else{
    String html = "<html><body>";

  html += "<form method='GET' action='/conexionAutomatica'>";
  html += "<input type='submit' value='Conexión Automática'>";
  html += "</form>";

  html += "<form method='GET' action='/obtenerRedes'>";
  html += "<input type='submit' value='Ir a redes'>";
  html += "</form>";

  html += "<form method='GET' action='/obtenerDatos'>";
  html += "<input type='submit' value='Ir a datos'>";
  html += "</form>";

  html += "<form method='POST' action='/wifi'>";
  html += "Red Wi-Fi: <input type='text' name='ssid'><br>";
  html += "Contraseña: <input type='password' name='password'><br>";
  html += "<input type='submit' value='Conectar'>";
  html += "</form></body></html>";
  server.send(200, "text/html", html);
  }
}

void handleWifi() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    /*Serial.print("Conectando a la red Wi-Fi ");
    Serial.println(ssid);
    Serial.print("Clave Wi-Fi ");
    Serial.println(password);*/
    WiFi.disconnect(); // Desconectar la red Wi-Fi anterior, si se estaba conectado
    WiFi.begin(ssid.c_str(), password.c_str(),6);
    int cnt = 0;
    while (WiFi.status() != WL_CONNECTED and cnt < 8) {
      delay(1000);
      Serial.print(".");
      cnt++;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("Conexión manual establecida");
      if(leerStringDeEEPROM(50)==" " and leerStringDeEEPROM(100)==" "){
        escribirStringEnEEPROM(ssid,50);
        escribirStringEnEEPROM(password,100);
        String respuesta = "Conexión manual establecida;"+leerStringDeEEPROM(50)+";"+WiFi.localIP().toString();
        server.send(200, "text/plain", respuesta);
        Serial.println(respuesta);
        Serial.println("Conexión manual establecida 50");
       /*WiFi.mode(WIFI_STA);
        Serial.println("Se cambió a modo Wifi Station");*/
      }
      else if (leerStringDeEEPROM(150)==" " and leerStringDeEEPROM(200)==" " and leerStringDeEEPROM(50)!=ssid and leerStringDeEEPROM(100) != password){
        escribirStringEnEEPROM(ssid,150);
        escribirStringEnEEPROM(password,200);
        String respuesta = "Conexión manual establecida;"+leerStringDeEEPROM(150)+";"+WiFi.localIP().toString();
        server.send(200, "text/plain", respuesta);
        Serial.println(respuesta);
        Serial.println("Conexión manual establecida 150");
        /*WiFi.mode(WIFI_STA);
        Serial.println("Se cambió a modo Wifi Station");*/
      }

      else if(leerStringDeEEPROM(250)==" " and leerStringDeEEPROM(300)==" " and leerStringDeEEPROM(50)!=ssid and leerStringDeEEPROM(100) != password and leerStringDeEEPROM(150)!=ssid and leerStringDeEEPROM(200) != password){
        escribirStringEnEEPROM(ssid,250);
        escribirStringEnEEPROM(password,300);
        String respuesta = "Conexión manual establecida;"+leerStringDeEEPROM(250)+";"+WiFi.localIP().toString();
        server.send(200, "text/plain", respuesta);
        Serial.println(respuesta);
        Serial.println("Conexión manual establecida 250");
        /*WiFi.mode(WIFI_STA);
        Serial.println("Se cambió a modo Wifi Station");*/
      }

      else{
        if(leerStringDeEEPROM(50)==ssid and leerStringDeEEPROM(100)==password){
          //Serial.println("Estas credenciales ya están guardadas");
          server.send(200, "text/plain", "Estas credenciales ya está guardadas");
        }
        else if(leerStringDeEEPROM(150)==ssid and leerStringDeEEPROM(200)==password){
          //Serial.println("Estas credenciales ya están guardadas");
          server.send(200, "text/plain", "Estas credenciales ya está guardadas");
        }
        else if(leerStringDeEEPROM(250)==ssid and leerStringDeEEPROM(300)==password){
          //Serial.println("Estas credenciales ya están guardadas");
          server.send(200, "text/plain", "Estas credenciales ya está guardadas");
        }
        else{
          //Serial.println("Ya no hay espacio para más credenciales");
          server.send(200, "text/plain", "Ya no hay espacio para más credenciales");
        } 
      } 
    }
    else{
      //Serial.println("Conexión manual no establecida");
      server.send(200, "text/plain", "Conexión manual no establecida");
    }
}

void handleData(){

  String response;
  texto_send=texto;
  response = texto_send;


  server.send(200, "text/plain",response);
}

void readRollAngle(){
  String response = String(rollAngle);
  server.send(200,"text/plain",response);
}

void readPitchAngle(){
  String response = String(pitchAngle);
  server.send(200,"text/plain",response);
}

void initAP(const char* apSsid,const char* apPassword) { // Nombre de la red Wi-Fi y  Contraseña creada por el ESP32
  
  Serial.begin(115200);
  EEPROM.begin(512);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSsid, apPassword);

  server.on("/", handleRoot);
  server.on("/conexionAutomatica", handleConexionAutomatica);
  server.on("/obtenerRedes", HTTP_GET, handleRedes);
  server.on("/wifi", handleWifi);
  server.on("/obtenerDatos", HTTP_GET, handleData);
  server.on("/obtenerRoll", HTTP_GET, readRollAngle);
  server.on("/obtenerPitch", HTTP_GET, readPitchAngle);
  /*asyncServer.on("/rollAngle",HTTP_GET,[](AsyncWebServerRequest *request){
    request->send_P(200,"text/plain",readRollAngle().c_str());
  });
  asyncServer.on("/pitchAngle",HTTP_GET,[](AsyncWebServerRequest *request){
    request>send_P(200,"text/plain",readPitchAngle().c_str());
  });*/
  server.begin();
  //asyncServer.begin();

  
  Serial.println("Las credenciales 1 guardadas en la EEPROM son:");
  Serial.println("SSID:"+leerStringDeEEPROM(50));
  Serial.println("Contrasena:"+leerStringDeEEPROM(100));
  Serial.println("Las credenciales 2 guardadas en la EEPROM son:");
  Serial.println("SSID:"+leerStringDeEEPROM(150));
  Serial.println("Contrasena:"+leerStringDeEEPROM(200));
  Serial.println("Las credenciales 3 guardadas en la EEPROM son:");
  Serial.println("SSID:"+leerStringDeEEPROM(250));
  Serial.println("Contrasena:"+leerStringDeEEPROM(300));
  Serial.println("Ip de esp32...");
  Serial.println(WiFi.softAPIP());
  Serial.println("Servidor web iniciado");

  initAngles();

}

void loopAP() {
  loopAngles();
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis >= interval){
    previousMillis = currentMillis;
  }

  server.handleClient();
  texto="";
  data1=rollAngle;
  data2=pitchAngle;
  if(texto.length()==0){
  texto += data1;
  texto +=";";
  texto += data2;
  texto += ";";
  texto += String(previousMillis);
  //Serial.println(texto);
  //delay(15);
  }
}