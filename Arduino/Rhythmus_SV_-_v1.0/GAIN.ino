/* ===================================================================== *
 *                                                                       *
 * CHANNEL GAIN                                                          *
 *                                                                       *
 * ===================================================================== */

// *********************************************************************
// GAIN READ FUNCTION
// *********************************************************************
  void ChannelGain()
  {
    GainData[0] = play_mode;
    GainData[1] = gain;
    
    for(int i = 0; i < 10; i ++)                    // Loop through and read all 10 values
    { 
      GainFaders[i].update();
      
      if(GainFaders[i].hasChanged()) 
      {
        int val = GainFaders[i].getValue();
        int mappedGain = map(val, 0, 800, 0, 127);
  
        if (mappedGain == GainFaderValues[i])
        {
          GainData[2] = (byte) i + 1;
          GainData[3] = (byte) mappedGain;
  
          PrintHex8(GainData, 4);
        }
        else
        {
          GainFaderValues[i] = mappedGain;
        }
      };
    };
  }
