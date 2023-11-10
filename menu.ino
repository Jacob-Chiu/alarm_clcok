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
    }else if(millis() - pressTime > 2000 && displayOn){ //if it has been held down > 2sec.
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

void updateVars(){
  if(bright > 10){
    bright = 10;
  }else if(bright < 0){
    bright = 0;
  }
  changeBright(0);
}

void changeMenuPos(int num) {
  if(editVar){
    *(int*)right[currentMenuNumber][currentSelect] += num;
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
