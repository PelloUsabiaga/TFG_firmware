#include "ScanerCore.h"

const int dirPin = 2;
const int stepPin = 3;
const int motorInterfaceType = 1;
const long tipical_variation = 100;
const float a = 1;
const int motorRelePin = 4;
const int autoHomeSwitchPin = 5;



ScanerCore::ScanerCore(){}

void ScanerCore::init() {
  pinMode(motorRelePin, OUTPUT);
  
  ads.setGain(GAIN_SIXTEEN);
  if (!ads.begin()) {
    while (1);
  }
  stepper1 = AccelStepper(motorInterfaceType, stepPin, dirPin);
  stepper1.setMaxSpeed(800.0);
  stepper1.setAcceleration(10000.0);  

}

void ScanerCore::ProcessCommand(Command command, int argument) {
  switch(command) {
    case measureBuoyancy : {
      Println(String(MeasureBuoyancy(argument), 5));
      break;
    }
    case measureBuoyancyTime : {
      Println(String(MeasureBuoyancyTime(argument), 5));
      break;
    }    
    case Id : {
      Println(F("UPV/EHU, 3D scan, 0001, 0.0"));
      break;
    }
    case moveAxis : {
      RunAxisTo(argument);
      break;
    }
    case setAxisHome : {
      SetAxisAtZero();
      break;
    }
    case setMotorOn: {
      SetMotorOn();
      break;
    }
    case setMotorOff: {
      SetMotorOff();
      break;
    }
    case autoHomeAxis: {
      AutoHomeAxis();
      break;
    }
    case measureBuoyancyValues: {
      MeasureBuoyancyValues(argument);
      break;
    }
  }
}

void ScanerCore::SetPrinter(Printer printer) {
  currentPrinter = printer;
}

void ScanerCore::StartMessage() {
  switch(currentPrinter) {
    case serial : {
      break;
    }
    case mqtt : {
      break;
    }
  }
}

void ScanerCore::EndMessage() {
  switch(currentPrinter) {
    case serial : {
      Serial.println("");
      break;
    }
    case mqtt : {
      break;
    }
  }
}

void ScanerCore::Print(String argument) {
  switch(currentPrinter) {
    case serial : {
      Serial.print(argument);
      break;
    }
    case mqtt : {
      break;
    }
  }
}

void ScanerCore::PrintMessage(String argument) {
  switch(currentPrinter) {
    case serial : {
      Serial.println(argument);
      break;
    }
    case mqtt : {
      break;
    }
  }
}

void ScanerCore::Println(String argument) {
  switch(currentPrinter) {
    case serial : {
      Serial.println(argument);
      break;
    }
    case mqtt : {

      break;
    }
  }
}

void ScanerCore::Reset() {
  
}

int ScanerCore::SelfTest() {
  return 0;
}

long ScanerCore::MeasureADC() {
  return ads.readADC_Differential_0_1();
}


double ScanerCore::MeasureBuoyancyTime(int milliseconds) {
  long previous_reading = -1;
  long current_reading;
  int error_count = 0;
  
  double acumulator = 0;
  int acumulated_n;
  
  error_count = 0;
  previous_reading = MeasureADC();

  long initial_time = millis();
  while (millis() - initial_time < milliseconds) {
    current_reading = MeasureADC();
    if (abs(current_reading - previous_reading) < tipical_variation*a) {
      previous_reading = current_reading;
      acumulator += current_reading;
      acumulated_n++;
      error_count = 0;
    } 
    else {
      error_count++;
    }
    
    if (error_count > 10) {
      previous_reading = current_reading;
      error_count = 5;
    }
  }
  return acumulator / acumulated_n;
}


double ScanerCore::MeasureBuoyancy(int averages) {
  long previous_reading;
  long current_reading;
  int error_count = 0;
  int total_error_count = 0;
  
  int acumulated_n = 0;
  double acumulator = 0;
  
  previous_reading = MeasureADC();

  while (acumulated_n != averages) {
    current_reading = MeasureADC();
    if (abs(current_reading - previous_reading) < tipical_variation*a) {
      previous_reading = current_reading;
      acumulator += current_reading;
      acumulated_n++;
      error_count = 0;
    } 
    else {
      error_count++;
      total_error_count++;
    }
    
    if (error_count > 10) {
      previous_reading = current_reading;
      error_count = 5;
    }
  }
  if (total_error_count <= (averages / 30)) {
    return acumulator / averages;
  }
  else {
    return MeasureBuoyancy(averages);  
  }
}


void ScanerCore::MeasureBuoyancyValues(int averages) {  
  for (int i = 0; i < averages; i++) {
    Print(String(MeasureADC()));
    Print(",");
  }
  Println("");
}


void ScanerCore::RunAxisTo(long absolute) {
  stepper1.moveTo(absolute);
  stepper1.runToPosition();
}


void ScanerCore::AutoHomeAxis() {
  stepper1.setCurrentPosition(0);
  long nowPosition = 0;
  SetMotorOn();
  while (digitalRead(autoHomeSwitchPin) == HIGH) {
    nowPosition += 100;
    RunAxisTo(nowPosition);
  }
  SetAxisAtZero();
}

void ScanerCore::SetAxisAtZero() {
  stepper1.setCurrentPosition(0);
}

void ScanerCore::SetMotorOn() {
  digitalWrite(motorRelePin, HIGH);
}

void ScanerCore::SetMotorOff() {
  digitalWrite(motorRelePin, LOW);
}
