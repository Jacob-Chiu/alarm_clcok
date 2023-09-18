/*
 * https://learn.adafruit.com/adafruit-led-backpack/1-2-inch-7-segment-backpack-arduino-wiring-and-setup
 */

void initializeDisplays(){ //called once in void setup
  matrix.begin(0x70);  //starts display
  irrecv.enableIRIn(); //starts ir
  u8g2.begin(); //whaddya think?
  digitalWrite(dingerPin, LOW); //for some reason u8g2.begin() turns on the dinger pin
  displayOn = true; //turn on clock display (it starts as off)
  brightOn();
  drawStatus();
  u8g2.setBitmapMode(1); //transparent bitmap
}

void brightUp(){
  if(bright < 10){
    bright += 1;
    Serial.print("increasing brightness to ");
    Serial.println(bright);
    matrix.setBrightness(clockBrightLevel[bright]);
    analogWrite(lcdBacklightPin, lcdBrightLevel[bright]);
  }
}

void brightDown(){
  if(bright > 0){
    bright -= 1;
    Serial.print("decreasing brightness to ");
    Serial.println(bright);
    matrix.setBrightness(clockBrightLevel[bright]);
    analogWrite(lcdBacklightPin, lcdBrightLevel[bright]);
  }
}

void brightOn(){
  matrix.print((twelveHr*100)+mn, DEC); //Print the time on the display
  matrix.writeDigitRaw(2, 0x02);
  matrix.writeDisplay();
  analogWrite(lcdBacklightPin, lcdBrightLevel[bright]);
  drawStatus();
}

void brightOff(){
  matrix.clear();
  matrix.writeDisplay();
  analogWrite(lcdBacklightPin, 0);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  screensaverOn = false;
}

void centerText(String input, int height){ //draws a String input centered at height height
  int width = u8g2.getUTF8Width(input.c_str());
  u8g2.drawStr((128-width)/2, height, input.c_str());
}
