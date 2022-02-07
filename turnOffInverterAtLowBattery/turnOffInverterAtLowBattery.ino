////////////////////////
// Pin Declarations
////////////////////////
const int kBUTTON = 9;
const int kINV_STATUS = 4;
const int kBAT_VOLTAGE = A3;

////////////////////////
// On off voltages
////////////////////////
//14.4V:  Boost voltage
//13.8V:  Float Voltage
//13.2V:  Boost return Voltage
//12V:    Undervoltage warning
//Set it just under float voltage...Probably makes sense to just set it to flaot voltage directly as the boost voltage is 14.4V, it should always hit 13.8V even after inacuracy of the measurement.
const float kON_VOLTAGE = 13.6;
const float kOFF_VOLTAGE = 12.3;

///////////////////////////
// Calibration constants
///////////////////////////
const float kREAD_ADC_VAL = 993.64; //0 to 1023, float due to averaging.
const float kMEASURED_VOLTAGE = 14.52; //Measured voltage at time of kREAD_ADC_VAL
const float kV_PER_CODE = kMEASURED_VOLTAGE / kREAD_ADC_VAL;

////////////////////////
// Globals
////////////////////////
bool inverterOn;
void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:
  // put your setup code here, to run once:
  //kButton is pulled up by inverter to 5V therefore we can just use kBUTTON as open drain output.  Set floating at first
  openButton();
  pinMode(kINV_STATUS, INPUT);
  inverterOn = isInverterOn();
}
void shortButton()
{ 
  Serial.println("Short button");
  pinMode(kBUTTON, OUTPUT);
  digitalWrite(kBUTTON, LOW); 
}
void openButton()
{
    Serial.println("Open button");
    pinMode(kBUTTON, INPUT);
}
void pushReleaseButton()
{
    
    shortButton();
    delay(2000);
    openButton();
    delay(2000);
}
bool isInverterOn()
{
  return digitalRead(kINV_STATUS) == LOW;
}
void turnOnInverter() {
  //While inverter is off, try to turn it on
  while (isInverterOn() == false) {
      pushReleaseButton();
      //Allow up to 11 seconds for it to turn on before trying again
      for (int i = 0; i++; i<10) {
          delay(1000);
          if (isInverterOn() == true) {break;}
      }
  }
}

void turnOffInverter() {
  while (isInverterOn() == true) {
      pushReleaseButton();
      //Allow up to 11 seconds for it to turn off before trying again
      for (int i = 0; i++; i<10) {
          delay(1000);
          if (isInverterOn() == false) {break;}
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
  value = value / numReadings;
  Serial.print("ADC averaged over 10 sec:");
  Serial.print(value);
  Serial.println("");
  // Convert adc averaged reading to Volts.
  // kV_PER_CODE aquired by measuring battery voltage and ADC code.
  // divide Voltage by ADC code read for this constant.
  return  value * kV_PER_CODE;
 
}

void loop() {

  float batteryVoltage = getBatteryVoltage();
  Serial.print("Battery Voltage:");
  Serial.print(batteryVoltage);
  Serial.println("");
  Serial.print("Inverter is ");
  if (isInverterOn()){
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
  //if (isInverterOn()) {
  //    Seria.println("Inverter is On");
      if (batteryVoltage <= kOFF_VOLTAGE) {turnOffInverter();}
  //    inverterOn = false;
  //} else {
  //    Seria.println("Inverter is Off");
      if (batteryVoltage >= kON_VOLTAGE) {turnOnInverter();} 
  //    inverterOn = true;
  //}
  
}
