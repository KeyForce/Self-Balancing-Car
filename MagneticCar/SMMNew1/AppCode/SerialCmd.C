/*
**==============================================================================
** SerialCmd.C :                               -- by Dr. ZhuoQing, 2011-11-13
**
** Description:
**     
**==============================================================================
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define SerialCmd_GLOBALS		1
#include "serialCmd.h"

#include "F8013.h"
#include "term1.h"

//==============================================================================
//   SERIAL COMMAND PROCESS SUBROUTINE
//------------------------------------------------------------------------------
void SerialCmdInit(void) {
	int i;
	strcpy((char *)g_ucSerialCommandBuffer, "");
	
	SERIAL_ARG_NUM = 0;
	strcpy((char *)SERIAL_CMD, "");
	for(i = 0; i < SERIAL_ARG_NUM; i ++) {
		strcpy((char *)SERIAL_ARG[i], "");
	}
}

//------------------------------------------------------------------------------
void SerialCmd2Arg(void) {
	char c, ucCmdFlag, ucLongArgFlag;
	unsigned short nPoint, nArgPoint, nArgNumber;
	
	SERIAL_ARG_NUM = 0;
	strcpy((char *)SERIAL_CMD, "");
	
	nArgPoint = 0;
	ucCmdFlag = 1;
	nArgNumber = 0;
	ucLongArgFlag = 0;                          // Mark the long argument enclosed by '\'' exists
	
	for(nPoint = 0; c != 0; nPoint ++) {
		c = g_ucSerialCommandBuffer[nPoint];
		
		if(c == '\'') {
			nPoint ++;
			ucLongArgFlag = 1;
			for(; c != 0; nPoint ++) {
				c = g_ucSerialCommandBuffer[nPoint];
				if(c == '\'') break;
			}
			if(c == 0) break;
			continue;
		}
		
		if(c == 0 || isspace(c) || nArgPoint >= SERIAL_ARG_LENGTH - 1) {
			if(ucCmdFlag) {
				SERIAL_CMD[nArgPoint] = 0;
				ucCmdFlag = 0;
				nArgPoint = 0;
			} else {
				SERIAL_ARG[nArgNumber][nArgPoint] = 0;
				nArgPoint = 0;
				nArgNumber ++;
				if(nArgNumber >= SERIAL_ARG_NUMBER) break;
			}
			if(isspace(c)) continue;
		}
		
		if(ucCmdFlag) {
			SERIAL_CMD[nArgPoint ++] = c;
		} else {
			SERIAL_ARG[nArgNumber][nArgPoint ++] = c;
		}
	}
	//--------------------------------------------------------------------------
	SERIAL_ARG_NUM = (char)nArgNumber;
	
	//--------------------------------------------------------------------------
	if(ucLongArgFlag) {
		for(nPoint = 0; ; nPoint ++) {
			c = g_ucSerialCommandBuffer[nPoint];
			if(c == '\'') break;
			else if(c == 0x0) break;
		}
		
		if(c == 0) strcpy(g_ucSerialCommandBuffer, "");
		else {
			nArgPoint = 0;
			for(nPoint ++; c != 0; nPoint ++) {
				c = g_ucSerialCommandBuffer[nPoint];
				if(c == 0) break;
				if(c == '\'') break;
				g_ucSerialCommandBuffer[nArgPoint ++] = c;
			}
			g_ucSerialCommandBuffer[nArgPoint] = 0;
		}
	} else g_ucSerialCommandBuffer[0] = 0;

}

//------------------------------------------------------------------------------
unsigned char SerialCmdProcChar(unsigned char ucChar) { //return : 1: Has the command, 0 : No command
	char szStr[2];
	szStr[0 ] = (char)ucChar;
	szStr[1] = 0;

	//--------------------------------------------------------------------------
	if(ucChar == '\b') {
		unsigned int i;
		for(i = 0; i < SERIAL_COMMAND; i ++)
			if(g_ucSerialCommandBuffer[i] == 0) break;
		if(i > 0) i --;
		g_ucSerialCommandBuffer[i] = 0;
		return 0;
	}
		
	//--------------------------------------------------------------------------
	if(ucChar == '\r') {
#if SERIAL_CMD_ECHO
		SendChar('\r');
		SendChar('\n');
#endif // SERIAL_CMD_ECHO

		SerialCmd2Arg();
		SerialCmdProcCmd();
		return 1;
	}
	
	//--------------------------------------------------------------------------
	strcat(g_ucSerialCommandBuffer, szStr);
#if SERIAL_CMD_ECHO
	SendChar(ucChar);
#endif //SERIAL_CMD_ECHO
	return 0;
}

void SerialCmdProcCmd(void) {
	int i;

	if(strlen(SERIAL_CMD) == 0) {
		SendStr("No Serial Command !\r\n");
		return;
	}
	
	//--------------------------------------------------------------------------
	SendStr(SERIAL_CMD);
	SendStr(" : ");
	for(i = 0; i < SERIAL_ARG_NUM; i ++) {
		SendStr(SERIAL_ARG[i]);
		SendStr(" ");
	}
	if(strlen(g_ucSerialCommandBuffer) > 0) {
		SendStr(" - ");
		SendStr(g_ucSerialCommandBuffer);
	}
	SendStr("\r\n");
	
	//--------------------------------------------------------------------------
	if(strcmp("hello",				SERIAL_CMD) == 0) {
		SendStr("F8013 Debug Hello.\r\n");
	} else if(strcmp("dram", 		SERIAL_CMD) == 0) {
		DRAMFunc();
	} else if(strcmp("pram",		SERIAL_CMD) == 0) {
		PRAMFunc();
	} else if(strcmp("sizeof",		SERIAL_CMD) == 0) {
		SizeOfFunc();
	} else {
		SendStr("Error Cmd:");
		SendStr(SERIAL_CMD);
		SendStr("\r\n");
	}
	
	SerialCmdInit();
}
//==============================================================================
//				SEND SUBROUTINE FOR DEBUG
//------------------------------------------------------------------------------
char HEX2CHAR(char c) {
	return (char)((c < 10) ? (c + '0') : (c - 10 + 'A'));
}
unsigned char CHAR2HEX(char c) {
	if(c >= '0' && c <= '9') return (unsigned char)(c - '0');
	else if(c >= 'A' && c <= 'F') return (unsigned char)(c - 'A' + 10);
	else if(c >= 'a' && c <= 'f') return (unsigned char)(c - 'a' + 10);
	else return (unsigned char)c;
}
unsigned int Str2HEX16(char * pszString) {
	char c;
	unsigned char ucCount;
	unsigned int nValue;
	
	nValue = 0;
	for(ucCount = 0; ucCount < 4; ucCount ++) {
		c = *(pszString ++);
		if(c == 0) break;
		nValue = (nValue << 4) + CHAR2HEX(c);
	}
	return nValue;
}
		
//------------------------------------------------------------------------------
void SendHEX8(unsigned char ucChar) {
	char szString[3];
	szString[0] = HEX2CHAR((char)((ucChar >> 8) & 0xf));
	szString[1] = HEX2CHAR((char)(ucChar & 0xf));
	szString[2] = 0;
	SendStr(szString);
}
void SendHEX16(unsigned short nData) {
	SendHEX8((unsigned char)(nData >> 8));
	SendHEX8((unsigned char)(nData & 0xf));
}
void SendHEX32(unsigned long lnData) {
	SendHEX16((unsigned int)(lnData >> 16));
	SendHEX16((unsigned int)(lnData & 0xffff));
}
	


//==============================================================================
// 			DEBUG FUNCTIONS
//------------------------------------------------------------------------------
void SizeOfFunc(void) {
	SendStr("Size of unsigned char:");
	SendHEX16(sizeof(unsigned char));
	SendStr("\r\n");
	SendStr("Size of unsigned int:");
	SendHEX16(sizeof(unsigned int));
	SendStr("\r\n");
	SendStr("Size of unsigned long:");
	SendHEX16(sizeof(unsigned long));
	SendStr("\r\n");
	SendStr("Size of unsigned float:");
	SendHEX16(sizeof(float));
	SendStr("\r\n");
	SendStr("Size of unsigned double:");
	SendHEX16(sizeof(double));
	SendStr("\r\n");
}
	
void DRAMFunc(void) {
	int nStartAddress, nLength;
	int nCount;
	unsigned int * pPoint;
	unsigned int i;
	
	nStartAddress = 0;
	nLength = 0x100;
	
	if(SERIAL_ARG_NUM >= 2)	nStartAddress = (int)Str2HEX16(SERIAL_ARG[1]);
	if(SERIAL_ARG_NUM >= 3) nLength = (int)Str2HEX16(SERIAL_ARG[2]);
		
	pPoint = (unsigned int *)nStartAddress;
	nCount = 0;
	if(strcmp("r",		SERIAL_ARG[0]) == 0) {
		for(i = 0; i < nLength; i ++) {
			SendHEX16(*(pPoint ++));
			SendStr(" ");
			nCount ++;
			if(nCount >= 16) {
				nCount = 0;
				SendStr("\n");
			}
		}
		if(nCount > 0) SendStr("\r\n");
	} else {
		SendStr("Error DRAM Command:");
		SendStr(SERIAL_ARG[0]);
		SendStr("\r\n");
	}
}

void PRAMFunc(void) {
	unsigned int nStartAddress, nLength;
	int nCount;
	unsigned int i;
	unsigned int nData;
//	void * pProgram(void);
	
	nStartAddress = 0;
	nLength = 0x100;
	
	if(SERIAL_ARG_NUM >= 2)	nStartAddress = Str2HEX16(SERIAL_ARG[1]);
	if(SERIAL_ARG_NUM >= 3) nLength = Str2HEX16(SERIAL_ARG[2]);
		
	nCount = 0;
	if(strcmp("r",		SERIAL_ARG[0]) == 0) {
		for(i = 0; i < nLength; i ++) {
			nData = pmem_read((unsigned int *)nStartAddress);
//			pProgram = (void *(void))nStartAddress;
			nStartAddress ++;
			SendHEX16(nData);
			SendStr(" ");
			nCount ++;
			if(nCount >= 16) {
				nCount = 0;
				SendStr("\r\n");
			}
		}
		if(nCount > 0) SendStr("\r\n");
	} else {
		SendStr("Error PRAM Command:");
		SendStr(SERIAL_ARG[0]);
		SendStr("\n");
	}
}
	
	


//==============================================================================
//             END OF THE FILE : SerialCmd.C
//------------------------------------------------------------------------------

