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
// Purpose : This is the header file for the functions of the state machine
//           of the system.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : Function names can and should be renamed by the user to improve the
//           readability of the code. User must set the functions here and adjust
//           the structure skadiCommandTable located in Setup.c.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef __STATE_FUNCTIONS_H__
#define	__STATE_FUNCTIONS_H__

#include "Setup.h"
#include "Potentiometer.h"
#include "StateMachine.h"


//==============================================================================
// State Machine public functions prototypes
//==============================================================================

// Buttons functions
// =======================================
void AssessButtons (void);
// =======================================

inline INT8 FifoWriteBuffer (sUartFifoBuffer_t *fifo, UINT8 *data, UINT8 length);
inline INT8 FifoWrite       (sUartFifoBuffer_t *fifo, UINT8 *data);
inline INT8 FifoRead        (sUartFifoBuffer_t *fifo, UINT8 *data);

inline void ComputeCellPower (UINT8 cellIndex, UINT8 potIndex);
inline void GetAdcValues (void);
inline void ComputeMeanAdcValues (void);

//==============================================================================
// Macro definitions
//==============================================================================

// Typedef for mapping the buttons on the board
typedef union
{
  struct
  {
    UINT8  sw1       : 1
          ,sw2       : 1
          ,sw3       : 1
          ,          : 5
          ;
  } bits;

  UINT8 byte;

} Buttons_t;

typedef union
{
  struct
  {
    UINT8  sw1       : 1
          ,sw2       : 1
          ,sw3       : 1
          ,          : 5
          ;
  } bits;

  UINT8 byte;

} Chng_t;

typedef struct sButtonStates
{
  Buttons_t buttons;
  Chng_t    chng;
} sButtonStates_t;


//==============================================================================
// Variable declarations
//==============================================================================


#endif	/* __STATE_FUNCTIONS_H__ */

