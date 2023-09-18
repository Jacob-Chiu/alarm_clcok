void setup(){
  Serial.begin(115200);
  Wire.begin();
  pinMode(dingerPin, OUTPUT);
  initializeClock();
  initializeDisplays();
  initializeToasters();
  initializeEncoder();
}

void loop() {
  readSerialCommands();
  RtcDateTime currentTime = rtcObject.GetDateTime();
  if(currentTime.Second() != sc){ //happens every second
    displayTime();
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
    
    if(!screensaverOn && displayOn && !menuOn){ //update the status screen
      updateStatus();
    }
    
    if(millis() - lastAction > screensaverPeriod && displayOn && !screensaverOn){ //turn screensaver on
      screensaverOn = true;
    }
  }

  if(readIr()){ //readIr returns true if something new was pressed
    obeyIr();
    userInputDetected();
  }

  if(displayOn && screensaverOn){ //update screensaver
    updateToasters();
  }

  if(displayOn){
    readEncoder();
  }
  delay(1);
}
