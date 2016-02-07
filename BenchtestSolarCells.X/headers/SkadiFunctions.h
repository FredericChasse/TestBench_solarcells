//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : HeaderTemplate.h
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This header file gathers the functions used for Skadi.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : Function names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef __SKADI_FUNCTIONS__
#define	__SKADI_FUNCTIONS__

#include "Setup.h"


//==============================================================================
// Public functions prototypes
//==============================================================================

/**************************************************************
 * Function name  : LedToggle
 * Purpose        : Toggle Led 1 or 2 depending on the arguments
 * Arguments      : Received from Skadi functions
 * Returns        : None.
 *************************************************************/
void LedToggle();

/**************************************************************
 * Function name  : SetPwm
 * Purpose        : Set the duty cycle of a LED
 * Arguments      : 1 : LED num (0 - 15), 2 : duty cycle * 10
 * Returns        : None.
 *************************************************************/
void SetPwm();

/**************************************************************
 * Function name  : ClearScreen
 * Purpose        : Clear the terminal window
 * Arguments      : None.
 * Returns        : None.
 *************************************************************/
void ClearScreen();

//==============================================================================
// Macro definitions
//==============================================================================


//==============================================================================
// Variable declarations
//==============================================================================
//
///***********************************
// * Table of functions used in Skadi
// **********************************/
//sSkadiCommand_t skadiCommandTable[] =
//{
//   {"LedDebug"    , LedDebug    , 1, "Usage : flash Led DEBUG"}   // 1 argument
//  ,{"LedCan"      , LedCan      , 1, "Usage : flash Led CAN"}     // 1 argument
//  ,{"ReInitSystem", ReInitSystem, 0, "Redo StateInit()"}          // 0 argument
//};
//
///***************************************************
// * Length of the table of functions used in Skadi
// **************************************************/
//const size_t lengthSkadiCommandTable = 3;


#endif	/* __SKADI_FUNCTIONS__ */

