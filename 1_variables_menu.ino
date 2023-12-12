const byte NUM_MENUS = 5;
const byte NUM_MENU_ITEMS = 14;

const String menus[NUM_MENUS][NUM_MENU_ITEMS] = {
  {"Back", "Alarm", "Control"},
  {"Back", "Alarm #", "Hour", "Minute", "Second", "Week Days", "Low Day", "High Day", "Months"},
  {"Back", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"},
  {"Back", "Off", "Screensaver", "Reset", "Brightness"},
  {"Back", "Confirm reset"}
};

const String menuNames[NUM_MENUS] = {"home", "alarm", "alarmMonths", "control", "reset"};

String inArrow = "→";
String backArrow = "↶";
String nothing = "";

void* right[NUM_MENUS][NUM_MENU_ITEMS] = {
  {&backArrow, &inArrow, &inArrow},
  {&backArrow, &currentAlarmNum, &alarms[currentAlarmNum].hr, &alarms[currentAlarmNum].mn, &alarms[currentAlarmNum].sc, &alarms[currentAlarmNum].weekDays, 
   &alarms[currentAlarmNum].monthDayLow, &alarms[currentAlarmNum].monthDayHigh, &inArrow}, //this element is periodically redefined if currentAlarmNum changes
  {&backArrow, &alarms[currentAlarmNum].months[0], &alarms[currentAlarmNum].months[1], &alarms[currentAlarmNum].months[2], &alarms[currentAlarmNum].months[3], //this one too
               &alarms[currentAlarmNum].months[4], &alarms[currentAlarmNum].months[5], &alarms[currentAlarmNum].months[6], &alarms[currentAlarmNum].months[7], //part of the previous line
               &alarms[currentAlarmNum].months[8], &alarms[currentAlarmNum].months[9], &alarms[currentAlarmNum].months[10], &alarms[currentAlarmNum].months[11]}, //part of the previous line
  {&backArrow, &nothing, &nothing, &nothing, &bright},
  {&backArrow, &nothing}
};

const int rightType[NUM_MENUS][NUM_MENU_ITEMS] = { //0 = symbol, 1 = string, 2 = integer, 3 = byte, 4 = boolean, 5 = day of week
  {0, 0, 0, 0},
  {0, 3, 3, 3, 3, 5, 3, 3, 0},
  {0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
  {0, 1, 1, 1, 3},
  {0, 1}
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
      setMenu("control");
      break;
  }
}

void obeyAlarm(){
  switch(currentSelect){
    case 0:
      setMenu("home");
      break;
    case 8:
      setMenu("alarmMonths");
      break;
    default:
      editVar = !editVar;
      drawMenu();
      break;
  }
}

void obeyAlarmMonths(){
  switch(currentSelect){
    case 0:
      setMenu("alarm");
      break;
    default:  
      *(bool*)right[currentMenuNumber][currentSelect] = !*(bool*)right[currentMenuNumber][currentSelect]; //it switches when you click
      drawMenu();
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
      setMenu("reset");
      break;
    case 4:
      editVar = !editVar;
      drawMenu();
      break;
  }
}
void obeyReset(){
  switch(currentSelect){
    case 0:
      setMenu("control");
      break;
    case 1:
      Serial.println("resetting...");
      ESP.reset();
      break;
  }
}

void (*obeyArray[NUM_MENUS])() = {obeyHome, obeyAlarm, obeyAlarmMonths, obeyControl, obeyReset};
