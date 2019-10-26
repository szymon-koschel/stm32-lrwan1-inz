/*
 * Measurement.h
 *
 *  Created on: Aug 12, 2019
 *      Author: kshs
 */

#ifndef AIRQUALITYSENSOR_MEASUREMENT_H_
#define AIRQUALITYSENSOR_MEASUREMENT_H_
#include <stdint.h>

class AirQualityMeasurement {
	uint32_t quantity;
	uint32_t PM1;
	uint32_t PM2_5;
	uint32_t PM10;
public:
	AirQualityMeasurement();
	uint32_t getPM1();
	uint32_t getPM2_5();
	uint32_t getPM10();
	AirQualityMeasurement getAverage();
	void addMeasurement(uint32_t PM1, uint32_t PM2_5, uint32_t PM10);
	void print();
};

#endif /* AIRQUALITYSENSOR_MEASUREMENT_H_ */
