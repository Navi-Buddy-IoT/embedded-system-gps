#include "gps-sensor.hpp"
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setupSensor(){

  if (!mpu.begin()) {
    Serial.print("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}


float measureLatitude(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  return a.acceleration.x;  // Example: Use accelerometer x value as latitude
}

float measureLongitude(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  return a.acceleration.y;

}