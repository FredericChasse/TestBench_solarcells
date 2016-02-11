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
inline INT8 ShutdownPot(UINT8 numPot);   // Shutdown a potentiometer

/**************************************************************
 * Function name  : TurnOnPot
 * Purpose        : Turn on a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 TurnOnPot(UINT8 numPot);

/**************************************************************
 * Function name  : ResetPot
 * Purpose        : Reset a pot at its mid-value.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 ResetPot   (UINT8 numPot);   // Reset a pot at its mid-value

/**************************************************************
 * Function name  : ComputePotValue
 * Purpose        : Converts an ohmic value to a range of 0-255.
 * Arguments      : UINT32 desiredValue, in ohms.
 * Returns        : value on success, -1 on failure.
 *************************************************************/
inline INT16 ComputePotValue(UINT32 desiredValue);

/**************************************************************
 * Function name  : InitPot
 * Purpose        : Turn on a potentiometer and put it at midvalue.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 InitPot (UINT8 numPot);

/**************************************************************
 * Function name  : SetPot
 * Purpose        : Initialize a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-3)
 *                  UINT8 index : pot index (0 - 3)
 *                  UINT8 value : pot increment (0 - 255)
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 SetPot (UINT8 numPot, UINT8 index, UINT8 value);


//==============================================================================
// Macro definitions
//==============================================================================
#define MAX_POT_VALUE   1253
#define WIPER_VALUE     53

#define CS0             LATDbits.LATD4
#define CS1             LATDbits.LATD5
#define CS2             LATDbits.LATD6
#define CS3             LATDbits.LATD7


//==============================================================================
// Variable declarations
//==============================================================================


#endif	/* __POTENTIOMETER_H__ */

