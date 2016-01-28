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

/**************************************************************
 * Function name  : ShutdownPot
 * Purpose        : Shutdown a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
static inline INT8 ShutdownPot(UINT8 numPot);   // Shutdown a potentiometer

/**************************************************************
 * Function name  : ResetPot
 * Purpose        : Reset a pot at its mid-value.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
static inline INT8 ResetPot   (UINT8 numPot);   // Reset a pot at its mid-value

/**************************************************************
 * Function name  : ComputePotValue
 * Purpose        : Converts an ohmic value to a range of 0-255.
 * Arguments      : UINT32 desiredValue, in ohms.
 * Returns        : value on success, -1 on failure.
 *************************************************************/
static inline INT16 ComputePotValue(UINT32 desiredValue);

/**************************************************************
 * Function name  : InitPot
 * Purpose        : Initialize a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
static inline INT8 InitPot (UINT8 numPot);


//==============================================================================
// Macro definitions
//==============================================================================
#define MAX_POT_VALUE   1253
#define WIPER_VALUE     53


//==============================================================================
// Variable declarations
//==============================================================================


#endif	/* __POTENTIOMETER_H__ */

