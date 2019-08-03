#include <QuadraticEase.h>
#include <CapacitiveSensor.h>

CapacitiveSensor sensorA = CapacitiveSensor(4,2); // 3 megohm resistors between pins 4 & 2,
                                                  // pin 2 is sensor pin, add wire, foil

int LED_PIN = 9;

// Interaction Thresholds
int HOVER_THRESHOLD = 80;
int TOUCH_THRESHOLD = 9;

// Calibration
long baseline = 0;
int calibrationCycles = 500;
int sensorMin = 1000;
int sensorMax = 0;

// Smoothing
const int numReadings = 25;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

// easing
QuadraticEase ease;
const double STARTING_EASE_TIME = 1.5;
const int STARTING_LIGHT_VALUE = 15; // i think starting at low values causes flicker
double easedPosition = 0;
double prevEasePosition = 0;
double easeTime = STARTING_EASE_TIME;

// State Variables
boolean HOVERING = false;
boolean TOUCHING = false;
boolean PREVIOUS_TOUCHING = false;

boolean lightState = LOW;

int lightValue = 0;
int MAX_LIGHT_VALUE = 255;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  sensorA.set_CS_AutocaL_Millis(0xFFFFFFFF);

  // setup easing
  ease.setDuration(9);
  ease.setTotalChangeInPosition(275);

  calibrate();

  // initialize smoothing array
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
  long sensorAReading = sensorA.capacitiveSensor(30);
  long change = (sensorAReading - sensorMax);
  
  smoothInput(change);
    
  detectInteraction();
  toggleLightState();

  PREVIOUS_TOUCHING = TOUCHING;

  adjustLights();
}

void smoothInput(long change) {
  total = total - readings[readIndex];
  readings[readIndex] = change;
  total = total + readings[readIndex];

  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  average = total / numReadings;
}

void adjustLights() {
  if (lightState == HIGH && TOUCHING) {
    incrementEase();
  } else if (lightState == LOW) {
    resetEase();
  }

  Serial.println(lightValue);
  
  analogWrite(LED_PIN, lightValue);
}

void incrementEase() {
  if (lightValue < 255) {
    lightValue = ease.easeInOut(easeTime);
    double roundedPosition = round(lightValue);
  
    if (lightValue < STARTING_LIGHT_VALUE) { lightValue = 0; }
  
    easeTime += 0.013;
    prevEasePosition = lightValue;
  
    delay(3);
  }
}

void resetEase() {
  easeTime = STARTING_EASE_TIME;
  lightValue = 0;
  prevEasePosition = 0;
}

void toggleLightState() {
  if (TOUCHING == true && PREVIOUS_TOUCHING == false) {
    lightState = 1 - lightState;
  }
}

void detectInteraction() {
  if (average> TOUCH_THRESHOLD) {
    HOVERING = false;
    TOUCHING = true;
  } else if (average> HOVER_THRESHOLD) {
    HOVERING = true;
  } else {
    HOVERING = false;
    TOUCHING = false;
  }

//  pp("Light value: ", lightValue);
//  pp("Hovering? ", HOVERING);
//  pp("Touching? ", TOUCHING);
//  pp("Light State? ", lightState);
}

void calibrate() {
  int sum= 0;
  
  for (int i = 0; i < calibrationCycles ; i++) {
    long sensorAReading = sensorA.capacitiveSensor(30);
    
    if (sensorAReading > sensorMax) {
      sensorMax = sensorAReading;
    }

    if (sensorAReading < sensorMin) {
      sensorMin = sensorAReading;
    }
  }
  
//  pp("min: ", sensorMin);
//  pp("max: ", sensorMax);
}

// pretty print utility
void pp(String str, int num) {
  Serial.print(str);
  Serial.println(num);
}
