void setup(){
  Serial.begin(115200);
  Wire.begin();
  pinMode(DINGER_PIN, OUTPUT);
  initializeClock();
  initializeDisplays();
  initializeToasters();
  initializeEncoder();
}

void loop() {
  readSerialCommands();
  currentTime = rtcObject.GetDateTime();
  setTime(now()); //Time library normally tries to advance the time on its own using the ESP8266's internal clock, but this "freezes" the time so it does not.
  if(currentTime.Second() != second()){ //happens every second
    if((hour() == 0 && minute() == 0 && second() == 30) || !timeSynced){ //Sync NTP time on the 30th second of every day or if previous time sync failed
      getNtpTime();
    }else{
      getRtcTime();
    }

    if(hour() == 0 && minute() == 0 && second() == 1){ //turn on wifi and adjust for DST on the first second of every day
      dstUpdate();
      WiFi.forceSleepWake();
    }

    if(displayOn){
      if(second() == 0){
        displayTime();
      }
      if(!screensaverOn && !menuOn){
        updateStatus();
      }
    }
    
    if(now() - lastAction > SCREENSAVER_PERIOD && displayOn && !screensaverOn){ //turn screensaver on
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
