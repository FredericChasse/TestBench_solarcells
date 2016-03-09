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

UINT16 randValue = 0;

//sTustinValue  gradError   = {0}
//             ,gradError_p = {0}
//             ;

sMultiUnitValues_t multiUnitValues = 
{
//  .alphaGain            = 3900000000
  .deltaFloat           = 39.21568627450980392156862745098
 ,.deltaByte            = 10
 ,.gradError_p          = {0}
 ,.gradError            = {0}
 ,.initialInputFloat    = 442.1568627450980392156862745098
 ,.initialInputByte     = 100
 ,.sampleTime           = 0.04
 ,.alphaDividedByDelta  = 10000000
};

sPsoValues_t psoValues = 
{
   .c1            = 1
  ,.c2            = 1
  ,.gBestFloat    = 0
  ,.gBestByte     = 0
  ,.nParticles    = 3
  ,.omega         = 1
  ,.maxObjFnc     = 0
  ,.objFnc        = {0}
  ,.particleIndex = {0}
  ,.pBestByte     = {0}
  ,.pBestFloat    = {0}
  ,.particleSpeed = {0}
  ,.rMaxByte      = 255
  ,.rMaxFloat     = MAX_POT_VALUE
  ,.rMinByte      = 0
  ,.rMinFloat     = WIPER_VALUE
  ,.maxIteration  = 100
};

//==============================================================================
// Control Algorithms functions
//==============================================================================

void InitRandomValue(void)
{
//  *value = ReadCoreTimer();
//  *value = ReadCoreTimer() * 2.3283064370807973754314699618685e-10;
//  *value = (0xFFFF & ReadCoreTimer()) * 1.5259021896696421759365224689097e-5;
//  UINT16 temp = ReadCoreTimer() & 0xFFFF;
//  temp = (temp << 1) | ((((temp >> 3) & 1) ^ ((temp >> 12) & 1)) ^ (((temp >> 15) & 1) ^ ((temp >> 14) & 1)) );
//  *value = temp * 1.5259021896696421759365224689097e-5;
  
  randValue = 0xFFFF & ReadCoreTimer();
}

/*
 *        MSB                                                                       LSB
 *       _______________________________________________________________________________
 *  <---| 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |<------
 *      |____|____|____|____|____|____|____|____|____|____|____|____|____|____|____|____|       |
 *        |    |         |                                             |   _____                |
 *        |    |         |                                              ->| XOR |__             |
 *        |    |          ----------------------------------------------->|_____|  |    _____   |
 *        |    |                               _____                                -->| XOR |__|
 *        |     ----------------------------->| XOR |--------------------------------->|_____|
 *         ---------------------------------->|_____|
 */
void GetRandomValue(float *value, float max)
{
  randValue = (randValue << 1) |  ( (((randValue >> 3 ) & 1) ^ ((randValue >> 12) & 1)) 
                                  ^ (((randValue >> 15) & 1) ^ ((randValue >> 14) & 1)) 
                                  );
  
  *value = (float) randValue * 1.5259021896696421759365224689097e-5 * max;
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
      GetRandomValue(&value, 255);
      potIndexValue[i] = (UINT8) value;
    }
    
    SetPot(2, 0, potIndexValue[ 8]);
    SetPot(2, 1, potIndexValue[ 9]);
    SetPot(2, 2, potIndexValue[10]);
    
    psoValues.pBestByte [ 8] = potIndexValue[ 8];
    psoValues.pBestByte [ 9] = potIndexValue[ 9];
    psoValues.pBestByte [10] = potIndexValue[10];
    
    psoValues.pBestFloat[ 8] = potRealValues[potIndexValue[ 8]];
    psoValues.pBestFloat[ 9] = potRealValues[potIndexValue[ 9]];
    psoValues.pBestFloat[10] = potRealValues[potIndexValue[10]];
    
    psoValues.particleIndex[0] = 8;
    psoValues.particleIndex[1] = 9;
    psoValues.particleIndex[2] = 10;
    
    psoValues.maxObjFnc = 0;
    psoValues.gBestByte = 0;
    psoValues.gBestFloat = WIPER_VALUE;
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
  UINT16 potValue;
  
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
    
    potValue = ComputePotValueFloat2Dec(multiUnitValues.gradError.currentValue);
    
    if ( (potValue + multiUnitValues.deltaByte) > 255 )
    {
      potValue = 255 - multiUnitValues.deltaByte;
//      ComputePotValueDec2Float(potValue, &multiUnitValues.gradError.currentValue);
      multiUnitValues.gradError.currentValue = potRealValues[potValue];
    }
    
    potIndexValue[ 9] = potValue;
    potIndexValue[10] = potIndexValue[9] + multiUnitValues.deltaByte;
    
    SetPot(2, 1, potIndexValue[ 9]);
    SetPot(2, 2, potIndexValue[10]);
    
    if (iteration <= 60000)
    {
      iteration++;
    }
    else
    {
      oMultiUnitDone = 1;
    }
  }
  else
  {
    LED1_ON;
    LED2_ON;
  }
}


void ParticleSwarmOptimization (void)
{
  UINT8 matlabBuffer[100];
  UINT8 i;
  UINT8 index;
  float fIteration;
  float rand1, rand2;
  
  if (!oPsoDone)
  {
    // Send data to MATLAB
    fIteration = iteration;
    memcpy(&matlabBuffer[ 0], &fIteration, 4);
    
    memcpy(&matlabBuffer[ 4], &potRealValues[potIndexValue[ 8]], 4);
    memcpy(&matlabBuffer[ 8], &potRealValues[potIndexValue[ 9]], 4);
    memcpy(&matlabBuffer[12], &potRealValues[potIndexValue[10]], 4);
    
    memcpy(&matlabBuffer[16], &sCellValues.cells[ 8].cellPowerFloat, 4);
    memcpy(&matlabBuffer[20], &sCellValues.cells[ 9].cellPowerFloat, 4);
    memcpy(&matlabBuffer[24], &sCellValues.cells[10].cellPowerFloat, 4);
    
    AddDataToMatlabFifo(matlabBuffer, 28);
    
    // PSO algorithm
    psoValues.maxObjFnc = 0;
    for (i = 0; i < psoValues.nParticles; i++)
    {
      index = psoValues.particleIndex[i];
      
      // Pbest
      if (sCellValues.cells[index].cellPowerFloat > psoValues.objFnc[index])
      {
        psoValues.objFnc    [index] = sCellValues.cells[index].cellPowerFloat;
        psoValues.pBestByte [index] = potIndexValue    [index];
        psoValues.pBestFloat[index] = potRealValues    [potIndexValue[index]];
      }
      
      // Gbest
      if (psoValues.maxObjFnc < sCellValues.cells[index].cellPowerFloat)
      {
        psoValues.maxObjFnc  = sCellValues.cells[index].cellPowerFloat;
        psoValues.gBestByte  = index;
        psoValues.gBestFloat = potRealValues[index];
      }
    }
    
    // Particles' speed
    float nextPos;
    
    for (i = 0; i < psoValues.nParticles; i++)
    {
      index = psoValues.particleIndex[i];
      
      GetRandomValue(&rand1, 1);
      GetRandomValue(&rand2, 1);
      
      psoValues.particleSpeed[index] = psoValues.c1 * rand1 * (psoValues.pBestFloat[index] - potRealValues[index])
                                     + psoValues.c2 * rand2 * (psoValues.gBestFloat        - potRealValues[index])
                                     ;
      
      nextPos = psoValues.omega * potRealValues[potIndexValue[index]];
      if (nextPos > MAX_POT_VALUE)
      {
        potIndexValue[index] = 255;
      }
      else if (nextPos < WIPER_VALUE)
      {
        potIndexValue[index] = 0;
      }
      else
      {
        potIndexValue[index] = ComputePotValueFloat2Dec(nextPos);
      }
    }
    
    SetPot(2, 0, potIndexValue[ 8]);
    SetPot(2, 1, potIndexValue[ 9]);
    SetPot(2, 2, potIndexValue[10]);
    
    if (iteration < psoValues.maxIteration)
    {
      iteration++;
    }
    else
    {
      oPsoDone = 1;
    }
  }
  else
  {
    LED1_ON;
    LED2_ON;
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