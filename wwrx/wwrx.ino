#include <RH_ASK.h>

//-----------------------------------------------------------------------------
// Variables:
//-----------------------------------------------------------------------------

RH_ASK rf_driver;
unsigned long time;

//-----------------------------------------------------------------------------
// setup:
//-----------------------------------------------------------------------------

void setup(void) {
  Serial.begin(9600);
  rf_driver.init();
}

//-----------------------------------------------------------------------------
// loop:
//-----------------------------------------------------------------------------

void loop(void) {

  uint8_t buf[73];
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen)) {
    time = millis();
    Serial.print((unsigned int)time, DEC);
    Serial.println((char *)buf);
  }
}
