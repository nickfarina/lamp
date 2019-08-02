#include <CapacitiveSensor.h>

CapacitiveSensor sensorA = CapacitiveSensor(4,2); // 1 megohm resistor between pins 4 & 2,
                                                  // pin 2 is sensor pin, add wire, foil
long baseline = 0;
int calibrationCycles = 500;

int HOVER_THRESHOLD = 20;
int TOUCH_THRESHOLD = 100;

// State Variables
boolean HOVERING = false;
boolean TOUCHING = false;
boolean PREVIOUS_TOUCHING = false;

boolean lightState = LOW;

int lightValue = 0;
int MAX_LIGHT_VALUE = 255;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  sensorA.set_CS_AutocaL_Millis(0xFFFFFFFF);

  calibrate();
}

void loop() {
  long sensorAReading = sensorA.capacitiveSensor(30);
  long change = (sensorAReading - baseline);
  
  detectInteraction(change);

  if (TOUCHING == true && PREVIOUS_TOUCHING == false) {
    if (lightState == LOW) {
      lightState = HIGH;
    } else if (lightState == HIGH) {
      lightState = LOW;
    }
  }

  PREVIOUS_TOUCHING = TOUCHING;

  // adjust lights
  if (lightState == HIGH && TOUCHING) {
    lightValue++;
  } else if (lightState == LOW) {
    lightValue = 0;
  }
  
}

void detectInteraction(long change) {
  if (change > TOUCH_THRESHOLD) {
    HOVERING = false;
    TOUCHING = true;
  } else if (change > HOVER_THRESHOLD) {
    HOVERING = true;
  } else {
    HOVERING = false;
    TOUCHING = false;
  }

  Serial.print("Light value: ");
  Serial.print(lightValue);
  Serial.print(", Hovering? ");
  Serial.print(HOVERING);
  Serial.print(", Touching? ");
  Serial.print(TOUCHING);  
  Serial.print("Light State? ");
  Serial.println(lightState);
}

void calibrate() {
  int sum= 0;
  
  for (int i = 0; i < calibrationCycles ; i++) {
    long sensorAReading = sensorA.capacitiveSensor(30);
    sum += sensorAReading;
  }

  baseline = sum / calibrationCycles;
  
  Serial.println("baseline: " + baseline);
}
