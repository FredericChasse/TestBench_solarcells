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
const float DELTA_FLOAT = 78;
const UINT8 DELTA_BYTE  = 10;
extern struct sAllCells sCellValues;
extern BOOL  oCaracMode
            ,oCaracDone
            ,oMultiUnitMode
            ,oMultiUnitDone
            ,oPsoMode
            ,oPsoDone
            ;

extern UINT8 potIndexValue[16];

extern const float potRealValues[256];
extern const float potRealValuesInverse[256];

UINT32 iteration = 0;

//sTustinValue  gradError   = {0}
//             ,gradError_p = {0}
//             ;

sMultiUnitValues_t multiUnitValues = 
{
  .alphaGain = 3900000000
 ,.deltaFloat = 39.21568627450980392156862745098
 ,.deltaByte = 10
 ,.gradError_p = {0}
 ,.gradError = {0}
 ,.initialInputFloat = 442.1568627450980392156862745098
 ,.initialInputByte = 100
 ,.sampleTime = 0.04
 ,.alphaDividedByDelta = 10000000
};

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
//    SetPot(0, 0, potIndexValue[ 0]);
//    SetPot(0, 1, potIndexValue[ 1]);
//    SetPot(0, 2, potIndexValue[ 2]);
//    SetPot(0, 3, potIndexValue[ 3]);
    
//    SetPot(2, 0, potIndexValue[ 4]);
//    SetPot(1, 1, potIndexValue[ 5]);
//    SetPot(1, 2, potIndexValue[ 6]);
//    SetPot(1, 3, potIndexValue[ 7]);
    
    SetPot(2, 0, potIndexValue[ 8]);
    SetPot(2, 1, potIndexValue[ 9]);
    SetPot(2, 2, potIndexValue[10]);
    SetPot(2, 3, potIndexValue[11]);
    
//    SetPot(3, 0, potIndexValue[12]);
//    SetPot(3, 1, potIndexValue[13]);
//    SetPot(3, 2, potIndexValue[14]);
//    SetPot(3, 3, potIndexValue[15]);
  }
  else if (oMultiUnitMode)
  {
    potIndexValue[ 9] = multiUnitValues.initialInputByte;
    potIndexValue[10] = potIndexValue[9] + multiUnitValues.deltaByte;
    
    multiUnitValues.gradError_p.currentValue = 0;
    multiUnitValues.gradError_p.previousValue = 0;
    multiUnitValues.gradError.previousValue = 0;
    multiUnitValues.gradError.currentValue = multiUnitValues.initialInputFloat;
    
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
  float fIteration;
  float gradError;
  UINT8 potValue;
  
  if (!oMultiUnitDone)
  {
    // Send data to MATLAB
    fIteration = iteration;
    memcpy(&matlabBuffer[ 0], &fIteration, 4);
    
    memcpy(&matlabBuffer[ 4], &potRealValues[potIndexValue[ 9]], 4);
    memcpy(&matlabBuffer[ 8], &potRealValues[potIndexValue[10]], 4);
    
    memcpy(&matlabBuffer[12], &sCellValues.cells[ 9].cellPowerFloat, 4);
    memcpy(&matlabBuffer[16], &sCellValues.cells[10].cellPowerFloat, 4);
    
    AddDataToMatlabFifo(matlabBuffer, 20);
    
    if (iteration <= 60000)
    {
      iteration++;
    }
    else
    {
      oMultiUnitDone = 1;
    }
    
    // Multi-Unit algorithm
    gradError = sCellValues.cells[10].cellPowerFloat - sCellValues.cells[9].cellPowerFloat;
    
    multiUnitValues.gradError_p.previousValue = multiUnitValues.gradError_p.currentValue;
//    multiUnitValues.gradError_p.currentValue  = (sCellValues.cells[10].cellPowerFloat - sCellValues.cells[9].cellPowerFloat) * multiUnitValues.alphaDividedByDelta;
    multiUnitValues.gradError_p.currentValue  = gradError * multiUnitValues.alphaDividedByDelta;
    
    TustinZ(&multiUnitValues.gradError_p, &multiUnitValues.gradError, multiUnitValues.sampleTime);
    
    if (multiUnitValues.gradError.currentValue > MAX_POT_VALUE)
    {
      multiUnitValues.gradError.currentValue = MAX_POT_VALUE;
    }
    
    if (multiUnitValues.gradError.currentValue < WIPER_VALUE)
    {
      multiUnitValues.gradError.currentValue = WIPER_VALUE;
    }
    
    potValue = ComputePotValue(multiUnitValues.gradError.currentValue);
    
    potIndexValue[ 9] = potValue;
    potIndexValue[10] = potIndexValue[9] + multiUnitValues.deltaByte;
    
    SetPot(2, 1, potIndexValue[ 9]);
    SetPot(2, 2, potIndexValue[10]);
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