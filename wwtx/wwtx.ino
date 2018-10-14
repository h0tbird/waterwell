#include <OneWire.h>
#include <DallasTemperature.h>
#include <RH_ASK.h>

// Constants:
#define ONE_WIRE_BUS 2
#define SENSOR_RESOLUTION 12
#define SENSOR_INDEX 0

// Variables:
float temp;
String str_temp;

// Initialize:
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

  // Acquire the data:
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(SENSOR_INDEX);
  str_temp = String(temp);

  // Print to serial console:
  Serial.print("Temperature is: ");
  Serial.println(str_temp);

  // Transfer via 433MHz:
  static char *msg = str_temp.c_str();
  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();

  // Sleep:
  delay(2000);
}
