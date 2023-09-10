/*
 * https://www.makerguides.com/ir-receiver-remote-arduino-tutorial/
 */

bool readIr(){
  if (irrecv.decode(&results)){ //if something was read
    prevIrCode = irCode;
    irCode = String(results.command);
    irrecv.resume();
    if(prevIrCode != irCode || millis() - pressTime > 125){ //make sure it isn't reading the same button twice in a row
      Serial.println(irCode);
      return(true);
    }
  pressTime = millis();
  }
  return(false);
}


void obeyIr(){
  if(irCode == "30"){ //on
    displayOn = !displayOn;
    if(displayOn){
      brightOn();
    }else{
      brightOff();
    }
  }else if(irCode == "23" && displayOn){ //vol up
    brightUp();
  }else if(irCode == "22" && displayOn){ //vol down
    brightDown();
  }else if(irCode == "78" && displayOn){ //up
    statusScreen += 1;
    statusScreen = statusScreen % 3;
    drawStatus();
  }else if(irCode == "79" && displayOn){ //down
    statusScreen += 2; //This is the same as subtracting 1 after doing mod. In arduino, mod doesn't work with negative numbers.
    statusScreen = statusScreen % 3;
    drawStatus();
  }
}
