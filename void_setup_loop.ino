void setup(){
  Serial.begin(115200);
  Wire.begin();
  pinMode(dingerPin, OUTPUT);
  initializeClock();
  initializeDisplays();
  initializeToasters();
  initializeEncoder();
  getRtcTime();
  serialPrintTime();
}

void loop() {
  readSerialCommands();
  currentTime = rtcObject.GetDateTime();
  if(currentTime.Second() != sc){ //happens every second
    if((hr == 0 && mn == 0 && sc == 11) || !timeSynced){ //Sync NTP time on the eleventh second of every day or if previous time sync failed
      timeSynced = getNtpTime();
      if(timeSynced){
        WiFi.forceSleepBegin();
      }else{
        getRtcTime();
      }
    }else{
      getRtcTime();
    }

    if(hr == 0 && mn == 0 && sc == 1){ //turn on wifi and adjust for DST on the first second of every day
      dstUpdate();
      WiFi.forceSleepWake();
    }

    if(displayOn){
      if(sc == 0){
        displayTime();
      }
      if(!screensaverOn && !menuOn){
        updateStatus();
      }
    }
    
    if(millis() - lastAction > screensaverPeriod && displayOn && !screensaverOn){ //turn screensaver on
      screensaverOn = true;
    }
  }
  if(screensaverOn){
    readIr(false);
    readEncoder(false);
    updateToasters(); //when displayOn is false, screensaverOn is also always false
  }else{
    readIr(true);
    readEncoder(true);
  }
  delay(1);
}
