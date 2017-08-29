// ============================================================
// Example:     LCDML: display liquid crystal
// ============================================================
// Autor:       Nils Feldkämper
// Last update: 08.01.2017
// License:     MIT
// ============================================================
// Descripton:
// This example shows you, how to use this lib with LiquidCrystal
// lib. The LCD object have to create in this tab. In "LCDML_DISO"
// you can edit the layout of the menu. (content, cursor, scrollbar)
//
// When you rewrite this function, you can use every other LCD
// or graphic LCD Lib with this menu.
// ============================================================
/*

 ** This version has been tweaked by Jack Mason, Trinity College Dublin
 Contact: masonja@tcd.ie

 Description:
 The original version has been added to for use with an accompanying Max patch & physical unit.
 It has been completed during an effort to create a new type of drum machine
 as part of the Trinity Music & Media Technology Masters program.

*/

// *********************************************************************
// Libraries
// *********************************************************************

  #include <LCDMenuLib.h>

  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>

  #include <ResponsiveAnalogRead.h>


// *********************************************************************
// Pre-processor definitions
// *********************************************************************

  // lib config
  #define _LCDML_DISP_cfg_button_press_time          200    // button press time in ms
  #define _LCDML_DISP_cfg_scrollbar                  1      // enable a scroll bar
  #define _LCDML_DISP_cfg_cursor                     0x7E   // cursor Symbol

  #define Sprint(a)      (Serial.print(a))
  #define Sprintln(a)    (Serial.println(a))
  #define Sprintl        (Serial.println())


// *********************************************************************
// LCDML TYPE SELECT
// *********************************************************************
  // settings for lcd
  #define _LCDML_DISP_cols             20
  #define _LCDML_DISP_rows             4

  // lcd object
  LiquidCrystal_I2C lcd(0x27, 16, 4);

  const uint8_t scroll_bar[6][8] =
  {
    {B10001, B10001, B10001, B10001, B10001, B10001, B10001, B10001}, // scrollbar top
    {B11111, B11111, B10001, B10001, B10001, B10001, B10001, B10001}, // scroll state 1
    {B10001, B10001, B11111, B11111, B10001, B10001, B10001, B10001}, // scroll state 2
    {B10001, B10001, B10001, B10001, B11111, B11111, B10001, B10001}, // scroll state 3
    {B10001, B10001, B10001, B10001, B10001, B10001, B11111, B11111}, // scrollbar bottom
    {B00100, B00100, B00100, B00100, B10101, B01110, B00100, B00000}  // downwards arrow
  };


// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************
  // create menu
  // menu element count - last element id
  // this value must be the same as the last menu element

  #define _LCDML_DISP_cnt    12

  // LCDMenuLib_add(id, group, prev_layer_element, new_element_num, lang_char_array, callback_function)
  LCDML_DISP_init(_LCDML_DISP_cnt);
  LCDML_DISP_add      (0  , _LCDML_G1  , LCDML_root          , 1  , "Performance Mode"  , LCDML_FUNC_perf);
  LCDML_DISP_add      (1  , _LCDML_G1  , LCDML_root          , 2  , "Edit Mode"         , LCDML_FUNC);
  LCDML_DISP_add      (2  , _LCDML_G1  , LCDML_root_2        , 1  , "New Kit"           , LCDML_FUNC);
  LCDML_DISP_add      (3  , _LCDML_G1  , LCDML_root_2        , 2  , "Load Kit"          , LCDML_FUNC_load);
  LCDML_DISP_add      (4  , _LCDML_G1  , LCDML_root_2        , 3  , "Edit Kit"          , LCDML_FUNC_edit);
  LCDML_DISP_add      (5  , _LCDML_G1  , LCDML_root_2        , 4  , "Save Kit"          , LCDML_FUNC_save);
  LCDML_DISP_add      (6  , _LCDML_G1  , LCDML_root_2_1      , 1  , "Search"            , LCDML_FUNC_search);
  LCDML_DISP_add      (7  , _LCDML_G1  , LCDML_root_2_1      , 2  , "Play Sample"       , LCDML_FUNC_play_sample);
  LCDML_DISP_add      (8  , _LCDML_G1  , LCDML_root_2_1      , 3  , "Get Similar"       , LCDML_FUNC);
  LCDML_DISP_add      (9  , _LCDML_G1  , LCDML_root_2_1_3    , 1  , "Random"            , LCDML_FUNC_get_random);
  LCDML_DISP_add      (10 , _LCDML_G1  , LCDML_root_2_1_3    , 2  , "Lowlevel"          , LCDML_FUNC_get_lowlevel);
  LCDML_DISP_add      (11 , _LCDML_G1  , LCDML_root_2_1_3    , 3  , "Rhythm"            , LCDML_FUNC_get_rhythm);
  LCDML_DISP_add      (12 , _LCDML_G1  , LCDML_root_2_1_3    , 4  , "Tonal"             , LCDML_FUNC_get_tonal);

  LCDML_DISP_createMenu(_LCDML_DISP_cnt);


// *********************************************************************
// LCDML BACKEND (core of the menu, do not change anything here yet)
// *********************************************************************
  // define backend function
  #define _LCDML_BACK_cnt    1  // last backend function id

  LCDML_BACK_init(_LCDML_BACK_cnt);
  LCDML_BACK_new_timebased_dynamic (0  , ( 20UL )         , _LCDML_start  , LCDML_BACKEND_control);
  LCDML_BACK_new_timebased_dynamic (1  , ( 1000UL )       , _LCDML_stop   , LCDML_BACKEND_menu);
  LCDML_BACK_create();


// *********************************************************************
// Master Control Variables
// *********************************************************************

  boolean   tempoGainSwitch = false;    // Switch for rotary in performance mode

  const int noOfReadings = 10,          // Data smoothing variable
            noOfChannels = 10;


// *********************************************************************
// Start/Stop Variables
// *********************************************************************

  const int startStopPin    = 9;      // Pin number for sequencer on/off switch
  boolean   startStopSwitch = false;  // Switch to tell if sequencer started or not


// *********************************************************************
// Master Gain
// *********************************************************************

  int masterGain = 100,
      mappedGain,
      LastMasterGain;


// *********************************************************************
// Timing Settings
// *********************************************************************

  const int minute = 60000,
            pulseTime = 25;
  int       bpm;

  double    wholeBeat,
            quarterBeat,
            halfBeat,
            oneThree,
            twoThree,
            threeThree;


// *********************************************************************
// CV Settings
// *********************************************************************

  class CVO
  {
    int    CVPin;                  // the number of the pin
    double OnTime;                 // milliseconds of on-time
    double OffTime;                // milliseconds of off-time

    double startTime;
    int    lastState;
    double previousMillis;         // will store last time updated

    public:
    CVO(int pin, double on, double off)
    {
      startTime = millis();
      CVPin = pin;
      pinMode(CVPin, OUTPUT);
      digitalWrite(CVPin, LOW);

      OnTime = on;
      OffTime = off;

      lastState = LOW;
      previousMillis = 0;
    }

    void Update(double OffTime)
    {
      double currentMillis = millis();

      if((lastState == HIGH) && (currentMillis - previousMillis >= OnTime))
      {
        lastState = LOW;                 // Turn it off
        previousMillis += OnTime;        // Remember the time
        digitalWrite(CVPin, lastState);  // Update
      }
      else if ((lastState == LOW) && (currentMillis - previousMillis >= OffTime))
      {
        lastState = HIGH;                // turn it on
        previousMillis += OffTime;       // Remember the time
        digitalWrite(CVPin, lastState);  // Update
      }
    }
  };

  // CV In Pins
  const int cvIn1   = 22,
            cvIn2   = 24,
            cvIn3   = 26,
            cvIn4   = 28,
            cvIn5   = 30,
            cvIn6   = 32,
            cvIn7   = 34,
            cvIn8   = 36,
            cvIn9   = 38,
            cvIn10  = 40;

  int     CvInPins[10] = {cvIn1, cvIn2, cvIn3, cvIn4, cvIn5, cvIn6, cvIn7, cvIn8, cvIn9, cvIn10};

  // Array to memorise CV In last state
  boolean CvStates[10] = {false, false, false, false, false, false, false, false, false, false};


  // CV Out Pins
  const int cv1Out  = 23,
            cv2Out  = 25,
            cv3Out  = 27,
            cv4Out  = 29,
            cv5Out  = 31,
            cv6Out  = 33,
            cv7Out  = 35,
            cv8Out  = 37,
            cv9Out  = 39,
            cv10Out = 41,
            cv11Out = 43,
            cv12Out = 45,
            cv13Out = 47,
            cv14Out = 49,
            cv15Out = 51,
            cv16Out = 53;

  int CvOutPins[16] = {cv1Out, cv2Out, cv3Out, cv4Out, cv5Out, cv6Out, cv7Out, cv8Out, cv9Out, cv10Out, cv11Out, cv12Out, cv13Out, cv14Out, cv15Out, cv16Out};

  // CV Out Initialisation
  CVO cvOut1(cv1Out, pulseTime, 175),
      cvOut2(cv2Out, pulseTime, 175),
      cvOut3(cv3Out, pulseTime, 175),
      cvOut4(cv4Out, pulseTime, 175),
      cvOut5(cv5Out, pulseTime, 175),
      cvOut6(cv6Out, pulseTime, 175),
      cvOut7(cv7Out, pulseTime, 175),
      cvOut8(cv8Out, pulseTime, 175),
      cvOut9(cv9Out, pulseTime, 175),
      cvOut10(cv10Out, pulseTime, 175),
      cvOut11(cv11Out, pulseTime, 175),
      cvOut12(cv12Out, pulseTime, 175),
      cvOut13(cv13Out, pulseTime, 175),
      cvOut14(cv14Out, pulseTime, 175),
      cvOut15(cv15Out, pulseTime, 175),
      cvOut16(cv16Out, pulseTime, 175);


// *********************************************************************
// Multiplexer Variables
// *********************************************************************

// Multiplexer broken at the moment...

// Multiplexer control pins
//int s0              = 4;
//int s1              = 5;
//int s2              = 6;
//int s3              = 7;
//
//// Multiplexer "SIG" in pin
//const int SIG_pin     = 11;
//
//int controlPin[]      = {s0, s1, s2, s3};
//
//int muxChannel[16][4] =
//{
//  {0,0,0,0}, //channel 0
//  {1,0,0,0}, //channel 1
//  {0,1,0,0}, //channel 2
//  {1,1,0,0}, //channel 3
//  {0,0,1,0}, //channel 4
//  {1,0,1,0}, //channel 5
//  {0,1,1,0}, //channel 6
//  {1,1,1,0}, //channel 7
//  {0,0,0,1}, //channel 8
//  {1,0,0,1}, //channel 9
//  {0,1,0,1}, //channel 10
//  {1,1,0,1}, //channel 11
//  {0,0,1,1}, //channel 12
//  {1,0,1,1}, //channel 13
//  {0,1,1,1}, //channel 14
//  {1,1,1,1}  //channel 15
//};


// *********************************************************************
// Channel Variabes
// *********************************************************************

  const int ANALOG_PIN1  = A0,
            ANALOG_PIN2  = A1,
            ANALOG_PIN3  = A2,
            ANALOG_PIN4  = A3,
            ANALOG_PIN5  = A4,
            ANALOG_PIN6  = A5,
            ANALOG_PIN7  = A6,
            ANALOG_PIN8  = A7,
            ANALOG_PIN9  = A8,
            ANALOG_PIN10 = A9;

  ResponsiveAnalogRead analog1(ANALOG_PIN1,   true),
                       analog2(ANALOG_PIN2,   true),
                       analog3(ANALOG_PIN3,   true),
                       analog4(ANALOG_PIN4,   true),
                       analog5(ANALOG_PIN5,   true),
                       analog6(ANALOG_PIN6,   true),
                       analog7(ANALOG_PIN7,   true),
                       analog8(ANALOG_PIN8,   true),
                       analog9(ANALOG_PIN9,   true),
                       analog10(ANALOG_PIN10, true);

  ResponsiveAnalogRead GainFaders[10]       = {analog1, analog2, analog3, analog4, analog5, analog6, analog7, analog8, analog9, analog10};
  int                  GainFaderValues[10]  = {};


// *********************************************************************
// Serial Communication
// *********************************************************************

  // {Mode, Control, Channel, Value}

  // Mode Bytes
  const byte play_mode     = 0x00,
             search_mode   = 0x01,
             load_mode     = 0x02,
             edit_mode     = 0x03,
             save_mode     = 0x04,
             get_spec      = 0x05;

  // Command Bytes
  const byte cv_in         = 0x00,
             gain          = 0x01;
  //         mute          = 0x00;

  // Channel/Value Bytes
  byte channel,
       value,
       master_channel = 0x00,
       lowlevel       = 0x00,
       rhythm         = 0x01,
       tonal          = 0x02;

  const byte bang          = 0x01;

  // Mode Arrays
  uint8_t CVData[4]        = {},
          GainData[4]      = {},
          LoadSaveData[4]  = {},
          SearchChars[32]  = {},
          SpectralProf[4]  = {};

  int     incomingByte     = 0;                 // for incoming serial data

  void PrintHex8(uint8_t *data, uint8_t length) // prints 8-bit data in hex with leading zeroes -  https://forum.arduino.cc/index.php?topic=38107.0
  {
   char tmp[16];
   for (int i = 0; i < length; i++)
   {
    sprintf(tmp, "0x%.2X", data[i]);
    Sprint(tmp);
   }
    Sprintl;
  }


// *********************************************************************
// Performance Mode Variabes
// *********************************************************************

  int    lastState      = LOW;
  double previousMillis = 0;


// *********************************************************************
// SAMPLE SEARCH VARIABLES
// *********************************************************************

  String lcdSearchTerm = "";

  boolean lookForReturn = false,
          charSwitch,
          playSample    = false,
          getSimilar    = false,
          getLowlevel   = false,
          getRhythm     = false,
          getTonal      = false,
          commandSwitch = false;

  int arrowX,
      rowX,
      searchTermPos,
      searchCount;

  const int rowMax = 20,
            rowMin = 0;


// *********************************************************************
// LOAD/SAVE KIT VARIABLES
// *********************************************************************

  uint8_t g_button_value = 0; // button value counter (global variable)
  int     ctrlMode;               // Control mode so Max can differentiate between Save/Load modes - Must be 2 or 4


// *********************************************************************
// SETUP
// *********************************************************************

  void setup()
  {
    while(!Serial);                                // wait until serial ready
    Serial.begin(115200);

    lcd.begin();                                  // LCD Begin

    // set special chars for scrollbar
    lcd.createChar(0, (uint8_t*)scroll_bar[0]);
    lcd.createChar(1, (uint8_t*)scroll_bar[1]);
    lcd.createChar(2, (uint8_t*)scroll_bar[2]);
    lcd.createChar(3, (uint8_t*)scroll_bar[3]);
    lcd.createChar(4, (uint8_t*)scroll_bar[4]);
    lcd.createChar(5, (uint8_t*)scroll_bar[5]);

    // You can create different menu trees
    // Enable all items with _LCDML_G1
    LCDML_DISP_groupEnable(_LCDML_G1);            // enable group 1

    // LCDMenu Setup
    LCDML_setup(_LCDML_BACK_cnt);
  };

// *********************************************************************
// LOOP
// *********************************************************************
  void loop()
  {
    LCDML_run(_LCDML_priority);   // this function must called here, do not delete it
  };

// *********************************************************************
// check some errors - do not change here anything
// *********************************************************************
# if(_LCDML_DISP_rows > _LCDML_DISP_cfg_max_rows)
# error change value of _LCDML_DISP_cfg_max_rows in LCDMenuLib.h
# endif
# if(_LCDML_DISP_cols > _LCDML_DISP_cfg_max_string_length)
# error change value of _LCDML_DISP_cfg_max_string_length in LCDMenuLib.h
# endif
