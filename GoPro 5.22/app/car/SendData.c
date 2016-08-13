#include "Include_Def.h"

void SendData1(void)
{
  uint8 iii;
  DataScope_Get_Channel_Data(  MMA8451_Value, 1);
  DataScope_Get_Channel_Data(  Angle_dot, 2);
  DataScope_Get_Channel_Data(  Angle, 3);
  for( iii = 0 ; iii < DataScope_Data_Generate(3); iii++ )
    LPLD_UART_PutChar( UART4 , DataScope_OutPut_Buffer[iii] );
  
  LED_P6x8Str(0,7,"1MMA 2ENC 3Angle");
}

void SendData2(void)
{ 
  uint8 iii;
  DataScope_Get_Channel_Data(  g_fAngleControlOut, 1);
  DataScope_Get_Channel_Data(  g_fSpeedControlOut, 2);
  DataScope_Get_Channel_Data(  g_fDirectionControlOut, 3);
  for( iii = 0 ; iii < DataScope_Data_Generate(3); iii++ )
    LPLD_UART_PutChar( UART4 , DataScope_OutPut_Buffer[iii] );
}
void SendData3(void)
 {
    uint8 iii;
    DataScope_Get_Channel_Data(  g_nLeftMotorPulse, 1);
    DataScope_Get_Channel_Data(  g_nRightMotorPulse, 2);
    DataScope_Get_Channel_Data(  g_fCarSpeed, 3);
    for( iii = 0 ; iii < DataScope_Data_Generate(3); iii++ )
       LPLD_UART_PutChar( UART4 , DataScope_OutPut_Buffer[iii] );
 }
void SendData4(void)
 {
    uint8 iii;
    DataScope_Get_Channel_Data(  fAngleAndSpeed, 1);
    DataScope_Get_Channel_Data(  g_fCarSpeed, 2);
    for( iii = 0 ; iii < DataScope_Data_Generate(2); iii++ )
       LPLD_UART_PutChar( UART4 , DataScope_OutPut_Buffer[iii] );
    
 }
void SendData5(void)
 {
    uint8 iii;
    DataScope_Get_Channel_Data(  VOLTAGE_LEFT, 1);
    DataScope_Get_Channel_Data(  VOLTAGE_RIGHT, 2);
    
    for( iii = 0 ; iii < DataScope_Data_Generate(2); iii++ )
       LPLD_UART_PutChar( UART4 , DataScope_OutPut_Buffer[iii] );
 }
void SendData6(void)
{}