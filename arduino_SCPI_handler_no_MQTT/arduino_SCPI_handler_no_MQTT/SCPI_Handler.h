#ifndef MY_SCPI_HANDLER
#define MY_SCPI_HANDLER

#include <Arduino.h>
#include "Command.h"

class SCPI_Handler {

  private:
    String currentCommand;
    String previousCommandRoot;
    int argument;
    Command currentCommandId;
  
  public:
  
    SCPI_Handler();
    void init();

    void LoadString(String string);
    bool ProcessCurrentCommand();
    bool ProcessStandardCommand();
    bool ProcessGeneralCommand();
    Command GetCurrentCommand();
    int GetCurrentArgument();
};

#endif
