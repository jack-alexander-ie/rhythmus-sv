/* ===================================================================== *
 *                                                                       *
 * SAMPLE SEARCH FUNCTIONS                                               *
 *                                                                       *
 * ===================================================================== */

// *********************************************************************
// MAIN SAMPLE SEARCH FUNCTION
// *********************************************************************
  void LCDML_DISP_setup(LCDML_FUNC_search)
  {   
    arrowX         =    0;
    rowX           =    0;
  
    searchTermPos  =    10;
    searchCount    =    1;
  
    for (int i = 0; i < sizeof(SearchChars); i++)
    {
      SearchChars[i] = 0x00;
    }
    SearchChars[0] =    search_mode;
    
    charSwitch     =    false;
  
    lcd.setCursor(0, 0);
    lcd.write(byte(5));
  
    // A - T
    lcd.setCursor(0, 1);
    for (int i = 65; i < 85; i++) {lcd.write(i);} 
    
    // U - Z
    lcd.setCursor(0, 3);
    for (int i = 85; i < 91; i++) {lcd.write(i);}
  
    // Space
    lcd.write(95);
  
    // Delete & Enter
    lcd.write(127);
    lcd.write('x');
  
    lcd.setCursor(9, 2);
    lcd.write(':');
  }
  
  void LCDML_DISP_loop(LCDML_FUNC_search)
  {
     unsigned char t = digitalRead(_LCDML_CONTROL_encoder_pin_button);
  
     // change button status if high and low active are switched
     if (_LCDML_CONTROL_encoder_high_active == 1) 
     {
       t != t;
     }
     
     if(LCDML_BUTTON_checkAny()) 
     { // check if any button is pressed
      
      if (LCDML_BUTTON_checkLeft() || LCDML_BUTTON_checkUp()) // check if button left is pressed
      {
        LCDML_BUTTON_resetLeft(); // reset the left button
        LCDML_BUTTON_resetUp(); // reset the left button
  
        if (rowX == 0)
        {
          // update lcd content
          lcd.setCursor(arrowX - 1, rowX);
          lcd.print(' ');
          
          lcd.setCursor(arrowX, rowX);
          lcd.write(byte(5));
  
          if (arrowX == rowMax)
          {
            for(int i = 0; i < rowMax; i++)
            {
              lcd.setCursor(i, rowX);
              lcd.print(' ');
            }
            rowX   =    2;
            arrowX =    0;
          }
        }
        else
        {
          lcd.setCursor(arrowX - 1, rowX);
          lcd.print(" ");
          
          lcd.setCursor(arrowX, rowX);
          lcd.write(byte(5));
  
          if (arrowX == 9)
          {
            for(int i = 0; i < 8; i++)
            {
              lcd.setCursor(i, rowX);
              lcd.print(' ');
            }
            
            lcd.setCursor(9, 2);
            lcd.write(':');
            
            rowX   =    0;
            arrowX =    0;
            
            lcd.setCursor(0, 0);
            lcd.write(byte(5));
          }
        }   
        arrowX++; 
      }    
      
      if (LCDML_BUTTON_checkEnter())
      {
        int charVal;
        uint8_t hexChar;

        LCDML_BUTTON_resetEnter();
        
        // press button once
        if (!t && g_LCDML_CONTROL_encoder_t_prev == 0) 
        {                    
            if (rowX == 0)
            {
              if (charSwitch)                           // Initial switch on load to take care of unwanted @ char
              { 
                charVal = (arrowX - 1) + 65;
              }
              else
              {
                charVal = arrowX + 65;
                charSwitch = true;
              };
  
              hexChar = (uint8_t) charVal;
              
              SearchChars[searchCount] = hexChar;
              
              searchCount++;
            }
            else
            {
              if (arrowX < 7)
              {
                charVal = (arrowX - 1) + 85;
                hexChar = (uint8_t) charVal;
  
                SearchChars[searchCount] = hexChar;
                
                searchCount++;
              }
              else if (arrowX == 7)
              {
                charVal = 10;
                hexChar = (uint8_t) charVal;
                
                SearchChars[searchCount] = hexChar;
                
                searchCount++;
              }
              else if (arrowX == 8)
              {
                // Delete last character                
                if (searchCount > 1) {searchCount--;}
                SearchChars[searchCount] = 0x00;
              }
              else if (arrowX == 9)                             // Confirm search term
              {
                int SearchCharsLength = sizeof(SearchChars);
                PrintHex8(SearchChars, SearchCharsLength);
                lookForReturn = true;
              }
            }

            printSearchTerm();                                  // print the search term to the screen

        } 
        else
        {
          g_LCDML_CONTROL_encoder_t_prev = 0;
        }
       }
      };
  
      if (lookForReturn)
      {
        serialReadReturns();
      }
  };
  
  void LCDML_DISP_loop_end(LCDML_FUNC_search)
  {

  };


// *********************************************************************
// PRINT SEARCH TERM TO LCD FUNCTION
// ********************************************************************* 
  void printSearchTerm()
  {
    for (int i = 1; i < searchCount + 1; i++)
    {
      char nullVal = SearchChars[i + 1];
      lcd.setCursor(i + 9, 2);
      
      if (SearchChars[i] > nullVal)
      {  
        lcd.write(SearchChars[i]);
      }
      else if (SearchChars[i] == nullVal)
      {
        lcd.print(" ");
      }
//      else if (ascVal == 10)
//      {
//        Sprintln("here");
//        lcd.print("_");
//        delay(500);
//        lcd.print(" ");
//      }
    } 
  }


// *********************************************************************
// PLAY SAMPLE FUNCTION
// *********************************************************************  
  void LCDML_DISP_setup(LCDML_FUNC_play_sample)
  {
    lcd.setCursor(0, 0);
    lcd.print("********************");
    
    lcd.setCursor(6, 1);
    lcd.print("PLAYING");
    lcd.setCursor(7, 2);
    lcd.print("AUDIO");

    lcd.setCursor(0, 3);
    lcd.print("********************");
  };
  
  void LCDML_DISP_loop(LCDML_FUNC_play_sample)
  {
    if (playSample == false)
    {
      playSample = true;
      Sprintln("0x000x000x0B0x01");
      lookForReturn = true;
    }
    else
    {
      playSample = false;
    };

    if (lookForReturn)
    {
      serialReadReturns();
    }
  };
  
  void LCDML_DISP_loop_end(LCDML_FUNC_play_sample)
  {
    Sprintln("0x000x000x0B0x00");
  };


// *********************************************************************
// GET RANDOM SIMILAR FUNCTION
// *********************************************************************  
  void LCDML_DISP_setup(LCDML_FUNC_get_random)
  {
    lcd.print("Searching...");
  };
  
  void LCDML_DISP_loop(LCDML_FUNC_get_random)
  {
    if (getSimilar == false)
    {
      getSimilar = true;
      Sprintln("0x000x000x0C0x01");
      lookForReturn = true;
    }
    
    if (lookForReturn) {serialReadReturns();}
    
  };
  
  void LCDML_DISP_loop_end(LCDML_FUNC_get_random)
  {
    getSimilar = false;
  };


// *********************************************************************
// GET BASED ON SPECTRAL CHARACTERISTIC
// ********************************************************************* 
void SpectralProfileHTTP(int specType)
{
  if (commandSwitch == false)
  {
    commandSwitch = true;
    
    SpectralProf[0]  = get_spec;
    SpectralProf[1]  = 0x00;
    
    int paramSelect;
  
    switch (specType)
    {
      case 1:
        paramSelect      = random(40);
        SpectralProf[2]  = lowlevel;
        break;
      
      case 2:
        paramSelect      = random(16);
        SpectralProf[2]  = rhythm;
        break;
      
      case 3:
        paramSelect      = random(17);
        SpectralProf[2]  = tonal;
        break;
    }

    byte parameter       = (byte) paramSelect;
    SpectralProf[3]      = parameter;

    PrintHex8(SpectralProf, 4);
  }

  if (lookForReturn)
  {
    serialReadReturns();
  }
};

// *********************************************************************
// GET BASED ON LOWLEVEL
// *********************************************************************  
  void LCDML_DISP_setup(LCDML_FUNC_get_lowlevel)
  {
    lcd.print("Searching...");
  };
  
  void LCDML_DISP_loop(LCDML_FUNC_get_lowlevel)
  {
    SpectralProfileHTTP(1);
  };
  
  void LCDML_DISP_loop_end(LCDML_FUNC_get_lowlevel)
  {
    commandSwitch = false;
  };


// *********************************************************************
// GET BASED ON RHYTHM
// *********************************************************************  
  void LCDML_DISP_setup(LCDML_FUNC_get_rhythm)
  {
    lcd.print("Searching...");
  };
  
  void LCDML_DISP_loop(LCDML_FUNC_get_rhythm)
  {
    SpectralProfileHTTP(2);
  };
  
  void LCDML_DISP_loop_end(LCDML_FUNC_get_rhythm)
  {
    commandSwitch = false;
  };

// *********************************************************************
// GET BASED ON TONAL
// *********************************************************************  
  void LCDML_DISP_setup(LCDML_FUNC_get_tonal)
  {
    lcd.print("Searching...");
  };
  
  void LCDML_DISP_loop(LCDML_FUNC_get_tonal)
  {
    SpectralProfileHTTP(3);
  };
  
  void LCDML_DISP_loop_end(LCDML_FUNC_get_tonal)
  {
    commandSwitch = false;
  };
