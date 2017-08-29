/* ===================================================================== *
 *                                                                       *
 * BACKEND SYSTEM                                                        *
 *                                                                       *
 * ===================================================================== *
 * every "backend function" needs three functions 
 * - void LCDML_BACK_setup(..func_name..)    
 * - void LCDML_BACK_loop(..func_name..)     
 * - void LCDML_BACK_stable(..func_name..)
 *
 * - every BACKEND function can be stopped and started
 * EXAMPLE CODE:
    void LCDML_BACK_setup(LCDML_BACKEND_control)
    {
      // setup
      // is called only if it is started or restartet (reset+start)
    }
    
    boolean LCDML_BACK_loop(LCDML_BACKEND_control)
    {    
      // runs in loop
      
     
      return false;  
    }
    
    void LCDML_BACK_stable(LCDML_BACKEND_control)
    {
      // stable stop
      // is called when a backend function is stopped with stopStable  
    }
 * ===================================================================== *
 */

// *********************************************************************
// SERIAL CALLBACK FUNCTION
// *********************************************************************
  void serialReadReturns()
  {
    if (Serial.available() > 0) 
    {
      incomingByte = Serial.read();             // read the incoming byte
      lcd.setCursor(0, 0);
  
      switch (incomingByte)
      {
        case 97:
          LCDML_lcd_menu_clear();
          lcd.print("Searching...");
          break;
          
        case 98:
          LCDML_lcd_menu_clear();
          lcd.print("Sample Loaded!");
          delay(3000);
          lookForReturn = false;
          LCDML_DISP_funcend();
          break;
          
        case 99:
          LCDML_lcd_menu_clear();
          lcd.print("Kit Loaded!");
          delay(3000);
          lookForReturn = false;
          LCDML_DISP_funcend();
          break;
          
        case 100:
          break;
          
        case 101:
          LCDML_lcd_menu_clear();
          lcd.print("Kit Saved!");
          delay(3000);
          lookForReturn = false;
          LCDML_DISP_funcend();
          break;

        case 102:
          lookForReturn = false;
          delay(2000);
          LCDML_DISP_funcend();
          break;
      }
    };
  }

