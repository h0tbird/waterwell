#include <OneWire.h>
#include <DallasTemperature.h>
#include <RH_ASK.h>

// Constants:
#define ONE_WIRE_BUS 2
#define SENSOR_RESOLUTION 12
#define SENSOR_COUNT 3

// Variables:
float temp;
String str1, str2;

// Initialize:
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
RH_ASK rf_driver;

// Sensors:
DeviceAddress sensorList [SENSOR_COUNT] = {
  { 0x28, 0x7, 0xCA, 0x77, 0x91, 0x16, 0x2, 0x53 }, // 01
  { 0x28, 0xEC, 0x2D, 0x77, 0x91, 0x4, 0x2, 0x74 }, // 02
  { 0x28, 0xDB, 0x63, 0x77, 0x91, 0x3, 0x2, 0x39 }, // 03
  // { 0x28, 0xB, 0xCA, 0x77, 0x91, 0x9, 0x2, 0xDA },  // 10
};

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

  for(dev = 1; dev <= deviceCount; dev++) {

    Serial.print("device-");
    Serial.print(dev);
    Serial.print(" =");

    sensors.getAddress(addr, (uint8_t)dev-1);

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
  for (int i = 0; i < SENSOR_COUNT; i++) {
    sensors.setResolution(sensorList[i], SENSOR_RESOLUTION);
  }

  // 433Mhz:
  rf_driver.init();

  // Info:
  printSensors();
}

//-----------------------------------------------------------------------------
// loop
//-----------------------------------------------------------------------------

void loop(void) {

  // Initialize the data:
  sensors.requestTemperatures();
  str1 = String();
  str2 = String();

  // Read the temperatures:
  for (int i = 0; i < SENSOR_COUNT; i++) {
    temp = sensors.getTempC(sensorList[i]);
    str2 = str1 + ' ' + temp;
    str1 = str2;
  }

  // Print to serial console:
  Serial.print("Temperatures TX:");
  Serial.println(str2);

  // Transfer via 433MHz:
  static char *msg = str2.c_str();
  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();

  // Sleep:
  delay(2000);
}
