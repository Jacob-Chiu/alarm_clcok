//SerialWombat documentation: https://broadwellconsultinginc.github.io/SerialWombatArdLib/class_serial_wombat_quad_enc.html

void initializeEncoder(){
  sw6C.begin(Wire, 0x6C); //Initialize the Serial Wombat library to use the primary I2C port, SerialWombat is address 6C
  qeWithPullUps.begin(1,2);  // Initialize a QE on pins 1 and 2
  qeWithPullUps.write(32768); //sets current position to 10
  sw6C.pinMode(ENCODER_SW_PIN, INPUT_PULLUP);
}

void readEncoder(bool act){
  encoderAmountUp = (qeWithPullUps.read() - 32768)/2;
  if(encoderAmountUp != 0){//see if encoder was twisted
    qeWithPullUps.write(32768);
    userInputDetected();
    if(act){
      if(!displayOn){
        brightOn();
      }else if(menuOn){
        changeMenuPos(encoderAmountUp);
      }else{
        changeStatus(encoderAmountUp);
      }
    }
  }

  prevEncoderSwState = encoderSwState;
  encoderSwState = sw6C.digitalRead(ENCODER_SW_PIN);
  
  if(!encoderSwState){ //if encoderSwState is false, the button has been pushed
    if(prevEncoderSwState){ //when it was first pressed
      pressTime = millis();
    }else if(millis() - pressTime > 1500 && displayOn){ //if it has been held down > 1.5sec.
      brightOff();
    }
  }else if(encoderSwState && !prevEncoderSwState && millis() - pressTime < 2000){ //button was released after short press
    userInputDetected();
    if(!displayOn){
      brightOn();
    }else if(act){ //short press
      if(menuOn){
        (*obeyArray[currentMenuNumber])();
      }else{
        menuOn = true;
        setMenu("home");
      }
    }
  }
}

void drawSelector(int x) { //does not send buffer
  u8g2.setDrawColor(2);
  u8g2.drawBox(0, 10 * x, 128, 10);
}

void setMenu(const String menuName){
  currentMenuNumber = menuNumber(menuName);
  currentMenuLength = menuLength(menus[currentMenuNumber]);
  currentSelect = 0;
  currentFirst = 0;
  drawMenu();
}

int menuLength(const String menuArray[]){
  int count = 0;
  while(true){
    if(menuArray[count] == ""){
      return(count);
    }
    count++;
  }
}

int menuNumber(const String menuName){
  int count = 0;
  while(true){
    if(menuNames[count] == menuName){
      return(count);
    }
    count++;
  }
}

String getRight(int item) {
  switch(rightType[currentMenuNumber][item]){
    case 0:
      return(*(String*)right[currentMenuNumber][item]); //type String symbol
      break;
    case 1:
      return(*(String*)right[currentMenuNumber][item]); //type String, normal text
      break;
    case 2:
      return(String(*(int*)right[currentMenuNumber][item])); //type int
      break;
    case 3: //type byte
      return(String(*(byte*)right[currentMenuNumber][item]));
      break;
    case 4: //type boolean
      if(*(bool*)right[currentMenuNumber][item] == true){
        return(String("true"));
      }else{
        return(String("false"));
      }
      break;
    case 5: //type day of week
      return(getDays(*(byte*)right[currentMenuNumber][item]));
      break;
    default:
      return(String("???"));
  }
}

void drawMenu() {
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.clearBuffer();
  if(editVar){
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(5, 5, (menus[currentMenuNumber][currentSelect] + String(": ")).c_str());
    rightText(getRight(currentSelect), 5, 5);
  }else{
    u8g2.setFont(u8g2_font_6x10_tf);
    for(int i = 0; i < 6 && currentFirst + i < currentMenuLength; i++) {
      u8g2.drawStr(1, i * 10, menus[currentMenuNumber][currentFirst + i].c_str());
      if(rightType[currentMenuNumber][currentFirst+i] != 0){
        rightText(getRight(currentFirst + i), i*10, 5);
      }
    }
    u8g2.setFont(u8g2_font_unifont_t_symbols);
    for(int i = 0; i < 6 && currentFirst + i < currentMenuLength; i++){
      if(rightType[currentMenuNumber][currentFirst+i] == 0){
        if(getRight(currentFirst + i) == "↶"){
          u8g2.drawUTF8(115, (i * 10) - 2, "↶");
        }else{
          u8g2.drawUTF8(115, i * 10, getRight(currentFirst+i).c_str());
        }
      }
    }
    drawSelector(currentSelect - currentFirst);
  }
  u8g2.sendBuffer();
}

void constrainVar(byte *b, byte down, byte up){
  if(*b < down || *b >= 200){ //since bytes are unsigned, when you subtract, they loop back (0 - 1 = 255). Any number 200+ is considered negative.
    *b = down;
  }else if(*b > up){
    *b = up;
  }
}

void loopVar(byte *b, byte down, byte up) {
  if(*b >= 200 && *b - 256 < down){ //since bytes are unsigned, when you subtract, they loop back (0-1 = 255). Any number 200+ is considered negative.
    *b = (up - (down - (*b - 256)) % (up - down + 1)) + 1;
  }else if(*b < down){
    *b = (up - (down - *b) % (up - down + 1)) + 1;
  }else if (*b > up){
    *b = down + (*b - down) % (up - down + 1);
  }
}

void updateVars(){ //remember to change this if you modify the menus
  switch(currentMenuNumber){
    case 3: //control
      constrainVar(&bright, 0, 10);
      changeBright(0);
      break;
    case 1: //alarm
      constrainVar(&currentAlarmNum, 0, NUM_ALARMS - 1);
      loopVar(&alarms[currentAlarmNum].hr, 0, 23);
      loopVar(&alarms[currentAlarmNum].mn, 0, 59);
      loopVar(&alarms[currentAlarmNum].sc, 0, 59);
      loopVar(&alarms[currentAlarmNum].weekDays, 0, 127);
      constrainVar(&alarms[currentAlarmNum].monthDayLow, 1, 31);
      constrainVar(&alarms[currentAlarmNum].monthDayHigh, 1, 31);
      break;
    case 2: //alarmMonths
      right[2][1] = &alarms[currentAlarmNum].months[0];
      right[2][2] = &alarms[currentAlarmNum].months[1];
      right[2][3] = &alarms[currentAlarmNum].months[2];
      right[2][4] = &alarms[currentAlarmNum].months[3];
      right[2][5] = &alarms[currentAlarmNum].months[4];
      right[2][6] = &alarms[currentAlarmNum].months[5];
      right[2][7] = &alarms[currentAlarmNum].months[6];
      right[2][8] = &alarms[currentAlarmNum].months[7];
      right[2][9] = &alarms[currentAlarmNum].months[8];
      right[2][10] = &alarms[currentAlarmNum].months[9];
      right[2][11] = &alarms[currentAlarmNum].months[10];
      right[2][12] = &alarms[currentAlarmNum].months[11];
      break;
  }
}

void changeMenuPos(int num) {
  if(editVar){
    switch(rightType[currentMenuNumber][currentSelect]){
      case 2:
        *(int*)right[currentMenuNumber][currentSelect] += num;
        break;
      case 3: //type byte
        *(byte*)right[currentMenuNumber][currentSelect] += num;
        break;
      case 4: //type boolean
        if(abs(num) % 2 == 1){ //mod doesn't handle negative numbers
          *(bool*)right[currentMenuNumber][currentSelect] = !*(bool*)right[currentMenuNumber][currentSelect];
        }
        break;
      case 5: //type day of week
        *(byte*)right[currentMenuNumber][currentSelect] += num;
        break;
      default:
        Serial.println("changeMenuPos was called on something that wasn't an integer, byte, or boolean");
    }
    updateVars();
  }else{
    currentSelect += num;
    if(currentSelect > currentMenuLength - 1){
      currentSelect = currentMenuLength - 1;
    }else if(currentSelect < 0){
      currentSelect = 0;
    }
    if(currentFirst + 5 < currentSelect){
      currentFirst = currentSelect - 5;
    }else if(currentFirst > currentSelect){
      currentFirst = currentSelect;
    }
  }
  drawMenu();
  return;
}
