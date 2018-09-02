int counter = 1;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);   // set the LED on
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  delay(1000);              // wait for a second
  digitalWrite(13, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(13, LOW);    // set the LED off
  delay(100);              // wait for a second
  Serial.println("Hello World");
  Serial.print("Counter = ");
  Serial.println(counter);
  delay(1000);
  counter++;
}
