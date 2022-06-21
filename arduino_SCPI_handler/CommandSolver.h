#ifndef MY_COMMANDSOLVER
#define MY_COMMANDSOLVER

#include <Arduino.h>
#include "ScanerCore.h"
#include "Command.h"

class CommandSolver {

  private:
    ScanerCore core;
  
  public:
 
    CommandSolver(ScanerCore core);
    void init();

    void ProcessCommand(Command command, int argument);
};

#endif
