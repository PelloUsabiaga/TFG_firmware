 #include "SCPI_Handler.h"


SCPI_Handler::SCPI_Handler(){
  init();
}

void SCPI_Handler::init() {
  
}


void SCPI_Handler::LoadString(String string) {
  currentCommand = string;
}

bool SCPI_Handler::ProcessCurrentCommand() {
  if (!ProcessStandardCommand()){
    if (!ProcessGeneralCommand()) {
      Serial.print("Command not suported: ");
      Serial.println(currentCommand);
      return false;
    }
  }
  return true;
}

bool SCPI_Handler::ProcessGeneralCommand() {
  if (currentCommand.startsWith(":")) {
    currentCommand = previousCommandRoot + currentCommand;
  }
  previousCommandRoot = "";
  argument = 0;
  if (currentCommand.startsWith("MEAS")) {
    previousCommandRoot += "MEAS";
    if (currentCommand.startsWith("MEAS:BUOY")) {
      if (currentCommand.startsWith("MEAS:BUOY ")) {
        argument = currentCommand.substring(sizeof("MEAS:BUOY ") - 1).toInt();
        currentCommandId = measureBuoyancy;
      }
      else if (currentCommand.startsWith("MEAS:BUOY:TIME ")) {
        argument = currentCommand.substring(sizeof("MEAS:BUOY:TIME ") - 1).toInt();
        currentCommandId = measureBuoyancyTime;
      }
      else if(currentCommand.startsWith("MEAS:BUOY:VALS ")) {
        argument = currentCommand.substring(sizeof("MEAS:BUOY:VALS ") - 1).toInt();
        currentCommandId = measureBuoyancyValues;
      }
      else {
        return false;
      }
    }
    else if (currentCommand.startsWith("MEAS:TIME")) {
      currentCommandId = MeasureTime;
    }
    else {
      return false;
    }
  }
  else if (currentCommand.startsWith("OUTP")) {
    previousCommandRoot += "OUTP";
    if (currentCommand.startsWith("OUTP:MOVE")) {
      argument = currentCommand.substring(sizeof("OUTP:MOVE ") - 1).toInt();
      currentCommandId = moveAxis;
    }
    else {
      return false;
    }
  }
  else if (currentCommand.startsWith("CONT")) {
    previousCommandRoot += "CONT";
    if (currentCommand.startsWith("CONT:CONF")) {
      previousCommandRoot += ":CONF";
      if (currentCommand.startsWith("CONT:CONF:MOTR")) {
        previousCommandRoot += ":MOTR";
        if (currentCommand.startsWith("CONT:CONF:MOTR:ON")) {
          currentCommandId = setMotorOn;
        }
        else if (currentCommand.startsWith("CONT:CONF:MOTR:OFF")) {
          currentCommandId = setMotorOff;
        }
        else {
          return false;
        }
      }
      else if (currentCommand.startsWith("CONT:CONF:AXIS")) {
        previousCommandRoot += ":AXIS";
        if (currentCommand.startsWith("CONT:CONF:AXIS:HOME")) {
          currentCommandId = setAxisHome;
        }
        else if (currentCommand.startsWith("CONT:CONF:AXIS:AUHO")) {
          currentCommandId = autoHomeAxis;
        }
        else {
          return false;
        }
      }
      else {
        return false;
      }
    }
    else {
      return true;
    }
  }
  else {
    return false;
  }
  return true;
}

bool SCPI_Handler::ProcessStandardCommand() { 
  if (currentCommand.startsWith("*IDN?")) {
    currentCommandId = Id;
  }
  else if (currentCommand.startsWith("*RST\n")) {
    currentCommandId = Reset;
  }
  else if (currentCommand.startsWith("*TST?\n")) {
    currentCommandId = SelfTest;
  }
  else {
    return false;
  }
  return true;
}

Command SCPI_Handler::GetCurrentCommand() {
  return currentCommandId;
}

int SCPI_Handler::GetCurrentArgument() {
  return argument;
}
