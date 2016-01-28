//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateFunctions.h
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the header file for the functions of the potentiometers AD8403.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef __POTENTIOMETER_H__
#define	__POTENTIOMETER_H__

#include "Setup.h"


//==============================================================================
// State Machine private functions prototypes
//==============================================================================
static inline INT8 ShutdownPot(UINT8 numPot);   // Shutdown a potentiometer
static inline INT8 ResetPot   (UINT8 numPot);   // Reset a pot at its mid-value
static inline INT16 SetPot(UINT32 desiredValue);  // Converts an ohmic value to a range of 0-255


//==============================================================================
// Macro definitions
//==============================================================================
#define MAX_POT_VALUE   1253
#define WIPER_VALUE     53


//==============================================================================
// Variable declarations
//==============================================================================


#endif	/* __POTENTIOMETER_H__ */

