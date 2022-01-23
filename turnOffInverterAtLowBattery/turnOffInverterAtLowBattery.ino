const int kBUTTON = 3;
const int kINV_STATUS = 4;
const int kBAT_VOLTAGE = A3;
const float kON_VOLTAGE = 12.6;
const float kOFF_VOLTAGE = 12.3;
bool inverterOn;
void setup() {
  // put your setup code here, to run once:
  pinMode(kBUTTON, OUTPUT);
  pinMode(kINV_STATUS, INPUT);
  inverterOn = digitalRead(kINV_STATUS) == HIGH;
}

void turnOnInverter(bool lookAtStatus) {
  inverterOn = digitalRead(kINV_STATUS) == HIGH;
  // Always try to change if lookAtStatus is false
  while (inverterOn == false || lookAtStatus == false) {
      digitalWrite(kBUTTON, HIGH);
      delay(1000);
      digitalWrite(kBUTTON, LOW);
      delay(1000);
      if (lookAtStatus == false) { return; }
      for (int i = 0; i++; i<10) {
          delay(1000);
          inverterOn = digitalRead(kINV_STATUS);
          if (inverterOn = HIGH) {break;}
      }
  }
}

void turnOffInverter(bool lookAtStatus) {
  inverterOn = digitalRead(kINV_STATUS) == HIGH;
  while (inverterOn == true || lookAtStatus == false) {
      digitalWrite(kBUTTON, HIGH);
      delay(1000);
      digitalWrite(kBUTTON, LOW);
      delay(1000);
      if (lookAtStatus == false) { return; }
      for (int i = 0; i++; i<10) {
          delay(1000);
          inverterOn = digitalRead(kINV_STATUS);
          if (inverterOn = HIGH) {break;}
      }
  }
}

float getBatteryVoltage() {
  int i;
  float value = 0;
  //10000 readings at 1ms each makes for 10 second averaged voltage, which is fast enough for a battery monitor
  float numReadings = 10000;

  for (i = 0; i < int(numReadings); i++){
    value = value + float(analogRead(kBAT_VOLTAGE));
    delay(1);
  }

  // Take an average of all the readings.
  return = value / numReadings;
 
}

void loop() {
  // put your main code here, to run repeatedly:

  float batteryVoltage = getBatteryVoltage();
  
  if (inverterOn) {
      if (getBatteryVoltage() <= kOFF_VOLTAGE) {turnOffInverter(true);}
  } else {
      if (getBatteryVoltage() >= kON_VOLTAGE) {turnOffInverter(true);} 
  }
  
}
