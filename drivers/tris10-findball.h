/*!@addtogroup Other
 * @{
 * @defgroup T10FB FindBall
 * Tris10 FindBall
 * @{
 */

/*
 * $Id: trs10-findball-v2.h 133 2013-03-10 15:15:38Z xander $
 */

#ifndef __T10FB_H__
#define __T10FB_H__
/** \file trs10-findball-v2.h
 * \brief Tris10 FindBall driver
 *
 * trs10-findball.h provides an API for the Tris10 FindBall.
 *
 * Changelog:
 * - 0.1: Initial release
 *
 * Credits:
 * - Big thanks to Tris10 for providing me with the hardware necessary to write and test this.
 *
 * License: You may use this code as you wish, provided you give credit where its due.
 *
 * THIS CODE WILL ONLY WORK WITH ROBOTC VERSION 3.59 AND HIGHER.

 * \author Xander Soldaat (mightor_at_gmail.com)
 * \date  25 February 2014
 * \version 0.1
 * \example trs10-findball-test1.c
 */

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define T10FB_I2C_ADDR    0x02      /*!< IR Seeker I2C device address */
#define T10FB_OFFSET      0x42      /*!< Offset for data registers */
#define T10FB_DIR         0x00      /*!< Sensor number with best reading */
#define T10FB_STRENGTH    0x01      /*!< Signal strength on best sensor */
#define T10FB_STRENGTH_1  0x02      /*!< Sensor 1 signal strength above average */
#define T10FB_STRENGTH_2  0x03      /*!< Sensor 2 signal strength above average */
#define T10FB_STRENGTH_3  0x04      /*!< Sensor 3 signal strength above average */
#define T10FB_STRENGTH_4  0x05      /*!< Sensor 4 signal strength above average */
#define T10FB_STRENGTH_5  0x06      /*!< Sensor 5 signal strength above average */

int T10FBreadDir(tSensors link);
int T10FBreadStrength(tSensors link);

bool T10FBreadAllStrength(tSensors link, int *strength_1, int *strength_2, int *strength_3, int *strength_4, int *strength_5);

#ifdef __HTSMUX_SUPPORT__
int T10FBreadDir(tMUXSensor muxsensor);
int T10FBreadStrength(tMUXSensor muxsensor);
bool T10FBreadAllstrength_trength(tMUXSensor muxsensor, int &strength_1, int &strength_2, int &strength_3, int &strength_4, int &strength_5);
int T10FBreadDCAverage(tMUXSensor muxsensor);

tConfigParams T10FB_config = {HTSMUX_CHAN_I2C, 7, 0x02, 0x42}; /*!< Array to hold SMUX config data for sensor */
#endif // __HTSMUX_SUPPORT__

tByteArray T10FB_I2CRequest;    /*!< Array to hold I2C command data */
tByteArray T10FB_I2CReply;      /*!< Array to hold I2C reply data */


/**
 * Read the sensor number with best reading and return it.
 * @param link the T10FB port number
 * @return value of 0-5, the direction index of the detected IR signal, 0: no signal, -1: an error occurred.
 */
int T10FBreadDir(tSensors link) {
  memset(T10FB_I2CRequest, 0, sizeof(tByteArray));

  T10FB_I2CRequest[0] = 2;              // Message size
  T10FB_I2CRequest[1] = T10FB_I2C_ADDR; // I2C Address
  T10FB_I2CRequest[2] = T10FB_OFFSET + T10FB_DIR;  // Start direction register

  if (!writeI2C(link, T10FB_I2CRequest, T10FB_I2CReply, 1))
    return -1;

  return T10FB_I2CReply[0];
}


/**
 * Read the value of the DC Direction data register and return it.
 * @param link the T10FB port number
 * @return value of 0-9, the direction index of the detected IR signal or -1 if an error occurred.
 */
int T10FBreadStrength(tSensors link) {
  memset(T10FB_I2CRequest, 0, sizeof(tByteArray));

  T10FB_I2CRequest[0] = 2;              // Message size
  T10FB_I2CRequest[1] = T10FB_I2C_ADDR; // I2C Address
  T10FB_I2CRequest[2] = T10FB_OFFSET + T10FB_STRENGTH;  // Start direction register

  if (!writeI2C(link, T10FB_I2CRequest, T10FB_I2CReply, 1))
    return -1;

  return T10FB_I2CReply[0];
}


///**
// * Read the value of the DC Direction data register and return it.
// * @param muxsensor the SMUX sensor port number
// * @return value of 0-9, the direction index of the detected IR signal or -1 if an error occurred.
// */
//#ifdef __HTSMUX_SUPPORT__
//int T10FBreadDir(tMUXSensor muxsensor) {
//	memset(T10FB_I2CReply, 0, sizeof(tByteArray));

//  if (HTSMUXSensorTypes[muxsensor] != HTSMUXSensorCustom)
//    HTSMUXconfigChannel(muxsensor, T10FB_config);

//  if (!HTSMUXreadPort(muxsensor, T10FB_I2CReply, 1, T10FB_DC_DIR)) {
//    return -1;
//  }

//  return T10FB_I2CReply[0];
//}
//#endif // __HTSMUX_SUPPORT__


/**
 * Read the value of the all of the internal DC sensors above average.
 * @param link the T10FB port number
 * @param strength_1 data from internal sensor nr 1
 * @param strength_2 data from internal sensor nr 2
 * @param strength_3 data from internal sensor nr 3
 * @param strength_4 data from internal sensor nr 4
 * @param strength_5 data from internal sensor nr 5
 * @return true if no error occured, false if it did
 */
bool T10FBreadAllStrength(tSensors link, int *strength_1, int *strength_2, int *strength_3, int *strength_4, int *strength_5) {
  memset(T10FB_I2CRequest, 0, sizeof(tByteArray));

  int strengthVals[5];

  T10FB_I2CRequest[0] = 2;                      // Message size
  T10FB_I2CRequest[1] = T10FB_I2C_ADDR;         // I2C Address
  T10FB_I2CRequest[2] = T10FB_OFFSET + T10FB_STRENGTH_1;         // Sensor 0 signal strength

	for (int i = 0; i < 5; i++)
	{
		if (!writeI2C(link, T10FB_I2CRequest, T10FB_I2CReply, 1))
    	return false;
   	strengthVals[i] = T10FB_I2CReply[0];
   	T10FB_I2CRequest[2]++;
	}

  *strength_1 = strengthVals[0];
  *strength_2 = strengthVals[1];
  *strength_3 = strengthVals[2];
  *strength_4 = strengthVals[3];
  *strength_5 = strengthVals[4];

  return true;
}


///**
// * Read the value of the all of the internal DC sensors above average.
// * @param muxsensor the SMUX sensor port number
// * @param strength_1 data from internal sensor nr 1
// * @param strength_2 data from internal sensor nr 2
// * @param strength_3 data from internal sensor nr 3
// * @param strength_4 data from internal sensor nr 4
// * @param strength_5 data from internal sensor nr 5
// * @return true if no error occured, false if it did
// */
//#ifdef __HTSMUX_SUPPORT__
//bool T10FBreadAllstrength_trength(tMUXSensor muxsensor, int &strength_1, int &strength_2, int &strength_3, int &strength_4, int &strength_5) {
//  memset(T10FB_I2CReply, 0, sizeof(tByteArray));

//  if (HTSMUXSensorTypes[muxsensor] != HTSMUXSensorCustom)
//    HTSMUXconfigChannel(muxsensor, T10FB_config);

//  if (!HTSMUXreadPort(muxsensor, T10FB_I2CReply, 5, T10FB_DC_SSTR1)) {
//    return false;
//  }

//  strength_1 = T10FB_I2CReply[0];
//  strength_2 = T10FB_I2CReply[1];
//  strength_3 = T10FB_I2CReply[2];
//  strength_4 = T10FB_I2CReply[3];
//  strength_5 = T10FB_I2CReply[4];

//  return true;
//}
//#endif // __HTSMUX_SUPPORT__


///**
// * Read the value of the average data register and return it.
// * @param link the T10FB port number
// * @return value of 0-9, the direction index of the detected IR signal or -1 if an error occurred.
// */
//int T10FBreadDCAverage(tSensors link) {
//  memset(T10FB_I2CRequest, 0, sizeof(tByteArray));

//  T10FB_I2CRequest[0] = 2;              // Message size
//  T10FB_I2CRequest[1] = T10FB_I2C_ADDR; // I2C Address
//  T10FB_I2CRequest[2] = T10FB_OFFSET + T10FB_DC_SAVG;  // DC sensor signal strength average

//  if (!writeI2C(link, T10FB_I2CRequest, T10FB_I2CReply, 1))
//    return -1;

//  return T10FB_I2CReply[0];
//}


///**
// * Read the value of the average data register and return it.
// * @param muxsensor the SMUX sensor port number
// * @return value of 0-9, the direction index of the detected IR signal or -1 if an error occurred.
// */
//#ifdef __HTSMUX_SUPPORT__
//int T10FBreadDCAverage(tMUXSensor muxsensor) {
//  memset(T10FB_I2CReply, 0, sizeof(tByteArray));

//  if (HTSMUXSensorTypes[muxsensor] != HTSMUXSensorCustom)
//    HTSMUXconfigChannel(muxsensor, T10FB_config);

//  if (!HTSMUXreadPort(muxsensor, T10FB_I2CReply, 1, T10FB_DC_SAVG)) {
//    return -1;
//  }

//  return T10FB_I2CReply[0];
//}
//#endif // __HTSMUX_SUPPORT__


//// ---------------------------- AC Signal processing -----------------------------

///**
// * Set the DSP mode of the AC carrier wave detector.
// *
// * Mode is one of:
// * -DSP_1200
// * -DSP_600
// * @param link the T10FB port number
// * @param mode the frequency that should be detected
// * @return true if no error occured, false if it did
// */
//bool T10FBsetDSPMode(tSensors link, tT10FBDSPMode mode) {
//  memset(T10FB_I2CRequest, 0, sizeof(tByteArray));

//  T10FB_I2CRequest[0] = 3;              // Message size
//  T10FB_I2CRequest[1] = T10FB_I2C_ADDR; // I2C Address
//  T10FB_I2CRequest[2] = T10FB_DSP_MODE; // Start direction register
//  T10FB_I2CRequest[3] = (ubyte)mode;

//  return writeI2C(link, T10FB_I2CRequest);
//}

///**
// * Read the value of the AC Direction data register and return it.
// * @param link the T10FB port number
// * @return value of 0-9, the direction index of the detected IR signal or -1 if an error occurred.
// */
//int T10FBreadACDir(tSensors link) {
//  memset(T10FB_I2CRequest, 0, sizeof(tByteArray));

//  T10FB_I2CRequest[0] = 2;              // Message size
//  T10FB_I2CRequest[1] = T10FB_I2C_ADDR; // I2C Address
//  T10FB_I2CRequest[2] = T10FB_OFFSET + T10FB_AC_DIR;      // Start direction register

//  if (!writeI2C(link, T10FB_I2CRequest, T10FB_I2CReply, 1))
//    return -1;

//  return T10FB_I2CReply[0];
//}


///**
// * Read the value of the AC Direction data register and return it.
// * @param muxsensor the SMUX sensor port number
// * @return value of 0-9, the direction index of the detected IR signal or -1 if an error occurred.
// */
//#ifdef __HTSMUX_SUPPORT__
//int T10FBreadACDir(tMUXSensor muxsensor) {
//  memset(T10FB_I2CReply, 0, sizeof(tByteArray));

//  if (HTSMUXSensorTypes[muxsensor] != HTSMUXSensorCustom)
//    HTSMUXconfigChannel(muxsensor, T10FB_config);

//  if (!HTSMUXreadPort(muxsensor, T10FB_I2CReply, 1, T10FB_AC_DIR)) {
//    return -1;
//  }

//  return T10FB_I2CReply[0];
//}
//#endif // __HTSMUX_SUPPORT__


///**
// * Read the value of the all of the internal AC sensors and copy into specified buffer.
// * @param link the T10FB port number
// * @param acS1 data from internal sensor nr 1
// * @param acS2 data from internal sensor nr 2
// * @param acS3 data from internal sensor nr 3
// * @param acS4 data from internal sensor nr 4
// * @param acS5 data from internal sensor nr 5
// * @return true if no error occured, false if it did
// */
//bool T10FBreadAllACStrength(tSensors link, int &acS1, int &acS2, int &acS3, int &acS4, int &acS5) {
//  memset(T10FB_I2CRequest, 0, sizeof(tByteArray));

//  T10FB_I2CRequest[0] = 2;                      // Message size
//  T10FB_I2CRequest[1] = T10FB_I2C_ADDR;         // I2C Address
//  T10FB_I2CRequest[2] = T10FB_OFFSET + T10FB_AC_SSTR1;         // Sensor 0 signal strength

//  if (!writeI2C(link, T10FB_I2CRequest, T10FB_I2CReply, 5))
//    return false;

//  acS1 = T10FB_I2CReply[0];
//  acS2 = T10FB_I2CReply[1];
//  acS3 = T10FB_I2CReply[2];
//  acS4 = T10FB_I2CReply[3];
//  acS5 = T10FB_I2CReply[4];

//  return true;
//}


///**
// * Read the value of the all of the internal AC sensors and copy into specified buffer.
// * @param muxsensor the SMUX sensor port number
// * @param acS1 data from internal sensor nr 1
// * @param acS2 data from internal sensor nr 2
// * @param acS3 data from internal sensor nr 3
// * @param acS4 data from internal sensor nr 4
// * @param acS5 data from internal sensor nr 5
// * @return true if no error occured, false if it did
// */
//#ifdef __HTSMUX_SUPPORT__
//bool T10FBreadAllACStrength(tMUXSensor muxsensor, int &acS1, int &acS2, int &acS3, int &acS4, int &acS5) {
//  memset(T10FB_I2CReply, 0, sizeof(tByteArray));

//  if (HTSMUXSensorTypes[muxsensor] != HTSMUXSensorCustom)
//    HTSMUXconfigChannel(muxsensor, T10FB_config);

//  if (!HTSMUXreadPort(muxsensor, T10FB_I2CReply, 5, T10FB_AC_SSTR1)) {
//    return false;
//  }

//  acS1 = T10FB_I2CReply[0];
//  acS2 = T10FB_I2CReply[1];
//  acS3 = T10FB_I2CReply[2];
//  acS4 = T10FB_I2CReply[3];
//  acS5 = T10FB_I2CReply[4];

//  return true;
//}
//#endif // __HTSMUX_SUPPORT__

#endif // __T10FB_H__

/*
 * $Id: trs10-findball-v2.h 133 2013-03-10 15:15:38Z xander $
 */
/* @} */
/* @} */
