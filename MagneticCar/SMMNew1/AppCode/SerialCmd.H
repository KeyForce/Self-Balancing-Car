/*
**==============================================================================
** SerialCmd.H:             -- by Dr. ZhuoQing, 2011-11-13
**
**  Description:
**
**==============================================================================
*/
#ifndef __SerialCmd__
#define __SerialCmd__
//------------------------------------------------------------------------------
#ifdef SerialCmd_GLOBALS
    #define SerialCmd_EXT
#else
    #define SerialCmd_EXT extern
#endif // SerialCmd_GLOBALS
//------------------------------------------------------------------------------
//==============================================================================
//   SERIAL COMMAND PROCESS SUBROUTINE
//------------------------------------------------------------------------------
void SerialCmdInit(void);                       // Initialize all the serial command buffer

#define SERIAL_COMMAND			256
SerialCmd_EXT char g_ucSerialCommandBuffer[SERIAL_COMMAND];

#define SERIAL_ARG_NUMBER		8
#define SERIAL_ARG_LENGTH		16
SerialCmd_EXT char g_ucSerialCmd[SERIAL_ARG_LENGTH];
SerialCmd_EXT char g_ucSerialCmdArg[SERIAL_ARG_NUMBER][SERIAL_ARG_LENGTH];
SerialCmd_EXT char g_nSerialCmdArg;

void SerialCmd2Arg(void);
#define SERIAL_CMD		g_ucSerialCmd
#define SERIAL_ARG		g_ucSerialCmdArg
#define SERIAL_ARG_NUM  g_nSerialCmdArg
#define SERIAL_LONGARG	g_ucSerialCommandBuffer

//------------------------------------------------------------------------------
#define SERIAL_CMD_ECHO				1

unsigned char SerialCmdProcChar(unsigned char ucChar); //return : 1: Has the command, 0 : No command
void SerialCmdProcCmd(void);


//------------------------------------------------------------------------------
void SendHEX8(unsigned char ucChar);
void SendHEX16(unsigned short nData);
void SendHEX32(unsigned long lnData);

//------------------------------------------------------------------------------
unsigned int Str2HEX16(char * pszString);
char HEX2CHAR(char c);
unsigned char CHAR2HEX(char c);

//==============================================================================
//			DEBUG FUNCTIONS DEFINITIONS
//------------------------------------------------------------------------------
void DRAMFunc(void);
void PRAMFunc(void);
void SizeOfFunc(void);



//==============================================================================
//             END OF THE FILE : SerialCmd.H
//------------------------------------------------------------------------------
#endif // __SerialCmd__
