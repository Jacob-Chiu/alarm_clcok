//pins
const byte dingerPin = 16;
const byte lcdBacklightPin = 12;
const byte IR_RECEIVE_PIN = 2;
const byte encoderSwPin = 3;

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

int sc; //second
time_t epochTime;
int hr; //hour
int twelveHr; //hour, in 12 hour time. ex. 13:00 = 1:00
int mn; //minute
String weekDay; //Monday, Tuesday, etc.
int monthDay; //day of the month i.e. 12
int currentMonth; //month number
String monthName; //month name
int currentYear; //year

bool timeSynced = false; //if timesync by ntp succeeded

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

//displays
Adafruit_7segment matrix = Adafruit_7segment();
U8G2_ST7920_128X64_F_HW_SPI u8g2(U8G2_R0, /* CS=*/ 15);
bool displayOn = true;
int bright = 10;
const int clockBrightLevel[11] = {0, 1, 2, 3, 4, 5, 6, 7, 9, 12, 15};
const int lcdBrightLevel[11] = {22, 46, 69, 92, 115, 139, 162, 185, 208, 232, 255};

//toasters
#define N_FLYERS   5 // Number of flying things
struct Flyer {       // Array of flying things
  int x, y;      // Top-left position * 16 (for subpixel pos updates)
  int  depth;     // Stacking order is also speed, 12-24 subpixels/frame
  byte frame;     // Animation frame; Toasters cycle 0-3, Toast=255
} flyer[N_FLYERS];
unsigned long lastAction = millis();
bool screensaverOn = false;
unsigned long screensaverPeriod = 5000;
int toasterDelay = 200;

//status screen
int statusScreen = 0; //0 = Chiu Enterprises, 1 = date, 2 = second

//SerialWombat
SerialWombatChip sw6C;    //Declare a Serial Wombat chip
SerialWombatQuadEnc qeWithPullUps(sw6C); //declare the encoder
int currentSelect = 0;
int currentFirst = 0;
int currentMenuNumber = 0;
int currentMenuLength = 0;
bool encoderSwState = false;
bool prevEncoderSwState = false;
int encoderAmountUp = 0;

bool menuOn = false;

//misc
String serialCommand = "";
