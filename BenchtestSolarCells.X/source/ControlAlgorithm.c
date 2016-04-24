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

UINT16 perturbation = 300;

extern UINT8 matlabPacketSize;

extern UINT8 potIndexValue[16];
UINT8 potPreviousIndexValue[16];

extern const float potRealValues[256];
extern const float potRealValuesInverse[256];

UINT32 iteration = 0;

UINT16 randValue = 0;

UINT8 testUINT8 = 0;
float testFLOAT = 0;

//sTustinValue  gradError   = {0}
//             ,gradError_p = {0}
//             ;

sMultiUnitValues_t multiUnitValues = 
{
//  .alphaGain            = 3900000000
//  .deltaFloat           = 39.21568627450980392156862745098
// ,.deltaByte            = 10
  .deltaFloat           = 54.901960784314
 ,.deltaByte            = 14
 ,.gradError_p          = {0}
 ,.gradError            = {0}
 ,.initialInputFloat    = 442.1568627450980392156862745098
 ,.initialInputByte     = 100
 ,.sampleTime           = 0.08
 ,.alphaDividedByDelta  = 1500000
 ,.maxIteration         = 301
 ,.perturbIteration     = 100
 ,.oChangeHasOccured    = 0
 ,.oDoPerturb           = 0    
 ,.unitIndex            = {0}
};

sPsoValues_t psoValues = 
{
   .c1                = 1
  ,.c1i               = 1
  ,.c1f               = 2.5
  ,.c1Delta           = 0
  ,.c2                = 1.5
  ,.c2i               = 1.5
  ,.c2f               = 0
  ,.c2Delta           = 0
  ,.gBestFloat        = 0
  ,.gBestByte         = 0
  ,.nParticles        = 3
//  ,.omega             = 0.5
  ,.omega             = 0.6
  ,.maxObjFnc         = 0
  ,.objFnc            = {0}
  ,.previousObjFnc    = {0}
  ,.particleIndex     = {0}
  ,.pBestByte         = {0}
  ,.pBestFloat        = {0}
  ,.particleSpeed     = {0}
  ,.rMaxByte          = 255
  ,.rMaxFloat         = MAX_POT_VALUE
  ,.rMinByte          = 0
  ,.rMinFloat         = WIPER_VALUE
//  ,.maxIteration      = 56
  ,.maxIteration      = 224
  ,.perturbIteration  = 100
  ,.oChangeHasOccured = 0
  ,.oDoPerturb        = 0
  ,.detectPrecision   = 0.05
//  ,.detectPrecision   = 0.02
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
  randValue = ((randValue & 0x03) << 14)
            | ((randValue & 0x04) << 11) | ((randValue & 0x08)   <<  9)
            | ((randValue & 0xF0) <<  4) | ((randValue & 0xFF00) >>  8)
            ;
  
  randValue &= 0xACE1;
//  randValue = 0xACE1;
}

/*
 *  ____________________________________________________________________________________________
 * |                                                                                            |
 * |      MSB                                                                       LSB         |
 * |     _______________________________________________________________________________        |
 *  --->| 15 | 14 | 13 | 12 | 11 | 10 |  9 |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |--->   |
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
  UINT16 bitValue =   (((randValue >> 3 ) & 1) ^ ((randValue >> 12) & 1)) 
                    ^ (((randValue >> 15) & 1) ^ ((randValue >> 14) & 1));
//  UINT16 bitValue = ( (randValue >> 0 ) ^ (randValue >> 2) ^ (randValue >> 3) ^ (randValue >> 5) ) & 1;
  
  randValue = (randValue >> 1) | (bitValue << 15);
  
//  randValue = (randValue << 1) |  ( (((randValue >> 3 ) & 1) ^ ((randValue >> 12) & 1)) 
//                                  ^ (((randValue >> 15) & 1) ^ ((randValue >> 14) & 1)) 
//                                  );
  
  *value = (float) randValue * 1.5259021896696421759365224689097e-5 * max;
}


void SetPotInitialCondition (void)
{
  UINT8 i = 0;
  UINT8 index = 0;
  float value = 0;
  float potDelta;
  
  if (oCaracMode)
  {
    matlabPacketSize = MATLAB_PACKET_SIZE_CARAC;
    for (i = 0; i < 16; i++)
    {
      potIndexValue[i] = 0;
    }
    SetPotAllUnits(2, potIndexValue[0]);
    SetPotAllUnits(3, potIndexValue[0]);
  }
  else if (oPsoMode)
  {
    matlabPacketSize = MATLAB_PACKET_SIZE_PSO;
    
    psoValues.nParticles = 3;
//    psoValues.nParticles = 4;
    
    potDelta = (MAX_POT_VALUE - WIPER_VALUE) / (psoValues.nParticles + 1);
    
    psoValues.particleIndex[0] = 7 + 2;
    psoValues.particleIndex[1] = 7 + 3;
    psoValues.particleIndex[2] = 7 + 1;
//    psoValues.particleIndex[3] = 7 + 8;
    
    for (i = 0; i < psoValues.nParticles; i++)
    {
      index = psoValues.particleIndex[i];
      
//      GetRandomValue(&value, 255);  // Get random value between 0 and 255
//      potIndexValue[index] = (UINT8) (value + 0.5);
      
//      GetRandomValue(&value, 2*potDelta);  // Get random value between 0 and 2*potDelta
//      value -= potDelta;  // random value between [-potDelta, potDelta]
//      value = WIPER_VALUE + (i + 1)*potDelta + value;
      
      GetRandomValue(&value, MAX_POT_VALUE - WIPER_VALUE);  // Get random value between 0 and MAX_POT_VALUE - WIPER_VALUE
      value += WIPER_VALUE;  // random value between [WIPERVALUE, MAX_POT_VALUE]
      
      potIndexValue[index] = ComputePotValueFloat2Dec(value);
      potPreviousIndexValue[index] = 0;
      
      psoValues.pBestByte[index]      = potIndexValue[index];
      psoValues.objFnc[index]         = 0;
      psoValues.previousObjFnc[index] = 0;
      psoValues.pBestFloat[index]     = potRealValues[potIndexValue[index]];
      psoValues.particleSpeed[index]  = 0;
    }
    
    for (i = 0; i < psoValues.nParticles; i++)
    {
      SetPot(psoValues.particleIndex[i], potIndexValue[psoValues.particleIndex[i]]);
      psoValues.pBestByte[psoValues.particleIndex[i]] = potIndexValue[psoValues.particleIndex[i]];
      psoValues.pBestFloat[psoValues.particleIndex[i]] = potRealValues[potIndexValue[psoValues.particleIndex[i]]];
    }
//    SetPot(psoValues.particleIndex[0], potIndexValue[psoValues.particleIndex[0]]);
//    SetPot(psoValues.particleIndex[1], potIndexValue[psoValues.particleIndex[1]]);
////    SetPot(10, potIndexValue[10]);
//    SetPot(psoValues.particleIndex[2], potIndexValue[psoValues.particleIndex[2]]);
    
//    psoValues.pBestByte [psoValues.particleIndex[0]] = potIndexValue[psoValues.particleIndex[0]];
//    psoValues.pBestByte [psoValues.particleIndex[1]] = potIndexValue[psoValues.particleIndex[1]];
////    psoValues.pBestByte [10] = potIndexValue[10];
//    psoValues.pBestByte [psoValues.particleIndex[2]] = potIndexValue[psoValues.particleIndex[2]];
    
//    psoValues.pBestFloat[psoValues.particleIndex[0]] = potRealValues[potIndexValue[psoValues.particleIndex[0]]];
//    psoValues.pBestFloat[psoValues.particleIndex[1]] = potRealValues[potIndexValue[psoValues.particleIndex[1]]];
////    psoValues.pBestFloat[10] = potRealValues[potIndexValue[10]];
//    psoValues.pBestFloat[psoValues.particleIndex[2]] = potRealValues[potIndexValue[psoValues.particleIndex[2]]];
    
    psoValues.maxObjFnc = 0;
    psoValues.gBestByte = 0;
    psoValues.gBestFloat = WIPER_VALUE;
    
    psoValues.oChangeHasOccured = 0;
    psoValues.oDoPerturb = 0;
    
    psoValues.c1 = psoValues.c1i;
    psoValues.c2 = psoValues.c2i;
    
    psoValues.c1Delta = (psoValues.c1f - psoValues.c1i) / psoValues.maxIteration;
    psoValues.c2Delta = (psoValues.c2f - psoValues.c2i) / psoValues.maxIteration;
  }
  else if (oMultiUnitMode)
  {
    matlabPacketSize = MATLAB_PACKET_SIZE_MULTI_UNIT;
    
    multiUnitValues.unitIndex[0] = 7 + 3;
    multiUnitValues.unitIndex[1] = 7 + 2;
    
    potIndexValue[multiUnitValues.unitIndex[0]] = multiUnitValues.initialInputByte;
    potIndexValue[multiUnitValues.unitIndex[1]] = potIndexValue[multiUnitValues.unitIndex[0]] + multiUnitValues.deltaByte;
    
    multiUnitValues.gradError_p.currentValue = 0;
    multiUnitValues.gradError_p.previousValue = 0;
    multiUnitValues.gradError.previousValue = 0;
    multiUnitValues.gradError.currentValue = multiUnitValues.initialInputFloat;
    
    SetPot(multiUnitValues.unitIndex[0], potIndexValue[multiUnitValues.unitIndex[0]]);
    SetPot(multiUnitValues.unitIndex[1], potIndexValue[multiUnitValues.unitIndex[1]]);
    
    multiUnitValues.oChangeHasOccured = 0;
    multiUnitValues.oDoPerturb = 0;
  }
}


void Caracterization (void)
{
  UINT8 matlabBuffer[40];
  float fPotValue;
  
  if (!oCaracDone)
  {
    fPotValue = potIndexValue[0];
    memcpy(&matlabBuffer[ 0], &fPotValue, 4);
    memcpy(&matlabBuffer[ 4], &sCellValues.cells[ 8].cellPowerFloat, 4);
    memcpy(&matlabBuffer[ 8], &sCellValues.cells[ 9].cellPowerFloat, 4);
    memcpy(&matlabBuffer[12], &sCellValues.cells[10].cellPowerFloat, 4);
    memcpy(&matlabBuffer[16], &sCellValues.cells[11].cellPowerFloat, 4);
    memcpy(&matlabBuffer[20], &sCellValues.cells[12].cellPowerFloat, 4);
    memcpy(&matlabBuffer[24], &sCellValues.cells[13].cellPowerFloat, 4);
    memcpy(&matlabBuffer[28], &sCellValues.cells[14].cellPowerFloat, 4);
    memcpy(&matlabBuffer[32], &sCellValues.cells[15].cellPowerFloat, 4);
    
    AddDataToMatlabFifo(matlabBuffer, 36);

    if (potIndexValue[0] < 255)
    {
      potIndexValue[0]++;
      SetPotAllUnits(2, potIndexValue[0]);
      SetPotAllUnits(3, potIndexValue[0]);
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
  UINT8 matlabBuffer[20];
  float fIteration;
  float gradError;
  UINT16 potValue;
  
  if (!oMultiUnitDone)
  {
    // Send data to MATLAB
    fIteration = iteration;
    memcpy(&matlabBuffer[ 0], &fIteration, 4);
    
    memcpy(&matlabBuffer[ 4], &potRealValues[potIndexValue[multiUnitValues.unitIndex[0]]], 4);
    memcpy(&matlabBuffer[ 8], &potRealValues[potIndexValue[multiUnitValues.unitIndex[1]]], 4);
    
    memcpy(&matlabBuffer[12], &sCellValues.cells[multiUnitValues.unitIndex[0]].cellPowerFloat, 4);
    memcpy(&matlabBuffer[16], &sCellValues.cells[multiUnitValues.unitIndex[1]].cellPowerFloat, 4);
    
    AddDataToMatlabFifo(matlabBuffer, 20);
    
    // Multi-Unit algorithm
    gradError = sCellValues.cells[multiUnitValues.unitIndex[1]].cellPowerFloat 
              - sCellValues.cells[multiUnitValues.unitIndex[0]].cellPowerFloat
              ;
    
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
    
//    if ( (potValue + multiUnitValues.deltaByte) > 255 )
//    {
//      potValue = 255 - multiUnitValues.deltaByte;
//      multiUnitValues.gradError.currentValue = potRealValues[potValue];
//    }
    if ( (potValue + multiUnitValues.deltaByte/2) > 255 )
    {
      potValue = 255 - multiUnitValues.deltaByte/2;
      multiUnitValues.gradError.currentValue = potRealValues[potValue];
    }
    if ( ((float) potValue - (float) multiUnitValues.deltaByte/2) < 0 )
    {
      potValue = multiUnitValues.deltaByte/2;
      multiUnitValues.gradError.currentValue = potRealValues[potValue];
    }
    
//    potIndexValue[multiUnitValues.unitIndex[0]] = potValue;
//    potIndexValue[multiUnitValues.unitIndex[1]] = potIndexValue[multiUnitValues.unitIndex[0]] + multiUnitValues.deltaByte;
    potIndexValue[multiUnitValues.unitIndex[0]] = potValue - multiUnitValues.deltaByte/2;
    potIndexValue[multiUnitValues.unitIndex[1]] = potValue + multiUnitValues.deltaByte/2;
    
    SetPot(multiUnitValues.unitIndex[0], potIndexValue[multiUnitValues.unitIndex[0]]);
    SetPot(multiUnitValues.unitIndex[1], potIndexValue[multiUnitValues.unitIndex[1]]);
    
    if (iteration == multiUnitValues.perturbIteration)
    {
      if (multiUnitValues.oDoPerturb)
      {
//        UINT16 perturbation = 100;
        SetLedDutyCycle( 0, perturbation);
        SetLedDutyCycle( 1, perturbation);
        SetLedDutyCycle( 2, perturbation);
        SetLedDutyCycle( 3, perturbation);
        SetLedDutyCycle(12, perturbation);
        SetLedDutyCycle(13, perturbation);
        SetLedDutyCycle(14, perturbation);
        SetLedDutyCycle(15, perturbation);
      }
    }
    
    if (iteration <= multiUnitValues.maxIteration)
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
  UINT8 matlabBuffer[psoValues.nParticles*8 + 4];
  UINT8 i;
  UINT8 index;
  float fIteration;
  float rand1, rand2, rand3;
  
  if (!oPsoDone)
  {
    // Send data to MATLAB
    fIteration = iteration;
    memcpy(&matlabBuffer[ 0], &fIteration, 4);
    
    for (i = 0; i < psoValues.nParticles; i++)
    {
      memcpy(&matlabBuffer[i*4 + 4], &potRealValues[potIndexValue[psoValues.particleIndex[i]]], 4);
      memcpy(&matlabBuffer[i*4 + 4*psoValues.nParticles + 4], &sCellValues.cells[psoValues.particleIndex[i]].cellPowerFloat, 4);
//      memcpy(&matlabBuffer[i*4 + 28], &psoValues.pBestFloat[psoValues.particleIndex[i]], 4);
//      memcpy(&matlabBuffer[i*4 + 44], &psoValues.particleSpeed[psoValues.particleIndex[i]], 4);
    }
//    memcpy(&matlabBuffer[40], &psoValues.gBestFloat       , 4);
    
//    memcpy(&matlabBuffer[ 4], &potRealValues[potIndexValue[psoValues.particleIndex[0]]], 4);
//    memcpy(&matlabBuffer[ 8], &potRealValues[potIndexValue[psoValues.particleIndex[1]]], 4);
////    memcpy(&matlabBuffer[12], &potRealValues[potIndexValue[10]], 4);
//    memcpy(&matlabBuffer[12], &potRealValues[potIndexValue[psoValues.particleIndex[2]]], 4);
    
//    memcpy(&matlabBuffer[16], &sCellValues.cells[psoValues.particleIndex[0]].cellPowerFloat, 4);
//    memcpy(&matlabBuffer[20], &sCellValues.cells[psoValues.particleIndex[1]].cellPowerFloat, 4);
////    memcpy(&matlabBuffer[24], &sCellValues.cells[10].cellPowerFloat, 4);
//    memcpy(&matlabBuffer[24], &sCellValues.cells[psoValues.particleIndex[2]].cellPowerFloat, 4);

//    memcpy(&matlabBuffer[28], &psoValues.pBestFloat   [psoValues.particleIndex[0]], 4);
//    memcpy(&matlabBuffer[32], &psoValues.pBestFloat   [psoValues.particleIndex[1]], 4);
////    memcpy(&matlabBuffer[36], &psoValues.pBestFloat   [10], 4);
//    memcpy(&matlabBuffer[36], &psoValues.pBestFloat   [psoValues.particleIndex[2]], 4);
    

//    memcpy(&matlabBuffer[44], &psoValues.particleSpeed[psoValues.particleIndex[0]], 4);
//    memcpy(&matlabBuffer[48], &psoValues.particleSpeed[psoValues.particleIndex[1]], 4);
////    memcpy(&matlabBuffer[52], &psoValues.particleSpeed[10], 4);
//    memcpy(&matlabBuffer[52], &psoValues.particleSpeed[psoValues.particleIndex[2]], 4);
    
//    AddDataToMatlabFifo(matlabBuffer, 56);
    AddDataToMatlabFifo(matlabBuffer, psoValues.nParticles*8 + 4);
    
    // PSO algorithm
    psoValues.maxObjFnc = 0;
    for (i = 0; i < psoValues.nParticles; i++)
    {
      index = psoValues.particleIndex[i];
      
      // Pbest
      if (sCellValues.cells[index].cellPowerFloat > psoValues.objFnc[index])
      {
        psoValues.pBestByte [index] = potIndexValue[index];
        psoValues.pBestFloat[index] = potRealValues[potIndexValue[index]];
      }
      psoValues.previousObjFnc[index] = psoValues.objFnc[index];
      psoValues.objFnc[index] = sCellValues.cells[index].cellPowerFloat;
      
      // Gbest
      if (psoValues.maxObjFnc < sCellValues.cells[index].cellPowerFloat)
      {
        psoValues.maxObjFnc  = sCellValues.cells[index].cellPowerFloat;
        psoValues.gBestByte  = potIndexValue[index];
        psoValues.gBestFloat = potRealValues[potIndexValue[index]];
      }
    }
    
//    // Acceleration coefficients
//    psoValues.c1 = (psoValues.c1f - psoValues.c1i)*(iteration / psoValues.maxIteration) + psoValues.c1i;    // Cognitive parameter
//    psoValues.c2 = (psoValues.c2f - psoValues.c2i)*(iteration / psoValues.maxIteration) + psoValues.c2i;    // Social parameter
    
//    testFLOAT = ABS(psoValues.objFnc[index] - psoValues.previousObjFnc[index])/psoValues.objFnc[index];
//    testUINT8 = 0;
    // Detect perturbation
    if (iteration >= 3)
    {
      for (i = 0; i < psoValues.nParticles; i++)
      {
        index = psoValues.particleIndex[i];
        
        if (potIndexValue[index] == potPreviousIndexValue[index])
        {
          float error = (psoValues.objFnc[index] - psoValues.previousObjFnc[index]) / psoValues.objFnc[index];
          if (error < 0)
          {
            error = -error;
          }
          if (error >= psoValues.detectPrecision)
//          if ( (ABS(psoValues.objFnc[index] - psoValues.previousObjFnc[index]) 
//             /  psoValues.objFnc[index]) 
//             >= psoValues.detectPrecision 
//             )
          {
            psoValues.oChangeHasOccured = 1;
            i = psoValues.nParticles + 1;
          }
        }
      }
    }
    
//    fIteration = psoValues.oChangeHasOccured;
//    memcpy(&matlabBuffer[28], &fIteration, 4);
//    AddDataToMatlabFifo(matlabBuffer, 56);
    
    // Particles' speed
    
    for (i = 0; i < psoValues.nParticles; i++)
    {
      index = psoValues.particleIndex[i];
      
      GetRandomValue(&rand1, 1);  // Get random value between 0 and 1
      GetRandomValue(&rand2, 1);
      
      if (iteration == 0)
      {
        GetRandomValue(&rand3, 20);
        rand3 -= 10;
        psoValues.particleSpeed[index]  = rand3
                                        + psoValues.c1 * rand1 * (psoValues.pBestFloat[index] - potRealValues[potIndexValue[index]])  // Cognitive behavior
                                        + psoValues.c2 * rand2 * (psoValues.gBestFloat        - potRealValues[potIndexValue[index]])  // Social behavior
                                        ;
      }
      else
      {
        psoValues.particleSpeed[index]  = psoValues.omega * psoValues.particleSpeed[index]                                            // Inertia
                                        + psoValues.c1 * rand1 * (psoValues.pBestFloat[index] - potRealValues[potIndexValue[index]])  // Cognitive behavior
                                        + psoValues.c2 * rand2 * (psoValues.gBestFloat        - potRealValues[potIndexValue[index]])  // Social behavior
                                        ;
      }
    }
    
    // Acceleration coefficients
    psoValues.c1 = psoValues.c1 + psoValues.c1Delta;    // Cognitive parameter
    psoValues.c2 = psoValues.c2 + psoValues.c2Delta;    // Social parameter
    
    // Particles' positions
    float nextPos;
    
    for (i = 0; i < psoValues.nParticles; i++)
    {
      index = psoValues.particleIndex[i];
      
      if (!psoValues.oChangeHasOccured)
      {
        nextPos = potRealValues[potIndexValue[index]] + psoValues.particleSpeed[index];
        potPreviousIndexValue[index] = potIndexValue[index];
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
      else
      {
        GetRandomValue(&rand1, 200);  // Get random value between 0 and 100
        rand1 -= 100;                 // Random between -100 and 100
        
        nextPos = potRealValues[potIndexValue[index]] + rand1;
        potPreviousIndexValue[index] = potIndexValue[index];
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
        
        psoValues.pBestByte[index]  = potIndexValue[index];
        psoValues.pBestFloat[index] = potRealValues[potIndexValue[index]];
        
        GetRandomValue(&rand1, 20);  // Get random value between 0 and 10
        rand1 -= 10;                 // Random between -5 and 5
        
        psoValues.particleSpeed[index] = rand1;
      }
    }
    
    if (psoValues.oChangeHasOccured)
    {
      psoValues.maxObjFnc = 0;
      psoValues.gBestByte  = potIndexValue[psoValues.particleIndex[1]];
      psoValues.gBestFloat = potRealValues[psoValues.gBestByte];
      psoValues.c1 = psoValues.c1i;
      psoValues.c2 = psoValues.c2i;
      psoValues.oChangeHasOccured = 0;
    }
    
    for (i = 0; i < psoValues.nParticles; i++)
    {
      SetPot(psoValues.particleIndex[i], potIndexValue[psoValues.particleIndex[i]]);
    }
//    SetPot(psoValues.particleIndex[0], potIndexValue[psoValues.particleIndex[0]]);
//    SetPot(psoValues.particleIndex[1], potIndexValue[psoValues.particleIndex[1]]);
////    SetPot(10, potIndexValue[10]);
//    SetPot(psoValues.particleIndex[2], potIndexValue[psoValues.particleIndex[2]]);
    
    if (iteration == psoValues.perturbIteration)
    {
      if (psoValues.oDoPerturb)
      {
//        UINT16 perturbation = 100;
        SetLedDutyCycle( 0, perturbation);
        SetLedDutyCycle( 1, perturbation);
        SetLedDutyCycle( 2, perturbation);
        SetLedDutyCycle( 3, perturbation);
        SetLedDutyCycle(12, perturbation);
        SetLedDutyCycle(13, perturbation);
        SetLedDutyCycle(14, perturbation);
        SetLedDutyCycle(15, perturbation);
      }
    }
    
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
 * Desc     : Discrete integrator using Tustin's method
 * Graphic example :
 *
 *         _______
 *  X(s)  |   1   |  Y(s)
 * ------>| ----- |------>
 *        |   s   |
 *        |_______|
 * 
 *   Y(s)      1        Y(z)     T     z + 1
 *  -----  =  ---  ==> ----- =  --- * -------
 *   X(s)      s        X(z)     2     z - 1
 *
 *  iLaplace {Z} => y(n) = y(n-1) + T/2 * ( x(n-1) + x(n) )
 *
 */
void TustinZ (sTustinValue *input, sTustinValue *output, float acqTime)
{
  output->previousValue = output->currentValue;
  output->currentValue  = output->previousValue + acqTime/2 * (input->currentValue + input->previousValue);
}