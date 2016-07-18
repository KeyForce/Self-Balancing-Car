 /*
**==============================================================================
** F8013.H:             -- by Dr. ZhuoQing, 2011-11-12
**
**  Description:
**
**==============================================================================
*/
#ifndef __F8013__
#define __F8013__
//------------------------------------------------------------------------------
#ifdef F8013_GLOBALS
    #define F8013_EXT
#else
    #define F8013_EXT extern
#endif // F8013_GLOBALS
//------------------------------------------------------------------------------
#define WAITTIME_EN					1
#define SERIAL_EN					1
#define INLINE_FUNC_EN				1
#define LED_EN						1
#define MISC_EN						1
#define JUMPAPP_EN					1

//==============================================================================
void F8013Init(void);
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//		MISCELLANEOUS FUNCTION
//------------------------------------------------------------------------------
#if MISC_EN
#define DELAY_1MS_CONSTANT				1392
void Delay1MS(unsigned int n1MS);
#define DELAY_100US_CONSTANT			DELAY_1MS_CONSTANT / 10
void Delay100US(unsigned int n100US);
#define DELAY_10US_CONSTANT				DELAY_100US_CONSTANT / 10
void Delay10US(unsigned int n10US);
#endif // MISC_EN

//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
#if WAITTIME_EN
F8013_EXT	unsigned int g_nWaitTimeCount;
void WaitTime(unsigned int nWaitTime);
#define WAITTIME_INC  (g_nWaitTimeCount ++)
#endif//WAITTIME_EN

//------------------------------------------------------------------------------
#if SERIAL_EN
#define CAN_RECE 			((AS1_GetCharsInRxBuf() == 0) ? 0 : 1)
#define RECE_LOOP			0xfff0
unsigned char ReceChar(unsigned char * pucChar);
void SendChar(unsigned char ucChar);
void SendWord(unsigned int nWord);
void SendStr(char * pszString);

void SendHEX8(unsigned char ucChar);
void SendHEX16(unsigned int nValue);
unsigned char HEX2CHAR(unsigned char c);

#endif //SERIAL_EN



//==============================================================================
//  		MEMORY ACCESS FUNCTIONS
//------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// I N L I N E D   F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////
#if INLINE_FUNC_EN
unsigned int pmem_read(register unsigned int *addr);
void pmem_write(register unsigned int * addr, register unsigned int data);
void pmemReadDim(unsigned int nStartLength, unsigned int nLength, unsigned int * pWord);
#endif // INLINE_FUNC_EN


//==============================================================================
//				LED FUNCTION
//------------------------------------------------------------------------------
#if LED_EN
#define LED_ON		LED_SetVal()
#define LED_OFF		LED_ClrVal()
#endif // LED_EN

//------------------------------------------------------------------------------
//  JUMPAPP
//------------------------------------------------------------------------------
#if JUMPAPP_EN
void Jump2App(unsigned long lnAddress);
#define BOOTLOADER_ADD			0x0
#define JUMP_BOOTLOADER			Jump2App(BOOTLOADER_ADD)
#endif // JUMPAPP_EN

//==============================================================================
//             END OF THE FILE : F8013.H
//------------------------------------------------------------------------------
#endif // __F8013__
