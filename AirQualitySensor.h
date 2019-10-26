/*
 * AirQualitySensor.h
 *
 *  Created on: Aug 12, 2019
 *      Author: kshs
 */

#ifndef AIRQUALITYSENSOR_AIRQUALITYSENSOR_H_
#define AIRQUALITYSENSOR_AIRQUALITYSENSOR_H_

#include "Measurement/AirQualityMeasurement.h"

enum AirQualitySensorState {
	WARMUP,
	MEASUREMENT,
	BREAK
};

class AirQualitySensor {
	uint16_t TimeWarmup = 10;
	uint16_t TimeMeasurement;
	uint16_t TimeBreak;

	AirQualitySensorState State = WARMUP;

	bool newMeasurementAvailible = false;

	uint8_t BufferSize = 0;
	uint8_t Buffer[40];
	uint32_t framesInvalid = 0;
	uint8_t frameLength;
	uint32_t timer = 0;
	bool isFrameValid();
	void readMeasurementFromBuffer();

	bool isOn();
	void Toggle();
	void Reset();

	void setState(AirQualitySensorState State);
public:
	AirQualityMeasurement lastMeasurement;
	AirQualityMeasurement averageMeasurement;

	AirQualitySensor(uint16_t TimeMeasurement, uint16_t TimeBreak);
	virtual ~AirQualitySensor();

	void Tick();

	bool isNewMeasurementAvailible();
  void On();
  void Off();
  
	uint8_t addByte(uint8_t readbyte);
	uint32_t framesValid = 0;

};

#endif /* AIRQUALITYSENSOR_AIRQUALITYSENSOR_H_ */
