//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Potentiometer.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the functions of the potentiometers AD8403.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\Potentiometer.h"


//==============================================================================
// State Machine private functions prototypes
//==============================================================================

//==============================================================================
// Potentiometers functions
//==============================================================================


/**************************************************************
 * Function name  : ShutdownPot
 * Purpose        : Shutdown a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 ShutdownPot(UINT8 numPot)
{
  if (numPot >= 4)
  {
    return -1;
  }
  Port.E.ClearBits(1 << numPot);
  return 0;
}


/**************************************************************
 * Function name  : TurnOnPot
 * Purpose        : Turn on a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 TurnOnPot(UINT8 numPot)
{
  if (numPot >= 4)
  {
    return -1;
  }
  Port.E.SetBits(1 << numPot);
  return 0;
}


/**************************************************************
 * Function name  : ResetPot
 * Purpose        : Reset a pot at its mid-value.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 ResetPot(UINT8 numPot)
{
  if (numPot >= 4)
  {
    return -1;
  }
  UINT8 i = 0;
  Port.D.ClearBits(1 << (numPot + 8));
  for (i = 0; i < 200; i++);
  Port.D.SetBits(1 << (numPot + 8));
  return 0;
}


/**************************************************************
 * Function name  : ComputePotValue
 * Purpose        : Converts an ohmic value to a range of 0-255.
 * Arguments      : UINT32 desiredValue, in ohms.
 * Returns        : value on success, -1 on failure.
 *************************************************************/
inline INT16 ComputePotValue(UINT32 desiredValue)
{
  if ( !((desiredValue <= MAX_POT_VALUE) && (desiredValue >= WIPER_VALUE)) )
  {
    return -1;
  }
  return ((float) ((desiredValue - WIPER_VALUE) / MAX_POT_VALUE)) + 0.5;
}


/**************************************************************
 * Function name  : SetPot
 * Purpose        : Initialize a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-3)
 *                  UINT8 index : pot index (0 - 3)
 *                  UINT8 value : pot increment (0 - 255)
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 SetPot (UINT8 numPot, UINT8 index, UINT8 value)
{
  if (numPot >= 4)
  {
    return -1;
  }
  if (index >= 4)
  {
    return -1;
  }
  
  while(Spi.IsSpiBusy(SPI3));
  Port.D.ClearBits(1 << (numPot + 4));
  Spi.SendCharacter( SPI3, ( (index << 9) | value ) );
  while(Spi.IsSpiBusy(SPI3));
  Port.D.SetBits(1 << (numPot + 4));

  return 0;
}


/**************************************************************
 * Function name  : InitPot
 * Purpose        : Initialize a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 InitPot (UINT8 numPot)
{
  if (numPot >= 4)
  {
    return -1;
  }
  
  TurnOnPot(numPot);
  
  ResetPot(numPot);

  return 0;
}