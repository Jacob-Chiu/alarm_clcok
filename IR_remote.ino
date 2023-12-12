/*
 * https://www.makerguides.com/ir-receiver-remote-arduino-tutorial/
 */

void readIr(bool act){
  if (irrecv.decode(&results)){ //if something was read
    prevIrCode = irCode;
    irCode = String(results.command);
    irrecv.resume();
    //if(prevIrCode != irCode || millis() - pressTime > 125){ //make sure it isn't reading the same button twice in a row
    if(prevIrCode != irCode && irCode != 0){ //make sure it isn't reading the same button twice in a row
      if(act){
        Serial.print("received IR code: ");
        Serial.println(irCode);
        obeyIr();
      }
      userInputDetected();
    }
  }
}


void obeyIr(){
  if(irCode == "30"){ //on
    if(!displayOn){
      brightOn();
    }else{
      brightOff();
    }
  }else if(displayOn){
    if(irCode == "23"){ //vol up
      changeBright(1);
    }else if(irCode == "22"){ //vol down
      changeBright(-1);
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
