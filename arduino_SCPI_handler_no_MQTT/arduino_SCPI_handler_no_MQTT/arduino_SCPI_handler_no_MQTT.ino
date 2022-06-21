#include "SCPI_handler.h"
#include "ScanerCore.h"
#include "Command.h"
#include "Printer.h"


const int baudrate = 9600;

ScanerCore core;


void setup() {
  Serial.begin(baudrate);
  core.init();
  core.SetPrinter(serial);
  
  core.Println("Instrument setup complete");
}

void(* resetFunc) (void) = 0;

void loop() {
  if (Serial.available() > 0) {
    SCPI_Handler handler;
    handler.LoadString(Serial.readString());
    if (handler.ProcessCurrentCommand()) {
      Command command = handler.GetCurrentCommand();
      if (command == Reset) {
        resetFunc();
      }
      int argument = handler.GetCurrentArgument();
      core.SetPrinter(serial);
      core.ProcessCommand(command, argument); 
    }
  }
}
