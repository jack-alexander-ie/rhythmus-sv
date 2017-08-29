/* ===================================================================== *
 *                                                                       *
 * EDIT MODE FUNCTIONS                                                   *
 *                                                                       *
 * ===================================================================== */

// *********************************************************************
// EDIT MODE FUNCTION
// *********************************************************************
  void LCDML_DISP_setup(LCDML_FUNC_edit)
  {
    lcd.setCursor(0, 0);
    lcd.write(_LCDML_DISP_cfg_cursor);
  };
  
  void LCDML_DISP_loop(LCDML_FUNC_edit)
  {
    // Control message - 3
  };
  
  void LCDML_DISP_loop_end(LCDML_FUNC_edit)
  {
  };
