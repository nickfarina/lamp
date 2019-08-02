#include <CapacitiveSensor.h>

CapacitiveSensor sensorA = CapacitiveSensor(4,2); // 1 megohm resistor between pins 4 & 2,
                                                 // pin 2 is sensor pin, add wire, foil
long baseline = 0;
int calibrationCycles = 500;

int HOVER_THRESHOLD = 20;
int TOUCH_THRESHOLD = 100;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  sensorA.set_CS_AutocaL_Millis(0xFFFFFFFF);

  calibrate();
}

// the loop function runs over and over again forever
void loop() {
  long sensorAReading = sensorA.capacitiveSensor(30);
  long change = (sensorAReading - baseline);

//  Serial.println(change);

  
  if (change > TOUCH_THRESHOLD) {
    Serial.println("Touching!!");
  } else if (change > HOVER_THRESHOLD) {
    Serial.println("Hovering!");
  }
}

void calibrate() {
  int sum= 0;
  
  for (int i = 0; i < calibrationCycles ; i++) {
    long sensorAReading = sensorA.capacitiveSensor(30);
    sum += sensorAReading;
    Serial.println(sensorAReading);
  }

  baseline = sum / calibrationCycles;
  
  for (int i = 0; i < 30; i++) {
    Serial.println(baseline);
  } 
}
