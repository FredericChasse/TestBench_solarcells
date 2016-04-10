//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Setup.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the setup of the system. It contains the
//           initialization functions.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : For ChinookLib to be useable, it must be cloned on your hard
//           drive so the path
//               "..\..\..\ChinookLib\ChinookLib.X\headers\ChinookLib.h"
//           references an existing file.
//
//           Function names can and should be renamed by the user to improve the
//           readability of the code. Also, the LED used for testing errors in
//           TimerInit is a LED on the MAX32 development board. Developpers
//           should test for errors by the means (hardware of software) they
//           judge are the best.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "..\headers\Setup.h"
#include "..\headers\Interrupts.h"
#include "..\headers\SkadiFunctions.h"


//==============================================================================
//	Variable Declaration
//==============================================================================

/***********************************
 * Table of functions used in Skadi
 **********************************/
sSkadiCommand_t skadiCommandTable[] =
{
   {"led"         , LedToggle       , 1 , "     | flash Led 1 or 2               | 1 arg"}    // 1 argument
  ,{"pwm"         , SetPwmSkadi     , 2 , "     | Set the duty cycle of a LED    | 2 arg"}    // 2 arguments
  ,{"pwmall"      , SetAllPwmSkadi  , 1 , "     | Set the duty cycle of all LEDs | 1 arg"}    // 1 argument
  ,{"pot"         , SetPotSkadi     , 3 , "     | Set the value of a single pot  | 3 arg"}    // 3 arguments
  ,{"allpot"      , SetAllPotSkadi  , 2 ,    "  | Set the value of 4 pots        | 2 arg"}    // 2 arguments
  ,{"clc"         , ClearScreen     , 0 , "     | Clear terminal window          | 0 arg"}    // 0 arguments
};


//==============================================================================
//	INIT FUNCTIONS
//==============================================================================


//===========================
//	INIT TIMERS
//===========================
void InitTimer(void)
{

  INT32 timerCounterValue = 0;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	Open timers
//%%%%%%%%%%%%%%%%%%%%%%%%%%%
//  timerCounterValue = Timer.Open(TIMER_1, 500, SCALE_MS);   // Open Timer 1 with a period of 500 ms
//  if (timerCounterValue < 0)
//  {
//    LED1_ON;
//  }
//  timerCounterValue = Timer.Open(TIMER_2, 500, SCALE_US);   // Open Timer 2 with a period of 500 us
//  if (timerCounterValue < 0)
//  {
//    LED1_ON;
//  }
//  timerCounterValue = Timer.Open(TIMER_3, 50, SCALE_MS);   // Timer used for ADC
//  timerCounterValue = Timer.Open(TIMER_3, 1, SCALE_MS);   // Timer used for ADC
  timerCounterValue = Timer.Open(TIMER_3, 100, SCALE_US);   // Timer used for ADC
  if (timerCounterValue < 0)
  {
    LED1_ON;
  }
//  timerCounterValue = Timer.Open(TIMER_4, 500, SCALE_MS);   // Open Timer 4 with a period of 500 ms
//  if (timerCounterValue < 0)
//  {
//    LED1_ON;
//  }
//  timerCounterValue = Timer.Open(TIMER_5, 500, SCALE_US);   // Open Timer 5 with a period of 500 us
//  if (timerCounterValue < 0)
//  {
//    LED1_ON;
//  }

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	Configure timer interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  INT8 err = 0;
//  Timer.ConfigInterrupt(TIMER_1, TIMER1_INTERRUPT_PRIORITY, TIMER1_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_1 to the values specified in Interrupt.h
//  Timer.ConfigInterrupt(TIMER_2, TIMER2_INTERRUPT_PRIORITY, TIMER2_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_2 to the values specified in Interrupt.h
  err = Timer.ConfigInterrupt(TIMER_3, TIMER3_INTERRUPT_PRIORITY, TIMER3_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_3 to the values specified in Interrupt.h
  if (err < 0)
  {
    LED1_ON;
  }
//  Timer.ConfigInterrupt(TIMER_4, TIMER4_INTERRUPT_PRIORITY, TIMER4_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_4 to the values specified in Interrupt.h
//  Timer.ConfigInterrupt(TIMER_5, TIMER5_INTERRUPT_PRIORITY, TIMER5_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_5 to the values specified in Interrupt.h

}


//===========================
//	INIT SPI
//===========================
void InitSpi(void)
{
  INT8 err = 0;
  SpiOpenFlags_t oMasterFlags =   SPI_MASTER_MODE
                                | SPI_16_BITS_CHAR
                                | SPI_ENHANCED_BUFFER_MODE
                                | SPI_TX_EVENT_BUFFER_SR_EMPTY
                                | SPI_RX_EVENT_BUFFER_NOT_EMPTY
//                                | SPI_SAMPLE_END_CLK
                                | SPI_DATA_ON_CLK_FEDGE
                                ;

  err = Spi.Open(SPI3, oMasterFlags, 5e6);   // Open the SPI3 as a master at a bitrate of 10 MHz
  if (err < 0)                // Check for errors
  {
    LED1_ON;    // Turn on the LED_DEBUG1
  }

  // SPI interrupts not functionnal as of yet
  Spi.ConfigInterrupt(SPI3, SPI3_INTERRUPT_PRIORITY, SPI3_INTERRUPT_SUBPRIORITY);  // Configure Interrupt for SPI3
}


//===========================
//	INIT PORTS
//===========================
void InitPorts(void)
{
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Set unused pins as input to protect the pins of the microcontroller
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  Port.A.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3      // RA8, RA11-13 non existent
                    | BIT_4  | BIT_5  | BIT_6  | BIT_7
                    | BIT_9  | BIT_10 | BIT_14 | BIT_15 );

  Port.B.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3
                    | BIT_4  | BIT_5  | BIT_6  | BIT_7
                    | BIT_8  | BIT_9  | BIT_10 | BIT_11
                    | BIT_12 | BIT_13 | BIT_14 | BIT_15 );

  Port.C.CloseBits  ( BIT_1  | BIT_2  | BIT_3  | BIT_4      // RC0, RC5-11 non existent
                    | BIT_12 | BIT_13 | BIT_14 | BIT_15 );

  Port.D.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3
                    | BIT_4  | BIT_5  | BIT_6  | BIT_7
                    | BIT_8  | BIT_9  | BIT_10 | BIT_11
                    | BIT_12 | BIT_13 | BIT_14 | BIT_15 );

  Port.E.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3      // RE10-15 non existent
                    | BIT_4  | BIT_5  | BIT_6  | BIT_7
                    | BIT_8  | BIT_9  );

  Port.F.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3      // RF6-7, RF9-11, RF14-15 non existent
                    | BIT_4  | BIT_5  | BIT_8  | BIT_12
                    | BIT_13 );

  Port.G.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3      // RG4-5, RG10-11 non existent
                    | BIT_6  | BIT_7  | BIT_8  | BIT_9
                    | BIT_12 | BIT_13 | BIT_14 | BIT_15 );

//  Port.B.SetPinsAnalogIn(0xFFFF);   // ADC0-15, RB0 = Vref+
  Port.B.SetPinsAnalogIn  ( BIT_0     // Vref+
                          | BIT_12    // POT12
                          | BIT_13    // POT12
                          | BIT_14    // POT12
                          | BIT_15    // POT12
                          );
  
  Port.C.CloseBits        ( BIT_12  // OSC_IN
                          | BIT_13  // GPIO0
                          | BIT_14  // GPIO1
                          | BIT_15  // OSC_OUT
                          );
  
  Port.D.SetPinsDigitalIn ( BIT_2); // SPI_SDI
  Port.D.SetPinsDigitalOut( BIT_0   // LED_OEn
                          | BIT_1   // SPI_CLK
                          | BIT_3   // SPI_SDO
                          | BIT_4   // SPI_CS0
                          | BIT_5   // SPI_CS1
                          | BIT_6   // SPI_CS2
                          | BIT_7   // SPI_CS3
                          | BIT_8   // RST_POT0n
                          | BIT_9   // RST_POT1n
                          | BIT_10  // RST_POT2n
                          | BIT_11  // RST_POT3n
                          );
  
  Port.E.SetPinsDigitalOut( BIT_0   // SHD_POT0n
                          | BIT_1   // SHD_POT1n
                          | BIT_2   // SHD_POT2n
                          | BIT_3   // SHD_POT3n
                          );
  Port.E.CloseBits (BIT_4);         // GPIO2
  
  Port.E.SetPinsDigitalIn ( BIT_5   // GP_SW1
                          | BIT_6   // GP_SW2
                          | BIT_7   // GP_SW3
                          );
  
  Port.F.SetPinsDigitalOut( BIT_0   // LED_DEBUG1
                          | BIT_1   // LED_DEBUG2
                          | BIT_4   // I2C_SDA
                          | BIT_5   // I2C_SCL
                          );
  Port.F.CloseBits        (BIT_3);  // No connect
  
  Port.G.SetPinsDigitalOut( BIT_6   // U2_TX
                          | BIT_8   // U1_TX
                          );
  Port.G.SetPinsDigitalIn ( BIT_7   // U1_RX
                          | BIT_9   // U2_RX
                          ); 
  Port.G.CloseBits        ( BIT_2   // GPIO3 (RG2-3 input only pins)
                          | BIT_3   // No connect (previously LED_OEn)
                          );
  
  Port.D.ClearBits        ( BIT_0   // LED Driver disabled
                          | BIT_8   // SHD_POT0n
                          | BIT_9   // SHD_POT1n
                          | BIT_10  // SHD_POT2n
                          | BIT_11  // SHD_POT3n
                          );
  
  Port.E.SetBits          ( BIT_0   // RST_POT0n
                          | BIT_1   // RST_POT1n
                          | BIT_2   // RST_POT2n
                          | BIT_3   // RST_POT3n
                          );
  
  LED1_OFF;
  LED2_OFF;

}


//===========================
//	INIT UART
//===========================
void InitUart (void)
{

  UartConfig_t       oConfigDbg     = UART_ENABLE_PINS_TX_RX_ONLY;
  UartConfig_t       oConfigMatlab  = UART_ENABLE_PINS_TX_RX_ONLY | UART_ENABLE_HIGH_SPEED;
  UartFifoMode_t     oFifoMode      = UART_INTERRUPT_ON_TX_BUFFER_EMPTY | UART_INTERRUPT_ON_RX_NOT_EMPTY;
  UartLineCtrlMode_t oLineControl   = UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1;

  Uart.Open(UART3, BAUD9600  , oConfigDbg, oFifoMode, oLineControl);   // Open UART 3 as : 9600 BAUD, 1 stop bit, no parity and 8 bits data
  Uart.Open(UART6, BAUD115200, oConfigMatlab, oFifoMode, oLineControl);   // Open UART 6 as : 9600 BAUD, 1 stop bit, no parity and 8 bits data

  Uart.EnableRx(UART3);
  Uart.EnableRx(UART6);

  Uart.EnableTx(UART3);
  Uart.EnableTx(UART6);

  Uart.ConfigInterrupt(UART3, UART3_INTERRUPT_PRIORITY, UART3_INTERRUPT_SUBPRIORITY);
  Uart.ConfigInterrupt(UART6, UART6_INTERRUPT_PRIORITY, UART6_INTERRUPT_SUBPRIORITY);
  
}


//===========================
//	INIT SKADI
//===========================
void InitSkadi(void)
{
//  Skadi.Init(skadiCommandTable, sizeof(skadiCommandTable)/sizeof(sSkadiCommand_t), UART1, FALSE);   // This system does not use UART interrupts
  Skadi.Init(skadiCommandTable, sizeof(skadiCommandTable)/sizeof(sSkadiCommand_t), UART3, TRUE);   // This system uses UART interrupts
}


//===========================
//	INIT I2C
//===========================
void InitI2c(void)
{
  INT8 err;

  I2c.Open(I2C5, I2C_FREQ_400K);
  err = I2c.ConfigInterrupt(I2C5, I2C5_INTERRUPT_PRIORITY, I2C5_INTERRUPT_SUBPRIORITY);
  if (err < 0)
  {
    LED1_ON;
  }
}


//===========================
//	INIT WATCHDOG TIMER
//===========================
void InitWdt(void)
{
  Wdt.Enable();
}


//===========================
//	INIT ADC
//===========================
void InitAdc(void)
{
  INT8 err;
  // Mode of operation.
  //================================================
  UINT32 samplingClk = ADC_CLK_TMR;     // Timer3 used for sampling
//  UINT32 samplingClk = ADC_CLK_MANUAL;     // Timer3 used for sampling
  //================================================

  // Hardware config.
  //================================================
  UINT32 configHardware = ADC_VREF_EXT_AVSS         // Vref+ external and Vref- is AVss
                        | ADC_SAMPLES_PER_INT_8;    // 1 sample/interrupt
  //================================================

  // Port config.
  //================================================
  UINT32 configPort =   //ENABLE_ALL_ANA
                        ENABLE_AN8_ANA
                      | ENABLE_AN9_ANA
                      | ENABLE_AN10_ANA
                      | ENABLE_AN11_ANA
                      | ENABLE_AN12_ANA
                      | ENABLE_AN13_ANA
                      | ENABLE_AN14_ANA
                      | ENABLE_AN15_ANA
                      ; // Enable AN1-AN15 in analog mode
  
  UINT32 configScan =   SKIP_SCAN_AN0  // Skip AN0 as it's vref+
                      | SKIP_SCAN_AN1
                      | SKIP_SCAN_AN2
                      | SKIP_SCAN_AN3
                      | SKIP_SCAN_AN4
                      | SKIP_SCAN_AN5
                      | SKIP_SCAN_AN6
                      | SKIP_SCAN_AN7
//                      | SKIP_SCAN_AN8
//                      | SKIP_SCAN_AN9
//                      | SKIP_SCAN_AN10
//                      | SKIP_SCAN_AN11
//                      | SKIP_SCAN_AN12
//                      | SKIP_SCAN_AN13
//                      | SKIP_SCAN_AN14
//                      | SKIP_SCAN_AN15
                      ;  
//  UINT32 configScan = 0;
//  UINT32 configScan = SKIP_SCAN_ALL;
  //================================================

  // Open ADC with parameters above
  err = Adc.Open(samplingClk, configHardware, configPort, configScan);
  if (err < 0)
  {
    LED1_ON;
  }

  err = Adc.ConfigInterrupt(ADC_INTERRUPT_PRIORITY, ADC_INTERRUPT_SUBPRIORITY);
  if (err < 0)
  {
    LED1_ON;
  }
}

//===========================
//	START INTERRUPTS
//===========================
void StartInterrupts(void)
{
  INT8 err;
  
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable timer interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//  Timer.EnableInterrupt(TIMER_1);
//  Timer.EnableInterrupt(TIMER_2);
  err = Timer.EnableInterrupt(TIMER_3);
  if (err < 0)
  {
    LED1_ON;
  }
//  Timer.EnableInterrupt(TIMER_4);
//  Timer.EnableInterrupt(TIMER_5);


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable RX UART interrupts and disable TX interrupts. 
// TX interrupts are disabled at init and only
// enabled when writing to the user's TX FIFO buffer
// with Uart.PutTxFifoBuffer(...)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  Uart.EnableRxInterrupts (UART3);  // Enable RX Interrupts for UART3
  Uart.DisableTxInterrupts(UART3);  // Disable TX Interrupts for UART3

  Uart.EnableRxInterrupts (UART6);  // Enable RX Interrupts for UART6
  Uart.DisableTxInterrupts(UART6);  // Disable TX Interrupts for UART6


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable SPI interrupts             // Not functionnal yet
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//  Spi.EnableRxInterrupts (SPI3);   // Enable RX Interrupts for SPI3
//  Spi.DisableTxInterrupts(SPI3);   // Enable TX Interrupts for SPI3


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable ADC interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  err = Adc.EnableInterrupts();   // Works only when not in manual mode
  if (err < 0)
  {
    LED1_ON;
  }


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable I2C interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  
  err = I2c.EnableInterrupt (I2C5, I2C_MASTER_INTERRUPT);
  if (err < 0)
  {
    LED1_ON;
  }
  err = I2c.DisableInterrupt(I2C5, I2C_SLAVE_INTERRUPT);
  if (err < 0)
  {
    LED1_ON;
  }
  err = I2c.DisableInterrupt(I2C5, I2C_BUS_COLLISION_INTERRUPT);
  if (err < 0)
  {
    LED1_ON;
  }


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable multi-vector interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
  INTEnableInterrupts();

}
