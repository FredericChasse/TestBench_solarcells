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
  if ( (numPot < 5) && (numPot >= 0) )
  {
    Port.E.ClearBits(1 << numPot);
    return 0;
  }
  else
  {
    return -1;
  }
}


/**************************************************************
 * Function name  : ResetPot
 * Purpose        : Reset a pot at its mid-value.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 ResetPot(UINT8 numPot)
{
  if ( (numPot < 5) && (numPot >= 0) )
  {
    Port.D.ClearBits(1 << (numPot + 8));
    return 0;
  }
  else
  {
    return -1;
  }
}


/**************************************************************
 * Function name  : ComputePotValue
 * Purpose        : Converts an ohmic value to a range of 0-255.
 * Arguments      : UINT32 desiredValue, in ohms.
 * Returns        : value on success, -1 on failure.
 *************************************************************/
inline INT16 ComputePotValue(UINT32 desiredValue)
{
  if ( (desiredValue <= MAX_POT_VALUE) && (desiredValue >= WIPER_VALUE) )
  {
    return ((float) ((desiredValue - WIPER_VALUE) / MAX_POT_VALUE)) + 0.5;
  }
  else
  {
    return -1;
  }
}


/**************************************************************
 * Function name  : InitPot
 * Purpose        : Initialize a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 InitPot (UINT8 numPot)
{
  if ( (numPot < 5) && (numPot >= 0) )
  {
    while(Spi.IsSpiBusy(SPI3));
    Port.D.ClearBits(1 << (numPot + 4));
    Spi.SendCharacter(SPI3, 0);
    Spi.SendCharacter(SPI3, 122); // MidValue
    while(Spi.IsSpiBusy(SPI3));
    Port.D.SetBits(1 << (numPot + 4));
    
    while(Spi.IsSpiBusy(SPI3));
    Port.D.ClearBits(1 << (numPot + 4));
    Spi.SendCharacter(SPI3, 1);
    Spi.SendCharacter(SPI3, 122); // MidValue
    while(Spi.IsSpiBusy(SPI3));
    Port.D.SetBits(1 << (numPot + 4));
    
    while(Spi.IsSpiBusy(SPI3));
    Port.D.ClearBits(1 << (numPot + 4));
    Spi.SendCharacter(SPI3, 2);
    Spi.SendCharacter(SPI3, 122); // MidValue
    while(Spi.IsSpiBusy(SPI3));
    Port.D.SetBits(1 << (numPot + 4));
    
    while(Spi.IsSpiBusy(SPI3));
    Port.D.ClearBits(1 << (numPot + 4));
    Spi.SendCharacter(SPI3, 3);
    Spi.SendCharacter(SPI3, 122); // MidValue
    while(Spi.IsSpiBusy(SPI3));
    Port.D.SetBits(1 << (numPot + 4));
    
    return 0;
  }
  else
  {
    return -1;
  }
}