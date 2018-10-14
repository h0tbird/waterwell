#include <OneWire.h>
#include <DallasTemperature.h>
#include <RH_ASK.h>

#define ONE_WIRE_BUS 2
#define SENSOR_RESOLUTION 12
#define SENSOR_INDEX 0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensorDeviceAddress;
RH_ASK rf_driver;

void setup(void) {
  Serial.begin(9600);
  sensors.begin();
  sensors.getAddress(sensorDeviceAddress, SENSOR_INDEX);
  sensors.setResolution(sensorDeviceAddress, SENSOR_RESOLUTION);
  rf_driver.init();
}

void loop(void) {
  sensors.requestTemperatures();
  Serial.print("Temperature is: ");
  Serial.println(sensors.getTempCByIndex(SENSOR_INDEX), 4);

  const char *msg = "Hello world";
  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();

  delay(5000);
}
