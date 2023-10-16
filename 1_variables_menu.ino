const byte NUM_MENUS = 8;
const byte NUM_MENU_ITEMS = 12;

String menus[NUM_MENUS][NUM_MENU_ITEMS] = {
  {"Back", "Alarm", "Timer", "Control"},
  {"Back", "Set", "Manage"},
  {"Back", "Year", "Month", "Day", "Time", "Repeat", "Message", "test", "test II", "etc"},
  {"Back", "Year", "Month", "Day", "Time", "Repeat", "Message"},
  {"Back", "Set", "Manage"},
  {"Back", "Year", "Month", "Day", "Time", "Repeat", "Message"},
  {"Back", "Year", "Month", "Day", "Time", "Repeat", "Message"},
  {"Back", "Off", "Screensaver", "Reset", "Brightness"}
};

String menuNames[NUM_MENUS] = {"home", "timer", "timerSet", "timerManage", "alarm", "alarmSet", "alarmManage", "control"};

String inArrow = "→";
String backArrow = "↶";
String nothing = "";

void* right[NUM_MENUS][NUM_MENU_ITEMS] = {
  {&backArrow, &inArrow, &inArrow, &inArrow},
  {&backArrow, &inArrow, &inArrow},
  {&backArrow, &nothing, &nothing, &nothing, &nothing, &nothing, &nothing, &nothing, &nothing, &nothing},
  {&backArrow, &nothing, &nothing, &nothing, &nothing, &nothing, &nothing},
  {&backArrow, &inArrow, &inArrow},
  {&backArrow,  &nothing, &nothing, &nothing, &nothing, &nothing, &nothing},
  {&backArrow,  &nothing, &nothing, &nothing, &nothing, &nothing, &nothing},
  {&backArrow, &nothing, &nothing, &nothing, &bright}
};

int rightType[NUM_MENUS][NUM_MENU_ITEMS] = {
  {0,0,0,0},
  {0,0,0},
  {0,1,1,1,1,1,1,1,1,1},
  {0,1,1,1,1,1,1},
  {0,0,0},
  {0,1,1,1,1,1,1},
  {0,1,1,1,1,1,1},
  {0,1,1,1,2}
};

void obeyHome(){
  switch(currentSelect){
    case 0:
      menuOn = false;
      drawStatus();
      break;
    case 1:
      setMenu("alarm");
      break;
    case 2:
      setMenu("timer");
      break;
    case 3:
      setMenu("control");
      break;
  }
}
void obeyTimer(){
  switch(currentSelect){
    case 0:
      setMenu("home");
      break;
    case 1:
      setMenu("timerSet");
      break;
    case 2:
      setMenu("timerManage");
      break;
  }
}
void obeyTimerSet(){
  switch(currentSelect){
    case 0:
      setMenu("timer");
      break;
    case 1:
      Serial.println("Year selected");
      break;
    case 2:
      Serial.println("Month selected");
      break;
    case 3:
      Serial.println("Day selected");
      break;
    case 4:
      Serial.println("Time selected");
      break;
    case 5:
      Serial.println("Repeat selected");
      break;
    case 6:
      Serial.println("Message selected");
      break;
    case 7:
      Serial.println("test selected");
      break;
    case 8:
      Serial.println("test II selected");
      break;
    case 9:
      Serial.println("etc selected");
      break;
  }
}
void obeyTimerManage(){
  switch(currentSelect){
   case 0:
      setMenu("timer");
      break;
    case 1:
      Serial.println("Year selected");
      break;
    case 2:
      Serial.println("Month selected");
      break;
    case 3:
      Serial.println("Day selected");
      break;
    case 4:
      Serial.println("Time selected");
      break;
    case 5:
      Serial.println("Repeat selected");
      break;
    case 6:
      Serial.println("Message selected");
      break;
  }
}
void obeyAlarm(){
  switch(currentSelect){
    case 0:
      setMenu("home");
      break;
    case 1:
      setMenu("alarmSet");
      break;
    case 2:
      setMenu("alarmManage");
      break;
  }
}
void obeyAlarmSet(){
  switch(currentSelect){
    case 0:
      setMenu("alarm");
      break;
    case 1:
      Serial.println("Year selected");
      break;
    case 2:
      Serial.println("Month selected");
      break;
    case 3:
      Serial.println("Day selected");
      break;
    case 4:
      Serial.println("Time selected");
      break;
    case 5:
      Serial.println("Repeat selected");
      break;
    case 6:
      Serial.println("Message selected");
      break;
  }
}
void obeyAlarmManage(){
  switch(currentSelect){
    case 0:
      setMenu("alarm");
      break;
    case 1:
      Serial.println("Year selected");
      break;
    case 2:
      Serial.println("Month selected");
      break;
    case 3:
      Serial.println("Day selected");
      break;
    case 4:
      Serial.println("Time selected");
      break;
    case 5:
      Serial.println("Repeat selected");
      break;
    case 6:
      Serial.println("Message selected");
      break;
  }
}
void obeyControl(){
  switch(currentSelect){
    case 0:
      setMenu("home");
      break;
    case 1:
      brightOff();
      break;
    case 2:
      screensaverOn = true;
      break;
    case 3: 
      Serial.println("resetting...");
      ESP.reset();
      break;
  }
}

void (*obeyArray[NUM_MENUS])() = {obeyHome, obeyTimer, obeyTimerSet, obeyTimerManage, obeyAlarm, obeyAlarmSet, obeyAlarmManage, obeyControl};
