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
}

void displayTime(){
  if(displayOn && sc == 0){
    if(hr == 0){ //convert 24-hour time to 12-hour time
      twelveHr = 12;
    }else if (hr <= 12){
      twelveHr = hr;
    }else{
      twelveHr = hr-12;
    }
    matrix.print((twelveHr*100)+mn, DEC); //Print the time on the display
    matrix.writeDigitRaw(2, 0x02);
    matrix.writeDisplay();
  }
}

void serialPrintTime(){
  Serial.print(weekDay + ", ");
  Serial.print(currentYear);
  Serial.print("-");
  Serial.print(currentMonth);
  Serial.print("-");
  Serial.print(monthDay);
  Serial.print(", ");
  
  if(hr == 0){ //convert 24-hour time to 12-hour time
    twelveHr = 12;
  }else if (hr <= 12){
    twelveHr = hr;
  }else{
    twelveHr = hr-12;
  }

  Serial.print(twelveHr);
  Serial.print(":");
  
  if(mn<10){ //print 10:06 instead of 10:6
    Serial.print(0);
    Serial.print(mn);
  }else{
    Serial.print(mn);
  }  
  
  Serial.print(":");
  
  if(sc<10){ //see above
    Serial.print(0);
    Serial.print(sc);
  }else{
    Serial.print(sc);
  }
  
  if(hr <= 11){ //print am/pm
    Serial.println(" AM");
  }else{
    Serial.println(" PM");
  }
}

void getRtcTime(){
  RtcDateTime currentTime = rtcObject.GetDateTime();
  currentYear = currentTime.Year();
  currentMonth = currentTime.Month();
  monthName = months[currentMonth - 1];
  monthDay = currentTime.Day(); 
  hr = currentTime.Hour();
  mn = currentTime.Minute();
  sc = currentTime.Second();
  setTime(hr,mn,sc,monthDay,currentMonth,currentYear);
  weekDay = daysOfTheWeek[weekday() - 1];
}

bool getNtpTime(){
  if(WiFi.status() == WL_CONNECTED){
    timeClient.update();
    hr = timeClient.getHours();
    mn = timeClient.getMinutes();
    sc = timeClient.getSeconds();
    epochTime = timeClient.getEpochTime();
    monthDay = day(epochTime);
    currentMonth = month(epochTime);
    monthName = months[currentMonth-1];
    currentYear = year(epochTime);
    weekDay = daysOfTheWeek[weekday(epochTime)- 1];
    
    RtcDateTime currentTime = RtcDateTime(currentYear-2000, currentMonth, monthDay, hr, mn, sc); //define date and time object from year to second
    rtcObject.SetDateTime(currentTime); //configure the RTC with object
    Serial.print("RTC time synced to ");
    serialPrintTime();
    return true;
  }
  else{
    Serial.println("RTC time sync failed");
    return false;
  }
}

void dstUpdate(){
  if(currentMonth == 3 && weekDay == "Sunday" && monthDay >= 8 && monthDay <= 14){ //Daylight saving starts on the 2nd Sunday of march
    const long utcOffsetInSeconds = -25200;
    timeClient.setTimeOffset(utcOffsetInSeconds);
    RtcDateTime currentTime = RtcDateTime(currentYear-2000, currentMonth, monthDay, hr+1, mn, sc); //Spring forward 1 hour
  }else if (currentMonth == 11 && weekDay == "Sunday" && monthDay <= 7){ //It ends on the 1st sunday of November
    const long utcOffsetInSeconds = -28800;
    timeClient.setTimeOffset(utcOffsetInSeconds);
    RtcDateTime currentTime = RtcDateTime(currentYear-2000, currentMonth, monthDay, hr-1, mn, sc); //Fall back 1 hour
  }
}
