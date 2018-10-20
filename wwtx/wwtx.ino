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
DeviceAddress deviceAddress;
RH_ASK rf_driver;

// Sensors:
DeviceAddress sensor00 = { 0x28, 0xB, 0xCA, 0x77, 0x91, 0x9, 0x2, 0xDA };

//-----------------------------------------------------------------------------
// printSensors
//-----------------------------------------------------------------------------

void printSensors(void){

  uint8_t deviceCount;
  byte dev,addr[8],i;

  // Count sensors:
  deviceCount = sensors.getDeviceCount();
  Serial.print("Device count: ");
  Serial.println(deviceCount);

  for(dev = 0; dev < deviceCount; dev++) {

    Serial.print("device-");
    Serial.print(dev);
    Serial.print(" =");

    sensors.getAddress(addr, (uint8_t)dev);

    for (i = 0; i < 8; i++) {
      Serial.write(' ');
      Serial.print(addr[i], HEX);
    }

    Serial.println();
  }
}

//-----------------------------------------------------------------------------
// setup
//-----------------------------------------------------------------------------

void setup(void) {

  // Serial:
  Serial.begin(9600);

  // DS18B20:
  sensors.begin();
  sensors.getAddress(deviceAddress, SENSOR_INDEX);
  sensors.setResolution(deviceAddress, SENSOR_RESOLUTION);

  // 433Mhz:
  rf_driver.init();

  // Info:
  printSensors();
}

//-----------------------------------------------------------------------------
// loop
//-----------------------------------------------------------------------------

void loop(void) {

  // Acquire the data:
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(SENSOR_INDEX);
  str_temp = String(temp);

  // Print to serial console:
  Serial.print("Temperature TX: ");
  Serial.println(str_temp);

  // Transfer via 433MHz:
  static char *msg = str_temp.c_str();
  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();

  // Sleep:
  delay(2000);
}
