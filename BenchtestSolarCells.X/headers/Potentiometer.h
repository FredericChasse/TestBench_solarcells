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
// Potentiometer public functions prototypes
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
 * Function name  : ComputePotValueDec2Float
 * Purpose        : Converts a range of 0-255 to an ohmic value.
 * Arguments      : UINT8 decimal value [0 - 255]
 *                  float *realValue : address of the result
 * Returns        : None.
 *************************************************************/
inline void ComputePotValueDec2Float(UINT8 desiredValue, float *realValue);

/**************************************************************
 * Function name  : ComputePotValueFloat2Dec
 * Purpose        : Converts an ohmic value to a range of 0-255.
 * Arguments      : float desiredValue, in ohms.
 * Returns        : value on success, -1 on failure.
 *************************************************************/
inline INT16 ComputePotValueFloat2Dec(float desiredValue);

/**************************************************************
 * Function name  : InitPot
 * Purpose        : Turn on a potentiometer and put it at midvalue.
 * Arguments      : UINT8 numPot : the number of the pot (0-3).
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 InitPot (UINT8 numPot);

/**************************************************************
 * Function name  : SetPot
 * Purpose        : Set the value of a potentiometer.
 * Arguments      : UINT8 numPot : the number of the pot (0-15)
 *                  UINT8 value : pot increment (0 - 255)
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 SetPot (UINT8 numPot, UINT8 value);

///**************************************************************
// * Function name  : SetPot
// * Purpose        : Initialize a potentiometer.
// * Arguments      : UINT8 numPot : the number of the pot (0-3)
// *                  UINT16 index : pot index (0 - 3)
// *                  UINT8 value : pot increment (0 - 255)
// * Returns        : 0 on success, -1 on failure.
// *************************************************************/
//inline INT8 SetPot (UINT8 numPot, UINT16 index, UINT8 value);

/**************************************************************
 * Function name  : SetPotAllUnits
 * Purpose        : Set all units of a pot to the same value.
 * Arguments      : UINT8 numPot : the number of the pot (0-3)
 *                  UINT8 index : pot index (0 - 3)
 *                  UINT8 value : pot increment (0 - 255)
 * Returns        : 0 on success, -1 on failure.
 *************************************************************/
inline INT8 SetPotAllUnits (UINT8 numPot, UINT8 value);


//==============================================================================
// Macro definitions
//==============================================================================
//#define MAX_POT_VALUE   9.55k +9.56k+9.53k+9.54k
//#define MAX_POT_VALUE   9545
#define MAX_POT_VALUE   1050.0f
//#define WIPER_VALUE     (51.7 + 50.9 + 52.3 + 50.2) / 4
//#define WIPER_VALUE     51
#define WIPER_VALUE     50.0f
//#define MID_VALUE       4.79k+4.88k+4.79k+4.79k
//#define MID_VALUE       4800
#define MID_VALUE       545.3.0f

#define VREF            2.489f
//#define VREF            3.268f

#define CS0             LATDbits.LATD4
#define CS1             LATDbits.LATD5
#define CS2             LATDbits.LATD6
#define CS3             LATDbits.LATD7

//float potRealValues[256];


//==============================================================================
// Variable declarations
//==============================================================================


#endif	/* __POTENTIOMETER_H__ */

