//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : HeaderTemplate.h
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the header file for the functions of the LED driver PCA9685.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef __LED_DRIVER_H__
#define	__LED_DRIVER_H__

#include "Setup.h"


//==============================================================================
// Public functions prototypes
//==============================================================================

/**************************************************************
 * Function name  : ShutdownLedDriver
 * Purpose        : Shutdown all LEDs at once.
 * Arguments      : None.
 * Returns        : None.
 *************************************************************/
inline void ShutdownLedDriver (void);

/**************************************************************
 * Function name  : TurnOnLedDriver
 * Purpose        : Turn on all LEDs at once.
 * Arguments      : None.
 * Returns        : None.
 *************************************************************/
inline void TurnOnLedDriver (void);

/**************************************************************
 * Function name  : InitLedDriver
 * Purpose        : Initialize all registers with LEDs at 0.
 * Arguments      : None.
 * Returns        : None.
 *************************************************************/
inline void InitLedDriver (void);

/**************************************************************
 * Function name  : SetLedPwm
 * Purpose        : Set the duty cycle of one LED.
 * Arguments      : UINT8 numLed : number of the LED to set. 0 - 15
 *                  UINT16 dutyCycle : duty cycle in percentage. Precision 
 *                  of 0.1%. Ex: 50.5% = 505. Max = 1000 (100.0%).
 * Returns        : -1 on failure, 0 on success.
 *************************************************************/
inline INT8 SetLedDutyCycle (UINT8 numLed, UINT16 dutyCycle);


//==============================================================================
// Macro definitions
//==============================================================================


//==============================================================================
// Variable declarations
//==============================================================================
I2C_7_BIT_ADDRESS ledDriver;

// PCA9685 Registers
//=======================================

// MODE1 register
typedef struct 
{
  union
  {
    struct
    {
      UINT8  allcall  : 1
            ,sub3     : 1
            ,sub2     : 1
            ,sub1     : 1
            ,sleep    : 1
            ,ai       : 1
            ,extclk   : 1
            ,restart  : 1
            ;
    } bits;
    UINT8 word;
  } data;
  
  UINT8 regAdd;
} sMode1Reg_t;

// MODE2 register
typedef struct 
{
  union
  {
    struct
    {
      UINT8  outne  : 2
            ,outdrv : 1
            ,och    : 1
            ,invrt  : 1
            ,       : 3
            ;
    } bits;
    UINT8 word;
  } data;
  
  UINT8 regAdd;
} sMode2Reg_t;

// ALL_LED_ON_L register
typedef struct 
{
  union
  {
    UINT16 word;
    struct 
    {
      union
      {
        struct
        {
          UINT8  msb    : 4
                ,fullOn : 1
                ,       : 3
                ;
        } bits;
        UINT8 word;
      } dataHigh;
      UINT8 dataLow;
    } data;
  } bytes;
  
  UINT8 regAddHigh;
  UINT8 regAddLow;
} sLedOn_t;

// ALL_LED_OFF_L register
typedef struct 
{
  union
  {
    UINT16 word;
    struct 
    {
      union
      {
        struct
        {
          UINT8  msb      : 4
                ,fullOff  : 1
                ,         : 3
                ;
        } bits;
        UINT8 word;
      } dataHigh;
      UINT8 dataLow;
    } data;
  } bytes;
  
  UINT8 regAddHigh;
  UINT8 regAddLow;
} sLedOff_t;

// PRE_SCALE register
typedef struct 
{
  UINT8 value;
  
  UINT8 regAdd;
} sPreScaleReg_t;


#endif	/* __LED_DRIVER_H__ */

