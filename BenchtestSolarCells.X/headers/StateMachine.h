//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateMachine.h
// Author  : Frederic Chasse
// Date    : 2015-02-25
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the header file for the state machine of the system.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : State names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef __STATE_MACHINE_H__
#define	__STATE_MACHINE_H__

#include "Setup.h"
#include "StateFunctions.h"
#include "Potentiometer.h"
#include "LedDriver.h"


//==============================================================================
// State Machine public function prototypes
//==============================================================================
void StateInit      (void);   // Initialization state of the system
void StateAcq       (void);   // Acquisition state
void StateSendData  (void);   // Send data on UART
void StateCompute   (void);   // Compute algorithm
void StateError     (void);   // Error state. User should assess and correct errors in this state
void StateScheduler (void);   // State Scheduler. Decides which state is next
                              // depending on current state and flags. Used as a function


//==============================================================================
// Macro definitions
//==============================================================================

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// State scheduler flags
// The state scheduler is at the end of each state and
// decides which state is next. Following flags
// are used in this decision. The names used can and
// should be renamed to improve readability. Also, the
// conditions tested in the defines should be changed
// to proper tests
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define INIT_2_ACQ     1                // StateInit to StateAcq
#define INIT_2_ERROR   0                // StateInit to StateError
#define INIT_2_COMP   0                // StateInit to StateCompute
#define INIT_2_SEND_DATA    0                // StateInit to StateSendData

#define ACQ_2_SEND_DATA     0                // StateAcq to StateSendData
#define ACQ_2_ACQ      1                // StateAcq to StateAcq
#define ACQ_2_COMP      0                // StateAcq to StateCompute
#define ACQ_2_ERROR   0   // StateAcq to StateError

#define SEND_DATA_2_ACQ      breakFlag                // StateSendData to StateAcq
#define SEND_DATA_2_SEND_DATA     !breakFlag                // StateSendData to StateSendData
#define SEND_DATA_2_ERROR    breakFlag && errorFlag   // StateSendData to StateError
#define SEND_DATA_2_COMP    breakFlag && errorFlag   // StateSendData to StateCompute

#define ERROR_2_ACQ   !errorFlag && breakFlag   // StateError to StateAcq
#define ERROR_2_SEND_DATA   !errorFlag && !breakFlag  // StateError to StateSendData
#define ERROR_2_ERROR  errorFlag                // StateError to StateError
#define ERROR_2_COMP  errorFlag                // StateError to StateCompute

#define COMP_2_ACQ  errorFlag         // StateCompute to StateAcq
#define COMP_2_SEND_DATA  errorFlag   // StateCompute to StateSendData
#define COMP_2_COMP  errorFlag    // StateCompute to StateCompute
#define COMP_2_ERROR  errorFlag   // StateCompute to StateError
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//==============================================================================
// Variable declarations
//==============================================================================
void (*pState)(void);       // State pointer, used to navigate between states
volatile  INT8  breakFlag   // Flag indicating if the emergency break has been pressed
               ,errorFlag   // Flag indicating an error
               ;

#endif	/* __STATE_MACHINE_H__ */

