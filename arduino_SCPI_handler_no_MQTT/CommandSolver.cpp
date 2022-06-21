#include "CommandSolver.h"

CommandSolver::CommandSolver(ScanerCore core){
  this->core = core;
  init();
}

void CommandSolver::init() {
  
}

void CommandSolver::ProcessCommand(Command command, int argument) {
  switch(command) {
    case MeasureBuoyancyCommand : {
      Serial.println("hemen nago");
      Serial.println(core.MeasureBuoyancy(argument));
      break;
    }
    case Id : {
      Serial.println(F("UPV/EHU, 3D scan, 0001, 0.0"));
      break;
    }
  }
}
