// =====================================================================
//
// CONTROL
//
// =====================================================================
// *********************************************************************
// *********************************************************************
// content:
// (0) Control with encoder
// *********************************************************************

#define _LCDML_CONTROL_cfg      0

// *********************************************************************
// CONTROL TASK, DO NOT CHANGE
// *********************************************************************
void LCDML_BACK_setup(LCDML_BACKEND_control)      
// *********************************************************************
{
  // call setup   
  LCDML_CONTROL_setup();      
}
// backend loop
boolean LCDML_BACK_loop(LCDML_BACKEND_control)
{    
  // call loop
  LCDML_CONTROL_loop();

  // go to next backend function and do not block it
  return true;  
}
// backend stop stable
void LCDML_BACK_stable(LCDML_BACKEND_control)
{
}

// *********************************************************************
// *************** (0) CONTROL WITH ENCODER ****************************
// *********************************************************************
#if(_LCDML_CONTROL_cfg == 0)
// settings
  #define _LCDML_CONTROL_encoder_pin_a           10 // pin encoder b
  #define _LCDML_CONTROL_encoder_pin_b           11 // pin encoder a
  #define _LCDML_CONTROL_encoder_pin_button      12 // pin taster
  
  #define _LCDML_CONTROL_encoder_high_active     0  // (0 = low active (pullup), 1 = high active (pulldown)) button
  #define _LCDML_CONTROL_digital_low_active      0  // (0 = low active (pullup), 1 = high active (pulldown) button
  
  #define _LCDML_CONTROL_digital_enable_quit     1
  #define _LCDML_CONTROL_digital_quit            13

  // global defines
  uint8_t  g_LCDML_CONTROL_encoder_t_prev = 0;
  uint8_t  g_LCDML_CONTROL_encoder_a_prev = 0;

// *********************************************************************
// setup
void LCDML_CONTROL_setup()
{
  // set encoder update intervall time 
  LCDML_BACK_dynamic_setLoopTime(LCDML_BACKEND_control, 5UL);  // 5ms 

  // init pins  
  pinMode(_LCDML_CONTROL_encoder_pin_a      , INPUT_PULLUP);
  pinMode(_LCDML_CONTROL_encoder_pin_b      , INPUT_PULLUP);
  pinMode(_LCDML_CONTROL_encoder_pin_button , INPUT_PULLUP);

  # if(_LCDML_CONTROL_digital_enable_quit == 1)
    pinMode(_LCDML_CONTROL_digital_quit     , INPUT_PULLUP);
  # endif

//  pinMode(_LCDML_CONTROL_digital_shift      , INPUT_PULLUP);

}
// *********************************************************************
// loop
void LCDML_CONTROL_loop()
{    
  // read encoder status
  unsigned char a = digitalRead(_LCDML_CONTROL_encoder_pin_a);
  unsigned char b = digitalRead(_LCDML_CONTROL_encoder_pin_b);
  unsigned char t = digitalRead(_LCDML_CONTROL_encoder_pin_button);
  
  // change button status if high and low active are switched
  if (_LCDML_CONTROL_encoder_high_active == 1) 
  {
    t != t;
  }
  
  // check encoder status and set control menu
  if (!a && g_LCDML_CONTROL_encoder_a_prev) 
  {
    g_LCDML_CONTROL_encoder_t_prev = 1;
    
    if (!b) { LCDML_BUTTON_up();   }
    else    { LCDML_BUTTON_down(); }            
  } 
  else 
  {
    // check button press time for enter
    if((millis() - g_LCDML_DISP_press_time) >= _LCDML_DISP_cfg_button_press_time) 
    {
      g_LCDML_DISP_press_time = millis(); // reset button press time
      
      // press button once
      if (!t && g_LCDML_CONTROL_encoder_t_prev == 0) 
      {          
          LCDML_BUTTON_enter();          
      } 
      else 
      {
        g_LCDML_CONTROL_encoder_t_prev = 0;
      }
    }      
  }
  
  g_LCDML_CONTROL_encoder_a_prev = a;  // set new encoder status 
  
  #if(_LCDML_CONTROL_digital_low_active == 1)
  #  define _LCDML_CONTROL_digital_a !
  #else
  #  define _LCDML_CONTROL_digital_a
  #endif

  // Digital 'go back' pin
  uint8_t but_stat = 0x00;

  #if(_LCDML_CONTROL_digital_enable_quit == 1)
    bitWrite(but_stat, 3, _LCDML_CONTROL_digital_a(digitalRead(_LCDML_CONTROL_digital_quit)));
  #endif

  if (bitRead(but_stat, 3)) { LCDML_BUTTON_quit() };
}
// *********************************************************************
// ******************************* END *********************************
// *********************************************************************

#else
  #error _LCDML_CONTROL_cfg is not defined or not in range
#endif
