void readSerialCommands(){
  if(Serial.available()){
    serialCommand = Serial.readStringUntil('\n');
    userInputDetected();
    Serial.print("received Serial command: ");
    Serial.println(serialCommand);
    if(serialCommand.equals("help")){
      Serial.println("help: "); 
      Serial.println(" - 'get time' to print time "); 
      Serial.println(" - 'reset' to reset "); 
      Serial.println(" - 'on' and 'off' to turn clock on and off "); 
      Serial.println(" - 'up', 'down', and 'select' to navigate "); 
      Serial.println(" - 'screensaver on' to turn the screensaver on "); 
      Serial.println(" - type anything to turn screensaver off");
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
        changeStatus(-1);
      }
    }else if(serialCommand.equals("down") && displayOn){
      if(menuOn){
        changeMenuPos(1);
      }else{
        changeStatus(1);
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
    }else if (serialCommand.equals("last sync")){
      serialPrintTime(lastSync);
    }else{
      Serial.println("sorry, that was an invalid command");
    }
  }
}
