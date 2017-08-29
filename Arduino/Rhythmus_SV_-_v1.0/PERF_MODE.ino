/* ===================================================================== *
 *                                                                       *
 * PERFORMANCE MODE                                                      *
 *                                                                       *
 * ===================================================================== */

// *********************************************************************
// PERFORMANCE MODE MAIN FUNCTION
// *********************************************************************
  void LCDML_DISP_setup(LCDML_FUNC_perf)
  {   
    bpm         = 120;                                                            // Initialise bpm;
    wholeBeat   = minute/bpm - pulseTime;
    quarterBeat = ((minute/bpm)/4) - pulseTime;
    halfBeat    = ((minute/bpm)/2) - pulseTime;
    oneThree    = ((minute/bpm)/3) - pulseTime;
    twoThree    = ((minute/bpm)/6) - pulseTime;
    threeThree  = ((minute/bpm)/12) - pulseTime;
    
    lcd.setCursor(2, 0);
    lcd.print("Performance Mode");
    lcd.setCursor(6, 1);
    lcd.print("Enabled");
    
    lcd.setCursor(0, 3);
    lcd.write(_LCDML_DISP_cfg_cursor);
    
    lcd.setCursor(1, 3);
    lcd.print("Tempo:");
    
    lcd.setCursor(7, 3);
    lcd.print(bpm);
  
    lcd.setCursor(12, 3);
    lcd.print("Vol:");
  
    mappedGain = map(masterGain, 0, 127, -70, 0);
    lcd.setCursor(16, 3);
    lcd.print(mappedGain);
  
    
    pinMode(startStopPin, INPUT_PULLUP);                                          // INPUT_PULLUP to ignore noise
    
    
    for (int i = 0; i < noOfChannels; i++) {pinMode(CvInPins[i], INPUT_PULLUP);}; // Initialise CV In pins
  
    
    //  for (int i = 0; i < noOfChannels; i++) 
    //  {pinMode(controlPin[i], OUTPUT); digitalWrite(controlPin[i], LOW);};      // Initialise multiplexer control pins
    
    for (int i = 0; i < sizeof(CVData); i++) {CVData[i] = 0x00;}                  // Initialise serial comms
    CVData[0]   = play_mode;
  
    LCDML_DISP_triggerMenu(1);                                                    // Set loop speed
  };
  
  void LCDML_DISP_loop(LCDML_FUNC_perf)
  {
    MasterSet();
  
    unsigned char startStop = digitalRead(startStopPin);
  
  //  if (startStop > 0)
  //  { 
      cvOut1.Update(wholeBeat);
      cvOut2.Update(quarterBeat);
      cvOut3.Update(halfBeat);
      cvOut4.Update(oneThree);
      cvOut5.Update(twoThree);
      
      cvOut6.Update(wholeBeat);
      cvOut7.Update(wholeBeat);
      cvOut8.Update(wholeBeat);
      cvOut9.Update(wholeBeat);
      cvOut10.Update(wholeBeat);
      cvOut11.Update(wholeBeat);
      cvOut12.Update(wholeBeat);
      cvOut13.Update(wholeBeat);
      cvOut14.Update(wholeBeat);
      cvOut15.Update(wholeBeat);
      cvOut16.Update(wholeBeat);
  //   };
    
    cvRead();
    
    ChannelGain();
  //    MuteSwitch();
  };
  
  void LCDML_DISP_loop_end(LCDML_FUNC_perf)
  {
    for (int i = 0; i < 4; i++) {digitalWrite(CvOutPins[i], LOW);};
  };
