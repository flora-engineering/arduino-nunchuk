/*
 * ArduinoNunchuk.cpp - Improved Wii Nunchuk library for Arduino
 *
 * Copyright 2011-2013 Gabriel Bianconi, http://www.gabrielbianconi.com/
 *
 * Project URL: http://www.gabrielbianconi.com/projects/arduinonunchuk/
 *
 * Based on the following resources:
 *   http://www.windmeadow.com/node/42
 *   http://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/
 *   http://wiibrew.org/wiki/Wiimote/Extension_Controllers
 *
 */

#include <Arduino.h>
#include <Wire.h>
#include "ArduinoNunchuk.h"

#define ADDRESS 0x52

void ArduinoNunchuk::init()
{
  Wire.begin();

  ArduinoNunchuk::_sendByte(0x55, 0xF0);
  ArduinoNunchuk::_sendByte(0x00, 0xFB);

  ArduinoNunchuk::update();
}

void ArduinoNunchuk::update()
{
  int count = 0;
  int values[6];

  Wire.requestFrom(ADDRESS, 6);

  while(Wire.available())
  {
    values[count] = Wire.read();
    count++;
  }

  ArduinoNunchuk::analogX = values[0];
  ArduinoNunchuk::analogY = values[1];
  ArduinoNunchuk::accelX = (values[2] << 2) | ((values[5] >> 2) & 3);
  ArduinoNunchuk::accelY = (values[3] << 2) | ((values[5] >> 4) & 3);
  ArduinoNunchuk::accelZ = (values[4] << 2) | ((values[5] >> 6) & 3);
  ArduinoNunchuk::zButton = !((values[5] >> 0) & 1);
  ArduinoNunchuk::cButton = !((values[5] >> 1) & 1);

  // refine neutral and extremes
  // if((ArduinoNunchuk::analogX < 135) && (ArduinoNunchuk::analogX > 120)) {
  //   ArduinoNunchuk::analogX = 127;
  // } else if(ArduinoNunchuk::analogX < 8) {
  //   ArduinoNunchuk::analogX = 0;
  // } else if(ArduinoNunchuk::analogX > 248) {
  //   ArduinoNunchuk::analogX = 255;
  // }
  //
  // if((ArduinoNunchuk::analogY < 135) && (ArduinoNunchuk::analogY > 120)) {
  //   ArduinoNunchuk::analogY = 127;
  // } else if(ArduinoNunchuk::analogY < 8) {
  //   ArduinoNunchuk::analogY = 0;
  // } else if(ArduinoNunchuk::analogY > 248) {
  //   ArduinoNunchuk::analogY = 255;
  // }

  if(_invert_FLAG){
    ArduinoNunchuk::analogX = 255 - ArduinoNunchuk::analogX;
    ArduinoNunchuk::analogY = 255 - ArduinoNunchuk::analogY;
  }

  ArduinoNunchuk::_sendByte(0x00, 0x00);
}

void ArduinoNunchuk::_sendByte(byte data, byte location)
{
  Wire.beginTransmission(ADDRESS);

  Wire.write(location);
  Wire.write(data);

  Wire.endTransmission();

  // delay(10);
}

void ArduinoNunchuk::invert(bool selection) {
  _invert_FLAG = selection;
}

bool ArduinoNunchuk::invertStatus() {
  return _invert_FLAG;
}
