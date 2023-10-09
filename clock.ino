/*
 * https://lastminuteengineers.com/esp8266-ntp-server-date-time-tutorial/
 * https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/
 * https://techtutorialsx.com/2016/05/22/esp8266-connection-to-ds3231-rtc/
 */

void initializeClock(){
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print ( "." );
  }
  Serial.println("Wifi Connected");
  timeClient.begin();
  
  rtcObject.Begin();     //Starts rtc
  if (!rtcObject.IsDateTimeValid()){
    if (rtcObject.LastError() != 0){
      Serial.print("RTC communications error = ");
      Serial.println(rtcObject.LastError());
    }else{
      Serial.println("RTC lost confidence in the DateTime! (Check battery.)");
    }
  }
  if (!rtcObject.GetIsRunning()){
    Serial.println("RTC was not actively running, starting now");
    rtcObject.SetIsRunning(true);
  }

  timeClient.update();
  setTime(timeClient.getEpochTime()); //sync NTP time to UTC, to know roughly what the date is

  if((month() >= 4 && month() <= 10) || //between april and october
    (month() == 3 &&  
      ((day() > 14) || //past the 14th of march
      (weekday() == 1 && day() >= 8) ||
      (weekday() == 2 && day() >= 9) ||
      (weekday() == 3 && day() >= 10) ||
      (weekday() == 4 && day() >= 11) || 
      (weekday() == 5 && day() >= 12) ||
      (weekday() == 6 && day() >= 13) ||
      (weekday() == 7 && day() >= 14))) ||
    
    (month() == 11 && 
      ((weekday() == 2 && day() <= 1) ||
      (weekday() == 3 && day() <= 2) ||
      (weekday() == 4 && day() <= 3) ||
      (weekday() == 5 && day() <= 4) ||
      (weekday() == 6 && day() <= 5) ||
      (weekday() == 7 && day() <= 6))))
  {
    timeClient.setTimeOffset(-25200);
    Serial.println("DST is on");
  }else{
    timeClient.setTimeOffset(-28800);
  }
  getNtpTime();
}

void displayTime(){
  matrix.print((hour()*100)+minute(), DEC); //Print the time on the display
  matrix.writeDigitRaw(2, 0x02);
  matrix.writeDisplay();
}

void serialPrintTime(time_t epoch = now()){
  Serial.print(dayStr(weekday(epoch)));
  Serial.print(", ");
  
  Serial.print(year());
  Serial.print("-");
  
  if(month()<10){ //print 10:06 instead of 10:6
    Serial.print(0);
  }
  Serial.print(month(epoch));
  Serial.print("-");
  
  if(day()<10){ //print 10:06 instead of 10:6
    Serial.print(0);
  }
  Serial.print(day(epoch));
  Serial.print(", ");

  Serial.print(hour(epoch));
  Serial.print(":");
  
  if(minute(epoch)<10){ //print 10:06 instead of 10:6
    Serial.print(0);
  }
  Serial.print(minute(epoch));
  Serial.print(":");
  
  if(second()<10){ //see above
    Serial.print(0);
  }
  Serial.print(second(epoch));
  
  if(isAM(epoch)){ //print am/pm
    Serial.println(" AM");
  }else{
    Serial.println(" PM");
  }
}

void getRtcTime(){
  currentTime = rtcObject.GetDateTime();
  setTime(currentTime.Hour(),currentTime.Minute(),currentTime.Second(),currentTime.Day(),currentTime.Month(),currentTime.Year());
}

void getNtpTime(){
  if(WiFi.status() == WL_CONNECTED){
    timeClient.update();
    setTime(timeClient.getEpochTime());
    currentTime = RtcDateTime(now()); //define date and time object from year to second
    rtcObject.SetDateTime(currentTime); //configure the RTC with object
    Serial.print("RTC time synced to ");
    serialPrintTime();
    lastSync = now();
    WiFi.forceSleepBegin();
    timeSynced = true;
  }else{
    Serial.println("Wifi was not connected, getting RTC time instead");
    getRtcTime();
    timeSynced = false;
  }
}

void dstUpdate(){
  if(month() == 3 && weekday() == 1 && day() >= 8 && day() <= 14){ //Daylight saving starts on the 2nd Sunday of march
    timeClient.setTimeOffset(-25200);
    adjustTime(60*60);
  }else if (month() == 11 && weekday() == 1 && day() <= 7){ //It ends on the 1st sunday of November
    timeClient.setTimeOffset(-28800);
    adjustTime(-60*60);
  }else{
    return; //exits function immediately
  }
  Serial.print("Daylight savings time adjusted to ");
  serialPrintTime();
  currentTime = RtcDateTime(now());
  rtcObject.SetDateTime(currentTime);
}
