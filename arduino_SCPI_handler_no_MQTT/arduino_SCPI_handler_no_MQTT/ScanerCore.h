#ifndef MY_SCANER_CORE
#define MY_SCANER_CORE

#include <Arduino.h>
#include <AccelStepper.h>
#include <Adafruit_ADS1X15.h>
#include "Command.h"
#include "Printer.h"



class ScanerCore {

  private:
    Adafruit_ADS1115 ads;
    AccelStepper stepper1;
    Printer currentPrinter;


  public:
    ScanerCore();
    void init();

    void Reset();
    int SelfTest();

    void SetPrinter(Printer printer);
    void ProcessCommand(Command command, int argument);
    void StartMessage();
    void EndMessage();
    void Print(String argument);
    void Println(String argument);
    void PrintMessage(String argument);
    
    double MeasureBuoyancy(int averages);
    void MeasureBuoyancyValues(int averages);
    double MeasureBuoyancyTime(int milliseconds);

    long MeasureADC();
    void RunAxisTo(long absolute);
    void SetAxisAtZero();
    void AutoHomeAxis();
    void SetMotorOn();
    void SetMotorOff();
};



#endif
