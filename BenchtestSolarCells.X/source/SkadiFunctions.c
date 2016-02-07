//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : SourceTemplate.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This C file gathers the functions used for Skadi.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : Function names can and should be renamed by the user to improve the
//           readability of the code. User must set the functions here and adjust
//           the structure skadiCommandTable located in Setup.c.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\SkadiFunctions.h"


//==============================================================================
// Private functions prototypes
//==============================================================================


//==============================================================================
// Variable definitions
//==============================================================================


//==============================================================================
// Functions
//==============================================================================

/**************************************************************
 * Function name  : LedToggle
 * Purpose        : Toggle Led 1 or 2 depending on the arguments
 * Arguments      : Received from Skadi functions
 * Returns        : None.
 *************************************************************/
void LedToggle(sSkadi_t *skadi, sSkadiArgs_t args)
{
  sUartLineBuffer_t buffer;

  UINT8 led = atoi(args.elements[0]);   // Convert argument to int

  if (led == 1)
  {
    LED1_TOGGLE;
  }
  else if (led == 2)
  {
    LED2_TOGGLE;
  }
  else
  {
    buffer.length = sprintf(buffer.buffer, "Cette led n'existe pas!\r\n\n");
    Uart.PutTxFifoBuffer(UART3, &buffer);
  }
}


/**************************************************************
 * Function name  : ClearScreen
 * Purpose        : Clear the terminal window
 * Arguments      : None.
 * Returns        : None.
 *************************************************************/
void ClearScreen(sSkadi_t *skadi, sSkadiArgs_t args)
{
  sUartLineBuffer_t buffer;
  buffer.buffer[0] = '\n';
  buffer.buffer[1] = '\r';
  UINT8 i;

  for (i = 2; i < 50; i++)
  {
    buffer.buffer[i] = '\n';
  }
  buffer.length = i;
  
  Uart.PutTxFifoBuffer(UART3, &buffer);
}


/**************************************************************
 * Function name  : SetPwm
 * Purpose        : Set the duty cycle of a LED
 * Arguments      : 1 : LED num (0 - 15), 2 : duty cycle * 10
 * Returns        : None.
 *************************************************************/
void SetPwm(sSkadi_t *skadi, sSkadiArgs_t args)
{
  sUartLineBuffer_t buffer;

  UINT16 led = atoi(args.elements[0]);   // Convert argument to int
  UINT16 pwm = atoi(args.elements[1]);   // Convert argument to int

  if ( (led <= 15) && (pwm <= 1000) )
  {
    SetLedDutyCycle(led, pwm);
  }
  else
  {
    buffer.length = sprintf(buffer.buffer, "Mauvais argument!\r\n\n");
    Uart.PutTxFifoBuffer(UART3, &buffer);
  }
}