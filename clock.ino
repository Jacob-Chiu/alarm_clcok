/*
 * https://lastminuteengineers.com/esp8266-ntp-server-date-time-tutorial/
 * https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/
 * https://techtutorialsx.com/2016/05/22/esp8266-connection-to-ds3231-rtc/
 */

void printTime(){
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

  if(displayOn && sc == 0){
    matrix.print((twelveHr*100)+mn, DEC); //Print the time on the display
    matrix.writeDigitRaw(2, 0x02);
    matrix.writeDisplay();
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
    printTime();
    return true;
  }
  else{
    Serial.println("RTC time sync failed");
    return false;
  }
}
