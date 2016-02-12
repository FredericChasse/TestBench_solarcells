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
// MATLAB FIFOs functions
//==============================================================================
inline INT8 FifoWrite(sUartFifoBuffer_t *fifo, UINT8 *data);
inline INT8 FifoRead (sUartFifoBuffer_t *fifo, UINT8 *data);


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
#define INIT_2_ACQ                1               // StateInit to StateAcq
#define INIT_2_ERROR              0               // StateInit to StateError
#define INIT_2_COMP               0               // StateInit to StateCompute
#define INIT_2_SEND_DATA          0               // StateInit to StateSendData

#define ACQ_2_SEND_DATA           oSendData       // StateAcq to StateSendData
#define ACQ_2_ACQ                !oSendData       // StateAcq to StateAcq
#define ACQ_2_COMP                0               // StateAcq to StateCompute
#define ACQ_2_ERROR               0               // StateAcq to StateError

#define SEND_DATA_2_ACQ           1              // StateSendData to StateAcq
#define SEND_DATA_2_SEND_DATA     0                // StateSendData to StateSendData
#define SEND_DATA_2_ERROR         0   // StateSendData to StateError
#define SEND_DATA_2_COMP          0   // StateSendData to StateCompute

#define ERROR_2_ACQ               0   // StateError to StateAcq
#define ERROR_2_SEND_DATA         0  // StateError to StateSendData
#define ERROR_2_ERROR             0                // StateError to StateError
#define ERROR_2_COMP              0                // StateError to StateCompute

#define COMP_2_ACQ                0         // StateCompute to StateAcq
#define COMP_2_SEND_DATA          0   // StateCompute to StateSendData
#define COMP_2_COMP               0    // StateCompute to StateCompute
#define COMP_2_ERROR              0   // StateCompute to StateError
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//=========================================
// UART packet size in bytes for MATLAB
//=========================================
#define MATLAB_PACKET_SIZE  128             // 32 floats = 128 bytes
//=========================================


//==============================================================================
// Variable declarations
//==============================================================================
void (*pState)(void);       // State pointer, used to navigate between states

#endif	/* __STATE_MACHINE_H__ */

