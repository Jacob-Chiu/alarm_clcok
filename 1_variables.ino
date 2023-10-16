//pins
const byte DINGER_PIN = 16;
const byte LCD_BACKLIGHT_PIN = 12;
const byte IR_RECEIVE_PIN = 2;
const byte ENCODER_SW_PIN = 3;

//IR
decode_results results;
IRrecv irrecv(IR_RECEIVE_PIN);
unsigned long pressTime = 0; 
String irCode = "x";
String prevIrCode;

//clock
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

RtcDS3231<TwoWire> rtcObject(Wire);
RtcDateTime currentTime;

bool timeSynced = true; //if timesync by ntp succeeded
time_t lastSync = 0;

//displays
Adafruit_7segment matrix = Adafruit_7segment();
U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, /* CS=*/ 15);
bool displayOn = true;
int bright = 10;
const byte CLOCK_BRIGHT_LEVEL[11] = {0, 1, 2, 3, 4, 5, 6, 7, 9, 12, 15};
const byte LCD_BRIGHT_LEVEL[11] = {22, 46, 69, 92, 115, 139, 162, 185, 208, 232, 255};

//toasters
const byte NUM_TOASTERS = 5; // Number of flying things
struct Toaster {       // Array of flying things
  int x, y;      // Top-left position * 16 (for subpixel pos updates)
  int  depth;     // Stacking order is also speed, 12-24 subpixels/frame
  byte frame;     // Animation frame; Toasters cycle 0-3, Toast=255
} toaster[NUM_TOASTERS];
time_t lastAction = 0; //last action epoch time
bool screensaverOn = false;
int screensaverPeriod = 60*5; //five minutes
int toasterDelay = 200; //delay between toaster frames

//status screen
int statusScreen = 0; //0 = Chiu Enterprises, 1 = date, 2 = second

//SerialWombat
SerialWombatChip sw6C;    //Declare a Serial Wombat chip
SerialWombatQuadEnc qeWithPullUps(sw6C); //declare the encoder
int currentSelect = 0;
int currentFirst = 0;
int currentMenuNumber = 0;
int currentMenuLength = 0;
bool encoderSwState = true;
bool prevEncoderSwState = true;
int encoderAmountUp = 0;

bool menuOn = false;

//misc
String serialCommand = "";
