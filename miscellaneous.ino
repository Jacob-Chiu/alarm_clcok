void readSerialCommands(){
  if(Serial.available()){
    serialCommand = Serial.readStringUntil('\n');
    userInputDetected();
    Serial.print("Received Serial command: ");
    Serial.println(serialCommand);
    if(serialCommand.equals("help")){
      Serial.println("Help: "); 
      Serial.println(" - 'get time' to print time"); 
      Serial.println(" - 'reset' to reset"); 
      Serial.println(" - 'on' to turn clock on or redraw display");
      Serial.println(" - 'off' to turn clock off (display must be on)");
      Serial.println(" - 'bright up' and 'bright down' to change brightness (display must be on)"); 
      Serial.println(" - 'up', 'down', and 'select' to navigate (display must be on)"); 
      Serial.println(" - 'screensaver on' to turn the screensaver on (display must be on)"); 
      Serial.println(" - type anything to turn screensaver off");
      Serial.println(" - 'last sync' to get the last NTP sync");
      Serial.println(" - 'wifi status' to get wifi status");
      Serial.println(" - 'wifi diagnostic' to get wifi diagnostic information");
      Serial.println(" - 'wifi on' and 'wifi off' to wake/sleep wifi");
      Serial.println(" - 'sync time' to get NTP time");
      Serial.println(" - 'print screen' to print to the serial monitor what's on the lcd");
    }else if(serialCommand.equals("get time")){
      serialPrintTime();
    }else if(serialCommand.equals("reset")){
      Serial.println("resetting...");
      ESP.reset();
    }else if(serialCommand.equals("off") && displayOn){
      brightOff();
    }else if(serialCommand.equals("on")){
      brightOn();
    }else if(serialCommand.equals("bright up") && displayOn){
      changeBright(1);
    }else if(serialCommand.equals("bright down") && displayOn){
      changeBright(-1);
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
    }else if(serialCommand.equals("screensaver on") && displayOn){
      screensaverOn = true;
    }else if (serialCommand.equals("last sync")){
      serialPrintTime(lastSync);
    }else if(serialCommand.equals("wifi status")){
      switch(WiFi.status()){
        case 0:
          Serial.println("Idle (changing between statuses)");
          break;
        case 1:
          Serial.println("No SSID available");
          break;
        case 3:
          Serial.println("Connected");
          break;
        case 4:
          Serial.println("Connection failed");
          break;
        case 6:
          Serial.println("Wrong password");
          break;
        case 7: 
          Serial.println("Disconnected");
          break;
        default:
          Serial.print("Didn't recognize status code: ");
          Serial.println(WiFi.status());
          break;
      }
    }else if (serialCommand.equals("wifi diagnostic")){
      WiFi.printDiag(Serial);
    }else if(serialCommand.equals("wifi on")){
      WiFi.forceSleepWake();
    }else if(serialCommand.equals("wifi off")){
      WiFi.forceSleepBegin();
    }else if(serialCommand.equals("sync time")){
      if(WiFi.status() == WL_CONNECTED){ //getNtpTime already checks for this, but if wifi isn't connected, it sets timesynced to false. We don't want that
        getNtpTime();
      }else{
        Serial.println("Wifi is not connected");
      }
    }else if(serialCommand.equals("hello")){
      Serial.println("Hello, how are you?");
    }else if(serialCommand.equals("print screen") && displayOn){
      u8g2.writeBufferPBM2(Serial);
    }else{
      Serial.println("Sorry, that was an invalid command. Type 'help' to get a list of commands. Check if the display is on; some commands are invalid if it's off.");
    }
  }
}
