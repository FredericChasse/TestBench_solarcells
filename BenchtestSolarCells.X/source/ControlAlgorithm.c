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
// Variable declarations
//==============================================================================
extern UINT16 nSamples;
const float DELTA_FLOAT = 3.9;
const UINT8 DELTA_BYTE  = 1;
extern struct sAllCells sCellValues;
extern BOOL  oCaracMode
            ,oCaracDone
            ,oMultiUnitMode
            ,oMultiUnitDone
            ,oPsoMode
            ,oPsoDone
            ;
extern UINT8 potIndexValue[16];

sTustinValue  gradError   = {0}
             ,gradError_p = {0}
             ;

//==============================================================================
// Control Algorithms functions
//==============================================================================

void RandomValue(float *value)
{
//  *value = ReadCoreTimer();
//  *value = ReadCoreTimer() * 2.3283064370807973754314699618685e-10;
  *value = (0xFFFF & ReadCoreTimer()) * 1.5259021896696421759365224689097e-5;
//  UINT16 temp = ReadCoreTimer() & 0xFFFF;
//  temp = (temp << 1) | ((((temp >> 3) & 1) ^ ((temp >> 12) & 1)) ^ (((temp >> 15) & 1) ^ ((temp >> 14) & 1)) );
//  *value = temp * 1.5259021896696421759365224689097e-5;
}


void SetPotInitialCondition (void)
{
  UINT8 i = 0;
  float value = 0;
  
  if (oCaracMode)
  {
    for (i = 0; i < 16; i++)
    {
      potIndexValue[i] = 0;
    }
    SetPotAllUnits(2, potIndexValue[0]);
  }
  else if (oPsoMode)
  {
    for (i = 0; i < 16; i++)
    {
      RandomValue(&value);
      potIndexValue[i] = value * 255;
    }
    SetPot(2, 0, potIndexValue[ 8]);
    SetPot(2, 1, potIndexValue[ 9]);
    SetPot(2, 2, potIndexValue[10]);
    SetPot(2, 3, potIndexValue[11]);
  }
  else if (oMultiUnitMode)
  {
    RandomValue(&value);
    potIndexValue[ 9] = value * 255;
    potIndexValue[10] = potIndexValue[9] + DELTA_BYTE;
    
    SetPot(2, 1, potIndexValue[ 9]);
    SetPot(2, 2, potIndexValue[10]);
  }
}


void Caracterization (void)
{
  UINT8 matlabBuffer[100];
  float fPotValue;
  
  if (!oCaracDone)
  {
    fPotValue = potIndexValue[0];
    memcpy(&matlabBuffer[ 0], &fPotValue, 4);
    memcpy(&matlabBuffer[ 4], &sCellValues.cells[ 8].cellPowerFloat, 4);
    memcpy(&matlabBuffer[ 8], &sCellValues.cells[ 9].cellPowerFloat, 4);
    memcpy(&matlabBuffer[12], &sCellValues.cells[10].cellPowerFloat, 4);
    memcpy(&matlabBuffer[16], &sCellValues.cells[11].cellPowerFloat, 4);
//      memcpy(&matlabBuffer[ 4], &sCellValues.cells[ 8].cellVoltFloat, 4);
//      memcpy(&matlabBuffer[ 8], &sCellValues.cells[ 9].cellVoltFloat, 4);
//      memcpy(&matlabBuffer[12], &sCellValues.cells[10].cellVoltFloat, 4);
//      memcpy(&matlabBuffer[16], &sCellValues.cells[11].cellVoltFloat, 4);
    AddDataToMatlabFifo(matlabBuffer, 20);

    if (potIndexValue[0] < 255)
    {
      potIndexValue[0]++;
      SetPotAllUnits(2, potIndexValue[0]);
    }
    else
    {
      oCaracDone = 1;
    }
  }
  else
  {
    LED1_ON;
    LED2_ON;
  }
}

void MultiUnit (void)
{
  UINT8 matlabBuffer[100];
  float fPotValue;
  
  float error;
  
  if (!oMultiUnitDone)
  {
    gradError_p.previousValue = gradError_p.currentValue;
    gradError_p.currentValue = sCellValues.cells[10].cellPowerFloat - sCellValues.cells[9].cellPowerFloat;
    
    TustinZ(&gradError_p, &gradError, 0.0002);
    
    if (gradError.currentValue > 255)
    {
      gradError.currentValue = 255;
    }
    
  }
}


void PSO (void)
{
  UINT8 matlabBuffer[100];
  float fPotValue;
  
  if (!oPsoDone)
  {
  }
}


/*
 * Function : TustinZ
 * Desc :     Discrete integrator using Tustin's method
 * Graphic example :
 *
 *         _______
 *  x(n)  |   1   | y(n)
 * ------>| ----- |------>
 *        |   s   |
 *        |_______|
 * 
 *   y(n)      1       T     z + 1
 *  -----  =  ---  =  --- * -------
 *   x(n)      s       2     z - 1
 *
 *  iLaplace => y(n) = y(n-1) + T/2 * ( x(n-1) + x(n) )
 *
 */
void TustinZ (sTustinValue *input, sTustinValue *output, float acqTime)
{
  output->previousValue = output->currentValue;
  output->currentValue  = output->previousValue + acqTime/2 * (input->currentValue + input->previousValue);
}