void setup(){
  Serial.begin(115200);
  pinMode(dingerPin, OUTPUT);
  
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print ( "." );
  }
  Serial.println("Wifi Connected");
  timeClient.begin();
  rtcObject.Begin();     //Starts rtc
  if(!rtcObject.GetIsRunning() || !rtcObject.IsDateTimeValid()){
    Serial.println("RTC ERROR");
    Serial.println(rtcObject.LastError());
    while(true){
      //infinite loop to stop code execution
    }
  }
  
  getNtpTime(); //so the clock knows roughly what date it is to determine whether or not it's DST
  if((currentMonth >= 4 && currentMonth <= 10) || //between april and october
    (currentMonth == 3 &&  
      ((monthDay > 14) || //past the 14th of march
      (weekDay == "Sunday" && monthDay >= 8) ||
      (weekDay == "Monday" && monthDay >= 9) ||
      (weekDay == "Tuesday" && monthDay >= 10) ||
      (weekDay == "Wednesday" && monthDay >= 11) || 
      (weekDay == "Thursday" && monthDay >= 12) ||
      (weekDay == "Friday" && monthDay >= 13) ||
      (weekDay == "Saturday" && monthDay >= 14))) ||
    
    (currentMonth == 11 && 
      ((weekDay == "Monday" && monthDay <= 1) ||
      (weekDay == "Tuesday" && monthDay <= 2) ||
      (weekDay == "Wednesday" && monthDay <= 3) ||
      (weekDay == "Thursday" && monthDay <= 4) ||
      (weekDay == "Friday" && monthDay <= 5) ||
      (weekDay == "Saturday" && monthDay <= 6))))
  {
    timeClient.setTimeOffset(-25200);
    Serial.println("DST is on");
  }else{
    timeClient.setTimeOffset(-28800);
  }
  getNtpTime();

  matrix.begin(0x70);  //starts display
  irrecv.enableIRIn(); //starts ir
  u8g2.begin(); //whaddya think?
  digitalWrite(dingerPin, LOW); //for some reason u8g2.begin() turns on the dinger pin
  displayOn = true; //turn on clock display (it starts as off)
  brightOn();
  drawStatus();
  
  for(int i=0; i<N_FLYERS; i++) {  // Randomize initial flyer states
    flyer[i].x     = (-32 + random(160)) * 16;
    flyer[i].y     = (-32 + random( 96)) * 16;
    flyer[i].frame = random(3) ? random(4) : 255; // 66% toaster, else toast
    flyer[i].depth = 10 + random(16);             // Speed / stacking order
  }
  qsort(flyer, N_FLYERS, sizeof(struct Flyer), compare); // Sort depths

  u8g2.setBitmapMode(1); //transparent bitmap
}

void loop() {
  RtcDateTime currentTime = rtcObject.GetDateTime();
  
  if(currentTime.Second() != sc){ //happens every second
    if((hr == 0 && mn == 0 && sc == 11) || timeSynced == false){ //Sync NTP time on the eleventh second of every day or if previous time sync failed
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
      if(currentMonth == 3 && weekDay == "Sunday" && monthDay >= 8 && monthDay <= 14){ //Daylight saving starts on the 2nd Sunday of march
        const long utcOffsetInSeconds = -25200;
        timeClient.setTimeOffset(utcOffsetInSeconds);
        RtcDateTime currentTime = RtcDateTime(currentYear-2000, currentMonth, monthDay, hr+1, mn, sc); //Spring forward 1 hour
      }else if (currentMonth == 11 && weekDay == "Sunday" && monthDay <= 7){ //It ends on the 1st sunday of November
        const long utcOffsetInSeconds = -28800;
        timeClient.setTimeOffset(utcOffsetInSeconds);
        RtcDateTime currentTime = RtcDateTime(currentYear-2000, currentMonth, monthDay, hr-1, mn, sc); //Fall back 1 hour
      }
      WiFi.forceSleepWake();
    }
    
    printTime();
    if(!screensaverOn && displayOn){
      updateStatus();
    }
    if(millis() - lastAction > screensaverPeriod && displayOn){
      screensaverOn = true;
    }
  }

  if(readIr()){ //readIr returns true if something new was pressed
    obeyIr();
    lastAction = millis();
    if(displayOn && screensaverOn){
      screensaverOn = false;
      drawStatus();
    }
  }

  if(displayOn && screensaverOn){
    updateToasters();
  }
  delay(1);
}
