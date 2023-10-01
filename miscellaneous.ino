void readSerialCommands(){
  if(Serial.available()){
    serialCommand = Serial.readStringUntil('\n');
    userInputDetected();
    if(serialCommand.equals("help")){
      Serial.println("help: \n- 'get time' to print time \n- 'reset' to reset \n- 'on' and 'off' to turn clock on and off \n- 'up', 'down', and 'select' to navigate \n- 'screensaver on' to turn the screensaver on \n- type anything to turn screensaver off");
    }else if(serialCommand.equals("get time")){
      serialPrintTime();
    }else if(serialCommand.equals("reset")){
      Serial.println("resetting...");
      ESP.reset();
    }else if(serialCommand.equals("off")){
      brightOff();
    }else if(serialCommand.equals("on")){
      brightOn();
    }else if(serialCommand.equals("up") && displayOn){
      if(menuOn){
        changeMenuPos(-1);
      }else{
        changeStatus(1);
      }
    }else if(serialCommand.equals("down") && displayOn){
      if(menuOn){
        changeMenuPos(1);
      }else{
        changeStatus(-1);
      }
    }else if(serialCommand.equals("select") && displayOn){
      if(menuOn){
        (*obeyArray[currentMenuNumber])();
      }else{
        menuOn = true;
        setMenu("home");
      }
    }else if(serialCommand.equals("screensaver on")){
      screensaverOn = true;
    }else{
      Serial.println("sorry, that was an invalid command");
    }
  }
}
