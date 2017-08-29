/* ===================================================================== *
 *                                                                       *
 * CONTROL VOLTAGE I/O                                                   *
 *                                                                       *
 * ===================================================================== */

// *********************************************************************
// CV READ FUNCTION
// *********************************************************************
  void cvRead()
  { 
    CVData[0] = play_mode;
    CVData[1] = cv_in;
  
    for (int i = 0; i < noOfChannels; i++)
    {
      int CvState = digitalRead(CvInPins[i]);
     
      if ((CvState == LOW) && (CvStates[i] == false))
      {
        CVData[2] = (byte) i + 1;
        CVData[3] = bang;
        
        PrintHex8(CVData, 4);
        
        CvStates[i] = true;
      }
      else if ((CvState == HIGH) && (CvStates[i] == true))
      {
        CvStates[i] = false;
      }
    }
  }
