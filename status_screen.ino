void drawStatus(){
  u8g2.clearBuffer();
  if(statusScreen == 0){
    u8g2.setDrawColor(1);
    u8g2.drawXBMP(0,0,128,64,CHIU_ENTERPRISES); //draw Chiu Enterprises
  }else if(statusScreen == 1){
    u8g2.setFont(u8g2_font_helvB18_tr);
    u8g2.setFontRefHeightExtendedText();
    u8g2.setDrawColor(1);
    u8g2.setFontPosTop();
    u8g2.setFontDirection(0);
    centerText(String(monthStr(month())).substring(0,3) + String(" ") + String(day()), 34); //display the date
    if(weekday() == 4){
      u8g2.setFont(u8g2_font_helvB14_tr);
    }
    centerText(dayStr(weekday()), 5);
  }else if(statusScreen == 2){
    u8g2.setFont(u8g2_font_inb57_mn);
    u8g2.setFontRefHeightExtendedText();
    u8g2.setDrawColor(1);
    u8g2.setFontPosTop();
    u8g2.setFontDirection(0);
    u8g2.clearBuffer();

    if(second() < 10){
      centerText(String("0") + String(second()), 0);
    }else{
      centerText(String(second()), 0);
    }
  }
  u8g2.sendBuffer();
}

void updateStatus(){
  if((statusScreen == 1 && hour() == 0 && minute() == 0 && second() == 0) || statusScreen == 2){
    drawStatus();
  }
}

void changeStatus(int amount){
  if(amount > 0){
    statusScreen += amount;
  }else{
    statusScreen += (2 * (-1 * amount)); //mod can't handle negative numbers
  }
  statusScreen = statusScreen % 3;
  drawStatus();
}
