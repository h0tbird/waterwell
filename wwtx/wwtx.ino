#include <OneWire.h>
#include <DallasTemperature.h>
#include <RH_ASK.h>

// Constants:
#define ONE_WIRE_BUS_0 2
#define ONE_WIRE_BUS_1 3
#define ONE_WIRE_BUS_2 4
#define ONE_WIRE_BUS_3 5
#define SENSOR_RESOLUTION 12
#define BUSES_COUNT 3
#define SENSOR_COUNT 3

// Variables:
float temp;
String str1, str2;

// Initialize buses:
OneWire buses[BUSES_COUNT] = {
  OneWire(ONE_WIRE_BUS_0),
  OneWire(ONE_WIRE_BUS_1),
  OneWire(ONE_WIRE_BUS_2),
};

// Initialize sensors:
DallasTemperature sensors[BUSES_COUNT] = {
  DallasTemperature(&buses[0]),
  DallasTemperature(&buses[1]),
  DallasTemperature(&buses[2]),
};

// Sensors list:
DeviceAddress sensorsList [BUSES_COUNT] [SENSOR_COUNT] = {
  {
    { 0x28, 0x7, 0xCA, 0x77, 0x91, 0x16, 0x2, 0x53 },  // 01
    { 0x28, 0xEC, 0x2D, 0x77, 0x91, 0x4, 0x2, 0x74 },  // 02
    { 0x28, 0xDB, 0x63, 0x77, 0x91, 0x3, 0x2, 0x39 },  // 03
  },
  {
    { 0x28, 0xD0, 0xEC, 0x77, 0x91, 0x9, 0x2, 0x82 },  // 04
    { 0x28, 0xE0, 0x6D, 0x77, 0x91, 0x10, 0x2, 0x37 }, // 05
    { 0x28, 0x2C, 0x1C, 0x77, 0x91, 0x6, 0x2, 0x33 },  // 06
  },
  {
    { 0x28, 0x54, 0x6E, 0x77, 0x91, 0xC, 0x2, 0x3 },   // 07
    { 0x28, 0x3, 0xE2, 0x77, 0x91, 0xE, 0x2, 0xD3 },   // 08
    { 0x28, 0x76, 0xA2, 0x77, 0x91, 0x15, 0x2, 0x86 }, // 09
  },
};

// Initialize 433MHz:
RH_ASK rf_driver;

//-----------------------------------------------------------------------------
// printSensors
//-----------------------------------------------------------------------------

void printSensors(void){

  uint8_t deviceCount;
  byte addr[8];

  for (int b = 0; b < BUSES_COUNT; b++) {

    deviceCount = sensors[b].getDeviceCount();
    Serial.print("\nBus[");
    Serial.print(b);
    Serial.print("] has ");
    Serial.print(deviceCount);
    Serial.println(" sensors:");

    for(int s = 0; s < deviceCount; s++) {

      Serial.print("sensor[");
      Serial.print(s);
      Serial.print("] =");

      sensors[b].getAddress(addr, (uint8_t)s);

      for (int i = 0; i < 8; i++) {
        Serial.write(' ');
        Serial.print(addr[i], HEX);
      }
      Serial.println();
    }
  }
  Serial.println();
}

//-----------------------------------------------------------------------------
// setup
//-----------------------------------------------------------------------------

void setup(void) {

  // Serial:
  Serial.begin(9600);

  // DS18B20:
  for (int b = 0; b < BUSES_COUNT; b++) {
    sensors[b].begin();
    for (int s = 0; s < SENSOR_COUNT; s++) {
      sensors[b].setResolution(sensorsList[b][s], SENSOR_RESOLUTION);
    }
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
  str1 = String();
  str2 = String();

  for (int b = 0; b < BUSES_COUNT; b++) {

    sensors[b].requestTemperatures();

    // Read the temperatures:
    for (int s = 0; s < SENSOR_COUNT; s++) {
      temp = sensors[b].getTempC(sensorsList[b][s]);
      str2 = str1 + ' ' + temp;
      str1 = str2;
    }
  }

  static char *msg = str2.c_str();

  // Print to serial console:
  Serial.print("Temperatures TX:");
  Serial.print(str2);
  Serial.print(" (");
  Serial.print(strlen(msg));
  Serial.println(")");

  // Transfer via 433MHz:
  rf_driver.send((uint8_t *)msg, strlen(msg));
  rf_driver.waitPacketSent();

  // Sleep:
  delay(5000);
}
