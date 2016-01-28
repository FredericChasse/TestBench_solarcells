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
static inline INT8 ShutdownPot(UINT8 numPot);   // Shutdown a potentiometer
static inline INT8 ResetPot   (UINT8 numPot);   // Reset a pot at its mid-value
static inline INT16 SetPot(UINT32 desiredValue);  // Converts an ohmic value to a range of 0-255

//==============================================================================
// Potentiometers functions
//==============================================================================


//==============================================================================
// ShutdownPot : Shutdown a potentiometer
//==============================================================================
static inline INT8 ShutdownPot(UINT8 numPot)
{
  if ( (numPot < 5) && (numPot >= 0) )
  {
    Port.E.ClearBits(numPot);
    return 0;
  }
  else
  {
    return -1;
  }
}
//==============================================================================


//==============================================================================
// ResetPot : Reset a pot at its mid-value
//==============================================================================
static inline INT8 ResetPot(UINT8 numPot)
{
  if ( (numPot < 5) && (numPot >= 0) )
  {
    Port.D.ClearBits(numPot + 8);
    return 0;
  }
  else
  {
    return -1;
  }
}
//==============================================================================


//==============================================================================
// SetPot : Converts an ohmic value to a range of 0-255
//==============================================================================
static inline INT16 SetPot(UINT32 desiredValue)
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
//==============================================================================