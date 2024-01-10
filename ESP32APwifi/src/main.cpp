#include <Arduino.h>
#include <WiFi.h>
#include "apwifiesp32.h"


void setup() {
    Serial.begin(115200);
    EEPROM.begin(512);
    //escribirStringEnEEPROM(" ",50);
    //escribirStringEnEEPROM(" ",100);
    //escribirStringEnEEPROM(" ",150);
    //escribirStringEnEEPROM(" ",200);
    //escribirStringEnEEPROM(" ",250);
    //escribirStringEnEEPROM(" ",300);
    initAP("ESP32 - Access Point","123456789");
}


void loop() {

  loopAP();

}
