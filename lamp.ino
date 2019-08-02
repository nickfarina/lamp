#include <CapacitiveSensor.h>

CapacitiveSensor sensorA = CapacitiveSensor(4,2); // 1 megohm resistor between pins 4 & 2,
                                                 // pin 2 is sensor pin, add wire, foil
long baseline = 30;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  sensorA.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn off autocalibrate on channel 1 - just as an example
}

// the loop function runs over and over again forever
void loop() {
  long sensorAReading = sensorA.capacitiveSensor(30);

  Serial.println(sensorAReading);

  delay(10);
}
