/* ===================================================================== *
 *                                                                       *
 * MASTER CONTROL                                                        *
 *                                                                       *
 * ===================================================================== */

// *********************************************************************
// UPDATE BPM FUNCTION
// *********************************************************************
  void bpmUpdate()
  {
    // Subtract pulsetime to compensate for length of pulse
    wholeBeat   = minute/bpm - pulseTime;
    quarterBeat = ((minute/bpm)/4) - pulseTime;
    halfBeat    = ((minute/bpm)/2) - pulseTime;
    oneThree    = ((minute/bpm)/3) - pulseTime;
    twoThree    = ((minute/bpm)/6) - pulseTime;
    threeThree  = ((minute/bpm)/12) - pulseTime;
  
    for (int i = 0; i <= 3; i++)
    {
      lcd.setCursor(i + 7, 3);
      lcd.print(" ");
    }
    
    lcd.setCursor(7, 3);
    lcd.print(bpm);
  }

// *********************************************************************
// UPDATE MASTER GAIN FUNCTION
// *********************************************************************
  void masterGainUpdate()
  {
    GainData[0] = play_mode;
    GainData[1] = gain;
    GainData[2] = master_channel;
    GainData[3] = (byte) masterGain;

    PrintHex8(GainData, 4);
    
    mappedGain = map(masterGain, 0, 127, -70, 0);
    
    for (int i = 0; i <= 3; i++)
    {
      lcd.setCursor(i + 16, 3);
      lcd.print(" ");
    }
    
    lcd.setCursor(16, 3);
    lcd.print(mappedGain);
  }

// *********************************************************************
// READ ENCODER VALUES, SET MASTER TEMPO & GAIN
// *********************************************************************
  void MasterSet()
  {  
      unsigned char t = digitalRead(_LCDML_CONTROL_encoder_pin_button);
  
      
      if (_LCDML_CONTROL_encoder_high_active == 1)            // change button status if high and low active are switched
      {
         t != t;
      }
     if (LCDML_BUTTON_checkAny())                              // check if any button is pressed 
     { 
      if (LCDML_BUTTON_checkLeft() || LCDML_BUTTON_checkUp()) // check if button left is pressed
      {
        LCDML_BUTTON_resetLeft();                             // reset the left button
        LCDML_BUTTON_resetUp();                               // reset the left button
        
        if (tempoGainSwitch == false)
        {
          bpm += 1;
          bpmUpdate();
        }
        else
        {
          if (masterGain == 127)
          {
            // Don't update it - MIDI vals 0 - 127
            // Serial.println("test: master > 127");
          }
          else
          {
            masterGain += 1;
            masterGainUpdate();
          }
        }
      } 
      
      if (LCDML_BUTTON_checkRight() || LCDML_BUTTON_checkDown()) // check if button left is pressed
      {
        LCDML_BUTTON_resetRight();                               // reset the right button
        LCDML_BUTTON_resetDown();                                // reset the right button
  
        if (tempoGainSwitch == false)
        {
          bpm -= 1;
          bpmUpdate();
        }
        else
        {
          if (masterGain == 0)
          {
            // Don't update it - MIDI vals 0 - 127
            // Serial.println("test: master >= 0");
          }
          else
          {
            masterGain -= 1;
            masterGainUpdate();
          }
        }
      }
      
      if (LCDML_BUTTON_checkEnter())
      {
        LCDML_BUTTON_resetEnter();
        
        if (!t && g_LCDML_CONTROL_encoder_t_prev == 0)          // press button once
        {                    
          if (tempoGainSwitch == false)
          {
            lcd.setCursor(0, 3);
            lcd.print(" ");
  
            lcd.setCursor(11, 3);
            lcd.write(_LCDML_DISP_cfg_cursor);
            
            tempoGainSwitch = true;
          }
          else if (tempoGainSwitch)
          {
            lcd.setCursor(11, 3);
            lcd.print(" ");
  
            lcd.setCursor(0, 3);
            lcd.write(_LCDML_DISP_cfg_cursor);
            
            tempoGainSwitch = false;
          }
        } 
        else
        {
          g_LCDML_CONTROL_encoder_t_prev = 0;
        }
      }
     }
  }
