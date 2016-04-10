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
 * Function name  : SetAllPwmSkadi
 * Purpose        : Set the duty cycle of all LEDs
 * Arguments      : 1 : duty cycle * 10
 * Returns        : None.
 *************************************************************/
void SetAllPwmSkadi(sSkadi_t *skadi, sSkadiArgs_t args)
{
  sUartLineBuffer_t buffer;
  UINT8 i;

  UINT16 pwm = atoi(args.elements[0]);   // Convert argument to int

  if (pwm <= 1000)
  {
    for (i = 0; i < 16; i++)
    {
      SetLedDutyCycle(i, pwm);
    }
  }
  else
  {
    buffer.length = sprintf(buffer.buffer, "Mauvais argument!\r\n\n");
    Uart.PutTxFifoBuffer(UART3, &buffer);
  }
}


/**************************************************************
 * Function name  : SetPwmSkadi
 * Purpose        : Set the duty cycle of a LED
 * Arguments      : 1 : LED num (0 - 15), 2 : duty cycle * 10
 * Returns        : None.
 *************************************************************/
void SetPwmSkadi(sSkadi_t *skadi, sSkadiArgs_t args)
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


/**************************************************************
 * Function name  : SetPotSkadi
 * Purpose        : Set the value of a potentiometer
 * Arguments      : 1 : Pot num (0 - 3), 2 : index (0 - 3), 3 : pot value (0 - 255)
 * Returns        : None.
 *************************************************************/
void SetPotSkadi(sSkadi_t *skadi, sSkadiArgs_t args)
{
  sUartLineBuffer_t buffer;

  UINT16  pot = atoi(args.elements[0])    // Convert argument to int
         ,index = atoi(args.elements[1])
         ,value = atoi(args.elements[2])
         ;

  if ( (pot <= 3) && (index <= 3) && (value <= 255) )
  {
    SetPot((pot << 2 | index), value);
  }
  else
  {
    buffer.length = sprintf(buffer.buffer, "Mauvais argument!\r\n\n");
    Uart.PutTxFifoBuffer(UART3, &buffer);
  }
}


/**************************************************************
 * Function name  : SetPotSkadi
 * Purpose        : Set the value of a potentiometer
 * Arguments      : 1 : Pot num (0 - 3), 2 : pot value (0 - 255)
 * Returns        : None.
 *************************************************************/
void SetAllPotSkadi(sSkadi_t *skadi, sSkadiArgs_t args)
{
  sUartLineBuffer_t buffer;

  UINT16  pot = atoi(args.elements[0])    // Convert argument to int
         ,value = atoi(args.elements[2])
         ;

  if ( (pot <= 3) && (value <= 255) )
  {
    SetPotAllUnits(pot, value);
  }
  else
  {
    buffer.length = sprintf(buffer.buffer, "Mauvais argument!\r\n\n");
    Uart.PutTxFifoBuffer(UART3, &buffer);
  }
}