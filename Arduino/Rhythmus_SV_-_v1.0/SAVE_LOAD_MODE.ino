/* ===================================================================== *
 *                                                                       *
 * LOAD/SAVE MODE FUNCTIONS                                              *
 *                                                                       *
 * ===================================================================== */

// *********************************************************************
// LOAD/SAVE KIT MAIN FUNCTION
// *********************************************************************
  void kitModeSetup()
  {
    lcd.setCursor(0, 0);
    lcd.print(F("Select kit number:"));
    lcd.setCursor(0, 2);
    lcd.print(F("0"));
    g_button_value = 0;                                       // Reset Button Value 
  };
  
  void kitLoop(byte mode)
  {
    unsigned char t = digitalRead(_LCDML_CONTROL_encoder_pin_button);
  
    
    if (_LCDML_CONTROL_encoder_high_active == 1)              // change button status if high and low active are switched
    {
      t != t;
    }
    
    if (LCDML_BUTTON_checkAny())                              // check if any button is pressed (enter, up, down, left, right)
    {
      if (LCDML_BUTTON_checkLeft() || LCDML_BUTTON_checkUp()) // check if button left is pressed
      {
        LCDML_BUTTON_resetLeft();                             // reset the left button
        LCDML_BUTTON_resetUp();                               // reset the left button
        g_button_value++;
        
        lcd.setCursor(0, 2);                                  // update lcd content & set cursor   
        lcd.print(g_button_value);                            // print change content
      }
  
      if (LCDML_BUTTON_checkEnter())
      {
        
        LCDML_BUTTON_resetEnter();
        
        if (!t && g_LCDML_CONTROL_encoder_t_prev == 0) 
        {
          LoadSaveData[0] = mode;
          LoadSaveData[1] = 0x00;
          LoadSaveData[2] = 0x00;
          LoadSaveData[3] = (byte) g_button_value;

          PrintHex8(LoadSaveData, 4);
          
          lookForReturn = true;
        }
        else
        {
          g_LCDML_CONTROL_encoder_t_prev = 0;
        }
      } 
    }
    
    if (g_button_value >= 25)                               // reset button count if > 24
    {
      g_button_value = 0;
      lcd.setCursor(1, 2);                                  // set cursor
      lcd.print(' ');                                       // set cursor
      lcd.setCursor(0, 2);                                  // set cursor
      lcd.print(g_button_value);                            // print change content
    }
  
    if (lookForReturn) {serialReadReturns();}
    
  };


// **************************************
// LOAD KIT
// **************************************
  void LCDML_DISP_setup(LCDML_FUNC_load)
  {
    kitModeSetup();
  };
  
  void LCDML_DISP_loop(LCDML_FUNC_load)
  { 
    kitLoop(load_mode);
  };
  
  void LCDML_DISP_loop_end(LCDML_FUNC_load) 
  {
  };


// **************************************
// SAVE KIT
// **************************************
  void LCDML_DISP_setup(LCDML_FUNC_save)
  {
    kitModeSetup();
  };
  
  void LCDML_DISP_loop(LCDML_FUNC_save)
  { 
    kitLoop(save_mode);
  };
  
  void LCDML_DISP_loop_end(LCDML_FUNC_save) 
  {
  };
