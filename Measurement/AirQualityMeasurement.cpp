/*
 * Measurement.cpp
 *
 *  Created on: Aug 12, 2019
 *      Author: kshs
 */

#include <AirQualitySensor/Measurement/AirQualityMeasurement.h>
#include <stdio.h>

AirQualityMeasurement::AirQualityMeasurement() {
	this->PM1 = 0;
	this->PM2_5 = 0;
	this->PM10 = 0;

	this->quantity = 0;

}

void AirQualityMeasurement::addMeasurement(uint32_t PM1, uint32_t PM2_5, uint32_t PM10) {
	this->PM1 += PM1;
	this->PM2_5 += PM2_5;
	this->PM10 += PM10;

	++this->quantity;
}

void AirQualityMeasurement::print() {
	printf("\r\n Measurement:\nPM1: %lu\nPM2.5: %lu\nPM10: %lu\nNumer: %lu\n\r", PM1/quantity, PM2_5/quantity, PM10/quantity, quantity);
}

