/*
 * https://learn.adafruit.com/adafruit-led-backpack/1-2-inch-7-segment-backpack-arduino-wiring-and-setup
 */

void initializeDisplays(){ //called once in void setup
  matrix.begin(0x70);  //starts display
  irrecv.enableIRIn(); //starts ir
  u8g2.begin(); //whaddya think?
  digitalWrite(DINGER_PIN, LOW); //for some reason u8g2.begin() turns on the dinger pin
  displayOn = true; //turn on clock display (it starts as off)
  brightOn();
  drawStatus();
  u8g2.setBitmapMode(1); //transparent bitmap
}

void changeBright(int amount){
  bright += amount;
  if(bright > 10){
    bright = 10;
  }else if(bright < 0){
    bright = 0;
  }
  Serial.print("changing brightness to ");
  Serial.println(bright);
  matrix.setBrightness(CLOCK_BRIGHT_LEVEL[bright]);
  analogWrite(LCD_BACKLIGHT_PIN, LCD_BRIGHT_LEVEL[bright]);
}

void brightOn(){
  displayOn = true;
  displayTime();
  analogWrite(LCD_BACKLIGHT_PIN, LCD_BRIGHT_LEVEL[bright]);
  if(menuOn){
    drawMenu();
  }else{
    drawStatus();
  }
}

void brightOff(){
  displayOn = false;
  matrix.clear();
  matrix.writeDisplay();
  analogWrite(LCD_BACKLIGHT_PIN, 0);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  screensaverOn = false;
}

void centerText(String input, int height){ //draws a String input centered at height height
  int width = u8g2.getUTF8Width(input.c_str());
  u8g2.drawUTF8((128-width)/2, height, input.c_str()); //drawUTF8 draws a string encoded in UTF8, and it's basically the same as drawStr except that strlen doesn't work
}

void rightText(String input, int height, int margin) {
  int width = u8g2.getUTF8Width(input.c_str());
  u8g2.drawUTF8(128 - width - margin, height, input.c_str());
}
