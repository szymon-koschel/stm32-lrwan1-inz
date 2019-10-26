/*
 * AirQualitySensor.cpp
 *
 *  Created on: Aug 12, 2019
 *      Author: kshs
 */

#include "Arduino.h"
#include <stdio.h>
#include "AirQualitySensor.h"

// DEVICE
#define PMS_SET 13
#define PMS_RESET 12

// TWO BYTES START
#define PRE1 0x42
#define PRE2 0x4d
#define PMSMAXBODYLEN 36

AirQualitySensor::AirQualitySensor(uint16_t TimeMeasurement,
		uint16_t TimeBreak) {
	this->TimeBreak = TimeBreak;
	this->TimeMeasurement = TimeMeasurement;
  
  pinMode(PMS_SET, OUTPUT);
  pinMode(PMS_RESET, OUTPUT);
  digitalWrite(PMS_RESET, HIGH);
}

void AirQualitySensor::On() {
  digitalWrite(PMS_SET, HIGH);
}

void AirQualitySensor::Off() {
  digitalWrite(PMS_SET, LOW);
}

void AirQualitySensor::Reset() {
  digitalWrite(PMS_RESET, LOW);
  digitalWrite(PMS_RESET, HIGH);
}

void AirQualitySensor::Toggle() {
	this->isOn() ? this->Off() : this->On();
}

bool AirQualitySensor::isOn() {
	return digitalRead(PMS_SET);
}

void AirQualitySensor::setState(AirQualitySensorState State) {
	this->State = State;
}

bool AirQualitySensor::isNewMeasurementAvailible() {
	if (this->newMeasurementAvailible) {
		this->newMeasurementAvailible = false;
		return true;
	}

	return false;
}

// Run every 1s
void AirQualitySensor::Tick() {
	if (timer == 0) {
		switch (State) {
		case WARMUP:
			this->On();
			break;
		case MEASUREMENT:
			this->averageMeasurement = AirQualityMeasurement();
			this->On();
			break;
		case BREAK:
			this->Off();
			this->newMeasurementAvailible = true;
			break;
		}
		++timer;
	} else {
		++timer;
		switch (State) {
		case WARMUP:
			if(timer == TimeWarmup) {
				this->On();
				setState(MEASUREMENT);
				timer = 0;
			} else if((timer + 1) == TimeWarmup) {
				this->Off();
			}
			break;
		case MEASUREMENT:
			if(timer == TimeMeasurement) {
				setState(BREAK);
				timer = 0;
			}
			break;
		case BREAK:
			if(timer == TimeBreak) {
				setState(WARMUP);
				timer = 0;
			}
			break;
		}

	}

}

bool AirQualitySensor::isFrameValid() {
	int sum = 0, pmssum;
	for (uint8_t i = 0; i < frameLength + 2; i++) {
		sum += Buffer[i];
	}

	pmssum = ((uint16_t) Buffer[frameLength + 2] << 8)
			| Buffer[frameLength + 3];

	return (pmssum == sum);

}

void AirQualitySensor::readMeasurementFromBuffer() {

	this->lastMeasurement = AirQualityMeasurement();

	uint32_t PM1 = (Buffer[4] << 8) | Buffer[5];
	uint32_t PM2_5 = (Buffer[6] << 8) | Buffer[7];
	uint32_t PM10 = (Buffer[8] << 8) | Buffer[9];

	this->lastMeasurement.addMeasurement(PM1, PM2_5, PM10);
	this->averageMeasurement.addMeasurement(PM1, PM2_5, PM10);
}

uint8_t AirQualitySensor::addByte(uint8_t readbyte) {
	if(State != MEASUREMENT)
		return 0;

	uint8_t size = BufferSize;
	if ((readbyte == PRE1 && size == 0) || (readbyte == PRE2 && size == 1)
			|| size == 2) {
		Buffer[BufferSize++] = readbyte;
	} else if (size == 3) {
		Buffer[BufferSize++] = readbyte;

		this->frameLength = ((uint16_t) Buffer[2] << 8) | Buffer[3];
		if (frameLength > PMSMAXBODYLEN) {
			++framesInvalid;
			BufferSize = 0;
		}
	} else if (size >= 4) {
		if (size < frameLength + 4) {
			Buffer[BufferSize++] = readbyte;
		} else {
//			CHECK CHECKSUM
			if (isFrameValid()) {
				++framesValid;
//			ADD MEASUREMENT
				readMeasurementFromBuffer();
			}

			BufferSize = 0;
			return 1;
		}
	} else {
		if (size)
			BufferSize = 0;
	}

	return 0;
}

AirQualitySensor::~AirQualitySensor() {
	// TODO Auto-generated destructor stub
}
