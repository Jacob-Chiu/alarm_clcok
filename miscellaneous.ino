void readSerialCommands(){
  if(Serial.available()){
    serialCommand = Serial.readStringUntil('\n');
    if(serialCommand.equals("help")){
      Serial.println("type 'get time' to print time");
    }else if(serialCommand.equals("get time")){
      serialPrintTime();
    }
  }
}
