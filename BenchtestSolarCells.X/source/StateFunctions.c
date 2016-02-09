//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateFunctions.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the functions of the state machine of
//           the system.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : Function names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\StateFunctions.h"


//==============================================================================
// Variable declarations
//==============================================================================

// All the buttons used. 3 on the steering wheel, 3 on the board
sButtonStates_t buttons =
{
  .chng   .byte = 0
 ,.buttons.byte = 0

 // switches on board are at 1 when not pressed
 ,.buttons.bits.sw1 = 1
 ,.buttons.bits.sw2 = 1
 ,.buttons.bits.sw3 = 1
};


//==============================================================================
// State Machine private functions prototypes
//==============================================================================


//==============================================================================
// State Machine functions
//==============================================================================



//==============================================================================
// Buttons functions
//==============================================================================
void AssessButtons (void)
{
  UINT16 i = 0;
  
  float sin[2][15] = { {0 , .4189 , .8378 , 1.2566 , 1.6755 , 2.0944 , 2.5133 , 2.9322 , 3.3510 , 3.7699 , 4.1888 , 4.6077 , 5.0265 , 5.4454 , 5.8643} ,
                       {0 , .4067 , .7431 , .9511  , .9945  , .8660  , .5878  , .2079  , -.2079 , -.5878 , -.8660 , -.9945 , -.9511 , -.7431 , -.4067} };
  
  sUartLineBuffer_t buffer = 
  { 
     .buffer = {0} 
    ,.length =  0 
  };
  
  memcpy(buffer.buffer, sin, 30*4);
  buffer.length = 30*4;
  
  // <editor-fold defaultstate="collapsed" desc="Check changes on board">
  // <editor-fold defaultstate="collapsed" desc="Change on SW1 on board">
  if (buttons.buttons.bits.sw1 != SW1)
  {
    buttons.buttons.bits.sw1    = SW1;
    buttons.chng.bits.sw1       =   1;
  }
  // </editor-fold>

  // <editor-fold defaultstate="collapsed" desc="Change on SW2 on board">
  if (buttons.buttons.bits.sw2 != SW2)
  {
    buttons.buttons.bits.sw2    = SW2;
    buttons.chng.bits.sw2       =   1;
  }
  // </editor-fold>

  // <editor-fold defaultstate="collapsed" desc="Change on SW3 on board">
  if (buttons.buttons.bits.sw3 != SW3)
  {
    buttons.buttons.bits.sw3    = SW3;
    buttons.chng.bits.sw3       =   1;
  }
  // </editor-fold>
  // </editor-fold>

  // <editor-fold defaultstate="collapsed" desc="Assess changes">
  if (buttons.chng.byte)  // If any change has occured on any button
  {
    // <editor-fold defaultstate="collapsed" desc="SW1 on board">
    if (buttons.chng.bits.sw1)
    {
      buttons.chng.bits.sw1 = 0;

      if (!buttons.buttons.bits.sw1)     // If SW1 is pressed
      {
        Uart.PutTxFifoBuffer(U_MATLAB, &buffer);
        while(!SW1);
        for (i = 0; i < 15; i++)
        {
          sin[0][i] += 2*PI;
          memcpy(buffer.buffer, sin, 120);
          buffer.length = 120;
        }
      }
      else                                // If SW1 is not pressed
      {
        
      }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="SW2 on board">
    if (buttons.chng.bits.sw2)
    {
      buttons.chng.bits.sw2 = 0;

      if (!buttons.buttons.bits.sw2)     // If SW2 is pressed
      {
        
      }
      else                              // If SW2 is not pressed
      {

      }
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="SW3 on board">
    if (buttons.chng.bits.sw3)
    {
      buttons.chng.bits.sw3 = 0;

      if (!buttons.buttons.bits.sw3)     // If SW3 is pressed
      {
        
      }
      else                          // If SW3 is not pressed
      {
        
      }
    }
    // </editor-fold>
  }
  // </editor-fold>
}