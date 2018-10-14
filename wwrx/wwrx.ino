#include <RH_ASK.h>

RH_ASK rf_driver;

void setup(void) {
  Serial.begin(9600);
  rf_driver.init();
}

void loop(void) {

  uint8_t buf[5];
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen)) {
    Serial.print("Temperature is: ");
    Serial.println((char *)buf);
  }
}
