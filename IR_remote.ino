/*
 * https://www.makerguides.com/ir-receiver-remote-arduino-tutorial/
 */

bool readIr(){
  if (irrecv.decode(&results)){ //if something was read
    prevIrCode = irCode;
    irCode = String(results.command);
    irrecv.resume();
    if(prevIrCode != irCode || millis() - pressTime > 125){ //make sure it isn't reading the same button twice in a row
      Serial.print("received IR code: ");
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
  }else if(displayOn){
    if(irCode == "23"){ //vol up
      brightUp();
    }else if(irCode == "22"){ //vol down
      brightDown();
    }else if(menuOn){
      if(irCode == "78"){ //up
        changeMenuPos(-1);
      }else if(irCode == "79"){ //down
        changeMenuPos(1);
      }else if(irCode == "5"){//center
        (*obeyArray[currentMenuNumber])();
      }
    }else{
      if(irCode == "78"){ //up
        changeStatus(1);
      }else if(irCode == "79"){ //down
        changeStatus(-1);
      }else if(irCode == "5"){//center
        menuOn = true;
        setMenu("home");
      }
    }
  }
}
