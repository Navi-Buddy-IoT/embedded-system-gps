#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include "math.h"
#include "time.h"
#include "gps-sensor.hpp"
#include "wifi-setup.hpp"
#include "http-client.hpp"

#define DEVICE_ID 1
String authToken;

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

void setup() {
   Serial.begin(115200);
  LCD.init();
  LCD.backlight();
  LCD.setCursor(0,0);
  LCD.print("Starting...");

  setupWiFi();
  authToken = login();
  Serial.print(authToken);
  setupSensor();
  LCD.clear();
}

void loop() {
  float latitudeInit = measureLatitude();
  float longitudeInit = measureLongitude();
  float latitudeStatic = -12.025895;
  float longitudeStatic = -77.058990;
  float latitude = (latitudeInit + 3 * 9.81)/1000 + latitudeStatic;
  float longitude = (longitudeInit + 3 * 9.81)/1000 + longitudeStatic;
  float latitudeShow = round(latitude * 100) /100;
  float longitudeShow = round(longitude * 100) /100;
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Lat: ");
  LCD.print(latitudeShow);
  LCD.setCursor(0, 1);
  LCD.print("Long: ");
  LCD.print(longitudeShow);

  
  String requestBody = String("{\"latitude\":") + latitude + ",\"longitude\":" + longitude + ",\"bicycleId\":" + DEVICE_ID + "}";
  Serial.print(requestBody);

  int responseCode = sendUpdateRequest(requestBody.c_str(), authToken.c_str(), DEVICE_ID);
  checkResponseCode(responseCode);
   
  delay(1000);
}
