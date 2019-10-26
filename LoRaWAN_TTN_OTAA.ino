#include "LoRaWAN.h"
#include "TimerMillis.h"
#include "AirQualitySensor.h"

#define REGION_EU868

TimerMillis timer;


const char *appEui  = "70B3D57ED001FF54";
const char *appKey  = "BD4CE45A76C2D84FC82F4931E32CC93D";
const char *devEui  = "00841424E2832512";
AirQualitySensor* sensor;

void setup( void )
{

  sensor = new AirQualitySensor(20, 5);

  Serial.begin(9600);

  while (!Serial) { }

  Serial1.begin(9600);

  while (!Serial1) { }

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);

  LoRaWAN.begin(EU868);
  LoRaWAN.addChannel(1, 868300000, 0, 6);

  LoRaWAN.setDutyCycle(false);
  LoRaWAN.setAntennaGain(2.0);
  LoRaWAN.joinOTAA(appEui, appKey, devEui);

  Serial.println("JOIN( )");
  timer.start(callbackTimer, 0, 1000);
}

void loop( void )
{



  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    //      Serial.print(inChar);
    sensor->addByte(inChar);

    if (sensor->isNewMeasurementAvailible()) {
      Serial.println("NEW DATA");
      sensor->averageMeasurement.print();

      digitalWrite(PIN_LED2, HIGH);
      delay(500);
      digitalWrite(PIN_LED2, LOW);
    }

  }



  if (LoRaWAN.joined() && !LoRaWAN.busy())
  {
    Serial.print("TRANSMIT( ");
    Serial.print("TimeOnAir: ");
    Serial.print(LoRaWAN.getTimeOnAir());
    Serial.print(", NextTxTime: ");
    Serial.print(LoRaWAN.getNextTxTime());
    Serial.print(", MaxPayloadSize: ");
    Serial.print(LoRaWAN.getMaxPayloadSize());
    Serial.print(", DR: ");
    Serial.print(LoRaWAN.getDataRate());
    Serial.print(", TxPower: ");
    Serial.print(LoRaWAN.getTxPower(), 1);
    Serial.print("dbm, UpLinkCounter: ");
    Serial.print(LoRaWAN.getUpLinkCounter());
    Serial.print(", DownLinkCounter: ");
    Serial.print(LoRaWAN.getDownLinkCounter());
    Serial.println(" )");

    LoRaWAN.beginPacket();
    LoRaWAN.write(0xef);
    LoRaWAN.write(0xbe);
    LoRaWAN.write(0xad);
    LoRaWAN.write(0xde);
    LoRaWAN.endPacket();
  }

}

void callbackTimer() {
  sensor->Tick();
}
