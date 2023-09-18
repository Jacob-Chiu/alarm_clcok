/*
 * https://learn.adafruit.com/animated-flying-toaster-oled-jewelry/code
 */

void userInputDetected(){
  lastAction = millis();
  if(displayOn && screensaverOn){
    screensaverOn = false;
    if(menuOn){
      drawMenu();
    }else{
      drawStatus();
    }
  }
}

static int compare(const void *a, const void *b) {
  return ((struct Flyer *)a)->depth - ((struct Flyer *)b)->depth;
}

void initializeToasters(){
  for(int i=0; i<N_FLYERS; i++) {  // Randomize initial flyer states
    flyer[i].x     = (-32 + random(160)) * 16;
    flyer[i].y     = (-32 + random( 96)) * 16;
    flyer[i].frame = random(3) ? random(4) : 255; // 66% toaster, else toast
    flyer[i].depth = 10 + random(16);             // Speed / stacking order
  }
  qsort(flyer, N_FLYERS, sizeof(struct Flyer), compare); // Sort depths
}

void updateToasters(){
  int i, f;
  int x, y;
  boolean resort = false;     // By default, don't re-sort depths
  
  u8g2.sendBuffer();      // Update screen to show current positions
  u8g2.clearBuffer();     // Start drawing next frame

  for(i=0; i<N_FLYERS; i++) { // For each flyer...
    
    // First draw each item...
    f = (flyer[i].frame == 255) ? 4 : (flyer[i].frame++ & 3); // Frame #
    x = flyer[i].x / 16;
    y = flyer[i].y / 16;
    u8g2.setDrawColor(1);
    u8g2.drawXBMP(x,y,32,32,img[f]);
    u8g2.setDrawColor(0);
    u8g2.drawXBMP(x,y,32,32,mask[f]);

    // Then update position, checking if item moved off screen...
    flyer[i].x -= flyer[i].depth * 2; // Update position based on depth,
    flyer[i].y += flyer[i].depth;     // for a sort of pseudo-parallax effect.
    if((flyer[i].y >= (64*16)) || (flyer[i].x <= (-32*16))) { // Off screen?
      if(random(7) < 5) {         // Pick random edge; 0-4 = top
        flyer[i].x = random(160) * 16;
        flyer[i].y = -32         * 16;
      } else {                    // 5-6 = right
        flyer[i].x = 128         * 16;
        flyer[i].y = random(64)  * 16;
      }
      flyer[i].frame = random(3) ? random(4) : 255; // 66% toaster, else toast
      flyer[i].depth = 10 + random(16);
      resort = true;
    }
  }
  // If any items were 'rebooted' to new position, re-sort all depths
  if(resort) qsort(flyer, N_FLYERS, sizeof(struct Flyer), compare);
  delay(toasterDelay);
}
