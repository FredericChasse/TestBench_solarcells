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

#include "..\headers\ControlAlgorithm.h"


//==============================================================================
// Control Algorithms functions
//==============================================================================

void MultiUnit (void)
{
  
}


void Caracterization (void)
{
  
}


void PSO (void)
{
  
}



/*
 * Function : TustinZ
 * Desc :     Discrete integrator using Tustin's method
 * Graphic example :
 *
 *   y(n)      1     T     z + 1
 *  -----  =  --- = --- * -------
 *   x(n)      s     2     z - 1
 *
 *         _______
 *  x(n)  |   1   | y(n)
 * ------>| ----- |------>
 *        |   s   |
 *        |_______|
 *
 *  iLaplace => y(n) = y(n-1) + T/2 * ( x(n-1) + x(n) )
 *
 */
void TustinZ (sTustinValue *input, sTustinValue *output, float acqTime)
{
  output->previousValue = output->currentValue;
  output->currentValue  = output->previousValue + acqTime/2 * (input->currentValue + input->previousValue);
}