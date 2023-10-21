/*
 * https://learn.adafruit.com/animated-flying-toaster-oled-jewelry/code
 */

void userInputDetected(){
  lastAction = now();
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
  return ((struct Toaster *)a)->depth - ((struct Toaster *)b)->depth;
}

void initializeToasters(){
  for(int i=0; i<NUM_TOASTERS; i++) {  // Randomize initial toaster states
    toaster[i].x     = (-32 + random(160)) * 16;
    toaster[i].y     = (-32 + random( 96)) * 16;
    toaster[i].frame = random(3) ? random(4) : 255; // 66% toaster, else toast
    toaster[i].depth = 10 + random(16);             // Speed / stacking order
  }
  qsort(toaster, NUM_TOASTERS, sizeof(struct Toaster), compare); // Sort depths
}

void updateToasters(){
  int i, f;
  int x, y;
  boolean resort = false;     // By default, don't re-sort depths
  
  u8g2.sendBuffer();      // Update screen to show current positions
  u8g2.clearBuffer();     // Start drawing next frame

  for(i=0; i<NUM_TOASTERS; i++) { // For each toaster...
    
    // First draw each item...
    f = (toaster[i].frame == 255) ? 4 : (toaster[i].frame++ & 3); // Frame #
    x = toaster[i].x / 16;
    y = toaster[i].y / 16;
    u8g2.setDrawColor(1);
    u8g2.drawXBMP(x,y,32,32,TOASTER_IMAGE[f]);
    u8g2.setDrawColor(0);
    u8g2.drawXBMP(x,y,32,32,TOASTER_MASK[f]);

    // Then update position, checking if item moved off screen...
    toaster[i].x -= toaster[i].depth * 2; // Update position based on depth,
    toaster[i].y += toaster[i].depth;     // for a sort of pseudo-parallax effect.
    if((toaster[i].y >= (64*16)) || (toaster[i].x <= (-32*16))) { // Off screen?
      if(random(7) < 5) {         // Pick random edge; 0-4 = top
        toaster[i].x = random(160) * 16;
        toaster[i].y = -32         * 16;
      } else {                    // 5-6 = right
        toaster[i].x = 128         * 16;
        toaster[i].y = random(64)  * 16;
      }
      toaster[i].frame = random(3) ? random(4) : 255; // 66% toaster, else toast
      toaster[i].depth = 10 + random(16);
      resort = true;
    }
  }
  // If any items were 'rebooted' to new position, re-sort all depths
  if(resort) qsort(toaster, NUM_TOASTERS, sizeof(struct Toaster), compare);
  delay(TOASTER_DELAY);
}
