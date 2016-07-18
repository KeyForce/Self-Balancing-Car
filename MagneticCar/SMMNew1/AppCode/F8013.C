/*
**==============================================================================
** F8013.C :                               -- by Dr. ZhuoQing, 2011-11-12
**
** Description:
**     
**==============================================================================
*/

#define F8013_GLOBALS			1
#include "F8013.h"
#if SERIAL_EN
#include "AS1.h"
#endif // SERIAL_EN

//------------------------------------------------------------------------------
void F8013Init(void) {
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//		MISCELLANEOUS FUNCTION
//------------------------------------------------------------------------------
#if MISC_EN
void Delay1MS(unsigned int n1MS) {
	unsigned int j, k;
	unsigned long i;
	
	k = 0;
	for(j = 0; j < n1MS; j ++) {
		for(i = 0; i < DELAY_1MS_CONSTANT; i ++) k ++;
	}
}
void Delay100US(unsigned int n100US) {
	unsigned int j, k;
	unsigned long i;
	
	k = 0;
	for(j = 0; j < n100US; j ++) {
		for(i = 0; i < DELAY_100US_CONSTANT; i ++) k ++;
	}
}
void Delay10US(unsigned int n10US) {
	unsigned int j, k;
	unsigned long i;
	
	k = 0;
	for(j = 0; j < n10US; j ++) {
		for(i = 0; i < DELAY_10US_CONSTANT; i ++) k ++;
	}
}

#endif // MISC_EN

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//			WAIT TIME MODULE
#if WAITTIME_EN		
void WaitTime(unsigned int nWaitTime) {
	unsigned int nEndTime = g_nWaitTimeCount + nWaitTime;
	for(;;) {
		if(g_nWaitTimeCount == nEndTime) break;
	}
}

#endif // WAITTIME_EN

//------------------------------------------------------------------------------
#if SERIAL_EN
unsigned char ReceChar(unsigned char * pucChar) {
	unsigned int i;
	for(i = 0;i < RECE_LOOP; i ++) {
		if(CAN_RECE) break;
	}
	if(i >= RECE_LOOP) return 1;
	
	AS1_RecvChar(pucChar);
	return 0;
}

void SendChar(unsigned char ucChar) {
	unsigned int i;
	for(i = 0; i < RECE_LOOP; i ++) {
		if(AS1_SendChar(ucChar) == ERR_OK) return;
	}
}
void SendWord(unsigned int nWord) {
	SendChar((unsigned char)(nWord >> 8));
	SendChar((unsigned char)nWord);
}		
void SendStr(char * pszString) {
	while(*(pszString)) {
//		AS1_SendChar((unsigned char)*(pszString));
		SendChar((unsigned char )*(pszString));
		pszString ++;
	}		
}
void SendHEX8(unsigned char ucChar) {
	unsigned char ucC;
	
	ucC = HEX2CHAR((unsigned char)((ucChar >> 4) & 0xf));
	SendChar(ucC);
	ucC = HEX2CHAR((unsigned char)(ucChar & 0xf));
	SendChar(ucC);
}
void SendHEX16(unsigned int nValue) {
	SendHEX8((unsigned char)(nValue >> 8));
	SendHEX8((unsigned char)(nValue & 0xff));
}

unsigned char HEX2CHAR(unsigned char x) {
	return (unsigned char)(x >= 10 ? 'A' - 10 + x : '0' + x);
}

#endif //SERIAL_EN

//==============================================================================
//  		MEMORY ACCESS FUNCTIONS
//------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
// I N L I N E D   F U N C T I O N S
/////////////////////////////////////////////////////////////////////////////
#if INLINE_FUNC_EN
unsigned int pmem_read(register unsigned int *addr)
{
   register unsigned int data;

   asm(move.w  p:(addr)+,data);
   return data;
}

void pmem_write(register unsigned int * addr, register unsigned int data)
{
   asm(move.w  data,p:(addr)+n);
}

void pmemReadDim(unsigned int nStartLength, unsigned int nLength, unsigned int * pWord) {
	unsigned int i, nCount;
	
	nCount = 0;
	for(i = nStartLength; i < nStartLength + nLength; i ++) 
		*(pWord + (nCount ++)) = pmem_read((unsigned int *)i);
		
}

#endif // INLINE_FUNC_EN

//------------------------------------------------------------------------------
//  JUMPAPP
//------------------------------------------------------------------------------
#if JUMPAPP_EN
void Jump2App(unsigned long lnAddress) {
//	INTC_VBA = ((0x4000) >> 7);
    asm(move.l lnAddress, N);
    asm(jmp (N));
}
#endif // JUMPAPP_EN


//==============================================================================
//             END OF THE FILE : F8013.C
//------------------------------------------------------------------------------

