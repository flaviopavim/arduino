//
//    FILE: GY521.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.3.8
// PURPOSE: Arduino library for I2C GY521 accelerometer-gyroscope sensor
//     URL: https://github.com/RobTillaart/GY521


#include "GY521.h"

//  keep register names in sync with BIG MPU6050 lib
#include "GY521_registers.h"


//  COMMANDS
#define GY521_WAKEUP                 0x00



/////////////////////////////////////////////////////
//
// PUBLIC
//
GY521::GY521(uint8_t address, TwoWire *wire)
{
  _address = address;
  _wire    = wire;
  reset();
}


#if defined (ESP8266) || defined(ESP32)
bool GY521::begin(uint8_t sda, uint8_t scl)
{
  _wire->begin(sda, scl);
  if (isConnected())
  {
    return wakeup();
  }
  return false;
}
#endif


bool GY521::begin()
{
  _wire->begin();
  if (isConnected())
  {
    return wakeup();
  }
  return false;
}


bool GY521::isConnected()
{
  _wire->beginTransmission(_address);
  return (_wire->endTransmission() == 0);
}


void GY521::reset()
{
  setThrottleTime(GY521_THROTTLE_TIME);

  _ax  = _ay  = _az  = 0;
  _aax = _aay = _aaz = 0;
  _gx  = _gy  = _gz  = 0;
  _pitch = 0;
  _roll  = 0;
  _yaw   = 0;
}


bool GY521::wakeup()
{
  _wire->beginTransmission(_address);
  _wire->write(GY521_PWR_MGMT_1);
  _wire->write(GY521_WAKEUP);
  return (_wire->endTransmission() == 0);
}


int16_t GY521::read()
{
  uint32_t now = millis();
  if (_throttle)
  {
    if ((now - _lastTime) < _throttleTime)
    {
      //  not an error.
      return GY521_THROTTLED;
    }
  }
  _lastTime = now;

  //  Connected ?
  _wire->beginTransmission(_address);
  _wire->write(GY521_ACCEL_XOUT_H);
  if (_wire->endTransmission() != 0)
  {
    _error = GY521_ERROR_WRITE;
    return _error;
  }

  //  Get the data
  int8_t n = _wire->requestFrom(_address, (uint8_t)14);
  if (n != 14)
  {
    _error = GY521_ERROR_READ;
    return _error;
  }
  //  ACCELEROMETER
  _ax = _WireRead2();           //  ACCEL_XOUT_H  ACCEL_XOUT_L
  _ay = _WireRead2();           //  ACCEL_YOUT_H  ACCEL_YOUT_L
  _az = _WireRead2();           //  ACCEL_ZOUT_H  ACCEL_ZOUT_L
  //  TEMPERATURE
  _temperature = _WireRead2();  //  TEMP_OUT_H    TEMP_OUT_L
  //  GYROSCOPE
  _gx = _WireRead2();           //  GYRO_XOUT_H   GYRO_XOUT_L
  _gy = _WireRead2();           //  GYRO_YOUT_H   GYRO_YOUT_L
  _gz = _WireRead2();           //  GYRO_ZOUT_H   GYRO_ZOUT_L

  //  duration interval
  now = micros();
  float duration = (now - _lastMicros) * 1e-6;  // duration in seconds.
  _lastMicros = now;


  //  next lines might be merged per axis.

  //  Convert raw acceleration to g's
  _ax *= _raw2g;
  _ay *= _raw2g;
  _az *= _raw2g;

  //  Error correct raw acceleration (in g) measurements  // #18 kudos to Merkxic
  _ax += axe;
  _ay += aye;
  _az += aze;

  //  prepare for Pitch Roll Yaw
  float _ax2 = _ax * _ax;
  float _ay2 = _ay * _ay;
  float _az2 = _az * _az;

  _aax = atan(       _ay / sqrt(_ax2 + _az2)) * GY521_RAD2DEGREES;
  _aay = atan(-1.0 * _ax / sqrt(_ay2 + _az2)) * GY521_RAD2DEGREES;
  _aaz = atan(       _az / sqrt(_ax2 + _ay2)) * GY521_RAD2DEGREES;
  //  optimize #22
  //  _aax = atan(_ay / hypot(_ax, _az)) * GY521_RAD2DEGREES;
  //  _aay = atan(-1.0 * _ax / hypot(_ay, _az)) * GY521_RAD2DEGREES;
  //  _aaz = atan(_az / hypot(_ax, _ay)) * GY521_RAD2DEGREES;

  //  Convert to Celsius
  _temperature = _temperature * 0.00294117647 + 36.53;  //  == /340.0  + 36.53;

  //  Convert raw Gyro to degrees/seconds
  _gx *= _raw2dps;
  _gy *= _raw2dps;
  _gz *= _raw2dps;

  //  Error correct raw gyro measurements.
  _gx += gxe;
  _gy += gye;
  _gz += gze;

  _gax += _gx * duration;
  _gay += _gy * duration;
  _gaz += _gz * duration;

  //  normalize internal vars
  if (_gax < 0) _gax += 360.0;
  else if (_gax >= 360.0) _gax -= 360.0;
  if (_gay < 0) _gay += 360.0;
  else if (_gay >= 360.0) _gay -= 360.0;
  if (_gaz < 0) _gaz += 360.0;
  else if (_gaz >= 360.0) _gaz -= 360.0;

  _yaw   = _gaz;
  _pitch = 0.96 * _gay + 0.04 * _aay;
  _roll  = 0.96 * _gax + 0.04 * _aax;

  //  normalize external vars.
  if (_pitch < 0) _pitch += 360;
  else if (_pitch >= 360) _pitch -= 360;
  if (_roll  < 0) _roll  += 360;
  else if (_roll  >= 360) _roll  -= 360;

  return GY521_OK;
}


int16_t GY521::readAccel()
{
  uint32_t now = millis();
  if (_throttle)
  {
    if ((now - _lastTime) < _throttleTime)
    {
      // not an error.
      return GY521_THROTTLED;
    }
  }
  _lastTime = now;

  //  Connected ?
  _wire->beginTransmission(_address);
  _wire->write(GY521_ACCEL_XOUT_H);
  if (_wire->endTransmission() != 0)
  {
    _error = GY521_ERROR_WRITE;
    return _error;
  }

  // Get the data
  int8_t n = _wire->requestFrom(_address, (uint8_t)6);
  if (n != 6)
  {
    _error = GY521_ERROR_READ;
    return _error;
  }
  //  ACCELEROMETER
  _ax = _WireRead2();  //  ACCEL_XOUT_H  ACCEL_XOUT_L
  _ay = _WireRead2();  //  ACCEL_YOUT_H  ACCEL_YOUT_L
  _az = _WireRead2();  //  ACCEL_ZOUT_H  ACCEL_ZOUT_L

  //  next lines might be merged per axis.

  //  Convert raw acceleration to g's
  _ax *= _raw2g;
  _ay *= _raw2g;
  _az *= _raw2g;

  //  Error correct raw acceleration (in g) measurements  // #18 kudos to Merkxic
  _ax += axe;
  _ay += aye;
  _az += aze;

  //  prepare for Pitch Roll Yaw
  float _ax2 = _ax * _ax;
  float _ay2 = _ay * _ay;
  float _az2 = _az * _az;

  _aax = atan(       _ay / sqrt(_ax2 + _az2)) * GY521_RAD2DEGREES;
  _aay = atan(-1.0 * _ax / sqrt(_ay2 + _az2)) * GY521_RAD2DEGREES;
  _aaz = atan(       _az / sqrt(_ax2 + _ay2)) * GY521_RAD2DEGREES;
  //  optimize #22
  //  _aax = atan(_ay / hypot(_ax, _az)) * GY521_RAD2DEGREES;
  //  _aay = atan(-1.0 * _ax / hypot(_ay, _az)) * GY521_RAD2DEGREES;
  //  _aaz = atan(_az / hypot(_ax, _ay)) * GY521_RAD2DEGREES;

  return GY521_OK;
}


int16_t GY521::readGyro()
{
  uint32_t now = millis();
  if (_throttle)
  {
    if ((now - _lastTime) < _throttleTime)
    {
      // not an error.
      return GY521_THROTTLED;
    }
  }
  _lastTime = now;

  //  Connected ?
  _wire->beginTransmission(_address);
  _wire->write(GY521_GYRO_XOUT_H);
  if (_wire->endTransmission() != 0)
  {
    _error = GY521_ERROR_WRITE;
    return _error;
  }

  //  Get the data
  int8_t n = _wire->requestFrom(_address, (uint8_t)6);
  if (n != 6)
  {
    _error = GY521_ERROR_READ;
    return _error;
  }
  //  GYROSCOPE
  _gx = _WireRead2();     //  GYRO_XOUT_H   GYRO_XOUT_L
  _gy = _WireRead2();     //  GYRO_YOUT_H   GYRO_YOUT_L
  _gz = _WireRead2();     //  GYRO_ZOUT_H   GYRO_ZOUT_L

  //  duration interval
  now = micros();
  float duration = (now - _lastMicros) * 1e-6;  //  duration in seconds.
  _lastMicros = now;

  //  next lines might be merged per axis.

  //  Convert raw Gyro to degrees/seconds
  _gx *= _raw2dps;
  _gy *= _raw2dps;
  _gz *= _raw2dps;

  //  Error correct raw gyro measurements.
  _gx += gxe;
  _gy += gye;
  _gz += gze;

  _gax += _gx * duration;
  _gay += _gy * duration;
  _gaz += _gz * duration;

  //  normalize internal vars
  if (_gax < 0) _gax += 360.0;
  else if (_gax >= 360.0) _gax -= 360.0;
  if (_gay < 0) _gay += 360.0;
  else if (_gay >= 360.0) _gay -= 360.0;
  if (_gaz < 0) _gaz += 360.0;
  else if (_gaz >= 360.0) _gaz -= 360.0;

  return GY521_OK;
}


int16_t GY521::readTemperature()
{
  //  DO NOT THROTTLE
  _wire->beginTransmission(_address);
  _wire->write(GY521_TEMP_OUT_H);
  if (_wire->endTransmission() != 0)
  {
    _error = GY521_ERROR_WRITE;
    return _error;
  }

  //  Get the data
  int8_t n = _wire->requestFrom(_address, (uint8_t)2);
  if (n != 2)
  {
    _error = GY521_ERROR_READ;
    return _error;
  }
  //  TEMPERATURE
  _temperature = _WireRead2();    //  TEMP_OUT_H    TEMP_OUT_L
  return GY521_OK;
}


bool GY521::setAccelSensitivity(uint8_t as)
{
  _afs = as;
  if (_afs > 3) _afs = 3;
  uint8_t val = getRegister(GY521_ACCEL_CONFIG);
  if (_error != 0)
  {
    return false;
  }
  // no need to write same value
  if (((val >> 3) & 3) != _afs)
  {
    val &= 0xE7;
    val |= (_afs << 3);
    if (setRegister(GY521_ACCEL_CONFIG, val) != GY521_OK)
    {
      return false;
    }
  }
  // calculate conversion factor.  // 4 possible values => lookup table?
  _raw2g = (1 << _afs) * GY521_RAW2G;
  return true;
}


uint8_t GY521::getAccelSensitivity()
{
  uint8_t val = getRegister(GY521_ACCEL_CONFIG);
  if (_error != GY521_OK)
  {
    return _error; // return and propagate error (best thing to do)
  }
  _afs = (val >> 3) & 3;
  return _afs;
}


bool GY521::setGyroSensitivity(uint8_t gs)
{
  _gfs = gs;
  if (_gfs > 3) _gfs = 3;
  uint8_t val = getRegister(GY521_GYRO_CONFIG);
  if (_error != 0)
  {
    return false;
  }
  //  no need to write same value
  if (((val >> 3) & 3) != _gfs)
  {
    val &= 0xE7;
    val |= (_gfs << 3);
    if (setRegister(GY521_GYRO_CONFIG, val) != GY521_OK)
    {
      return false;
    }
  }
  //  calculate conversion factor..  
  //  4 possible values => lookup table?
  _raw2dps = (1 << _gfs) * GY521_RAW2DPS;
  return true;
}


uint8_t GY521::getGyroSensitivity()
{
  uint8_t val = getRegister(GY521_GYRO_CONFIG);
  if (_error != GY521_OK)
  {
    return _error; //  return and propagate error (best thing to do)
  }
  _gfs = (val >> 3) & 3;
  return _gfs;
}


uint8_t GY521::setRegister(uint8_t reg, uint8_t value)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  _wire->write(value);
  //  no need to do anything if not connected.
  if (_wire->endTransmission() != 0)
  {
    _error = GY521_ERROR_WRITE;
    return _error;
  }
  return GY521_OK;
}


uint8_t GY521::getRegister(uint8_t reg)
{
  _wire->beginTransmission(_address);
  _wire->write(reg);
  if (_wire->endTransmission() != 0)
  {
    _error = GY521_ERROR_WRITE;
    return _error;
  }
  uint8_t n = _wire->requestFrom(_address, (uint8_t) 1);
  if (n != 1)
  {
    _error = GY521_ERROR_READ;
    return _error;
  }
  uint8_t val = _wire->read();
  return val;
}


//  to read register of 2 bytes.
int16_t GY521::_WireRead2()
{
  int16_t tmp = _wire->read();
  tmp <<= 8;
  tmp |= _wire->read();
  return tmp;
}


// -- END OF FILE --

