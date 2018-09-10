#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
#define SENSOR_RESOLUTION 12
#define SENSOR_INDEX 0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorDeviceAddress;

void setup(void) {
  Serial.begin(9600);
  sensors.begin();
  sensors.getAddress(sensorDeviceAddress, SENSOR_INDEX);
  sensors.setResolution(sensorDeviceAddress, SENSOR_RESOLUTION);
}

void loop(void) {
  sensors.requestTemperatures();
  Serial.print("Temperature is: ");
  Serial.println(sensors.getTempCByIndex(SENSOR_INDEX), 4);
  delay(3000);
}
