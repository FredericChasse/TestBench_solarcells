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
#include "Potentiometer.h"


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

typedef struct sMultiUnitValues
{
  sTustinValue gradError      // Used in the integral
              ,gradError_p
              ;
  float alphaGain;
  float deltaFloat;
  UINT8 deltaByte;
  float initialInputFloat;
  UINT8 initialInputByte;
  float sampleTime;
  float alphaDividedByDelta;
  const UINT16 maxIteration;
} sMultiUnitValues_t;

typedef struct sPsoValues
{
  float c1;
  const float c1f;
  const float c1i;
  float c2;
  const float c2f;
  const float c2i;
  const float omega;
  const UINT8 rMaxByte;
  const UINT8 rMinByte;
  const float rMaxFloat;
  const float rMinFloat;
  const UINT8 nParticles;
  UINT8 pBestByte[16];
  float pBestFloat[16];
  UINT8 particleIndex[16];
  float objFnc[16];
  float maxObjFnc;
  UINT8 gBestByte;
  float gBestFloat;
  const UINT16 maxIteration;
  float particleSpeed[16];
} sPsoValues_t;

#define MAX(x, y)   (x > y ? x : y)


//==============================================================================
// Control Algorithms public functions prototypes
//==============================================================================
void MultiUnit (void);
void ParticleSwarmOptimization (void);
void Caracterization (void);
void TustinZ    (sTustinValue *input, sTustinValue *output, float acqTime);
void InitRandomValue(void);
void GetRandomValue(float *value, float max);
void SetPotInitialCondition (void);

#endif	/* __CONTROL_ALGORITHM_H__ */

