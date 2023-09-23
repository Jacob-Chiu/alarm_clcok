void readSerialCommands(){
  if(Serial.available()){
    serialCommand = Serial.readStringUntil('\n');
    
    if(serialCommand.equals("help")){
      Serial.println("type 'get time' to print time or 'reset' to reset.");
    }else if(serialCommand.equals("get time")){
      serialPrintTime();
    }else if(serialCommand.equals("reset")){
      Serial.println("resetting...");
      ESP.reset();
    }else{
      Serial.println("sorry, that was an invalid command");
    }
  }
}
