//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// TestBench Solar Cells
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : ControlAlgorithm.h
// Author  : Frederic Chasse
// Date    : 2016-03-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the header file for the control algorithms of the system.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef __CONTROL_ALGORITHM_H__
#define	__CONTROL_ALGORITHM_H__

#include "Setup.h"
#include "StateMachine.h"
#include "StateFunctions.h"


//==============================================================================
// Macro definitions
//==============================================================================

/*
 * Structure used for the Tustin discrete integrators
 */
typedef struct sTustinValue
{
  float  previousValue
        ,currentValue
        ;
} sTustinValue;


//==============================================================================
// Control Algorithms public functions prototypes
//==============================================================================
void MultiUnit (void);
void PSO (void);
void Caracterization (void);
void TustinZ    (sTustinValue *input, sTustinValue *output, float acqTime);
void RandomValue(float *value);
void SetPotInitialCondition (void);

#endif	/* __CONTROL_ALGORITHM_H__ */

