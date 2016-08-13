#include "Include_Def.h"

//卡尔曼滤波
//dt=累加周期/1s
float Angle = 0, Angle_dot = 0;
float P[2][2] ={{ 0.004, 0 },{ 0, 0.004 }};	
float E = 0;  
float q_bias = 0;
float Angle_err = 0;
float PCt_0 = 0, PCt_1 = 0;
float K_0 = 0, K_1 = 0;
float t_0 = 0, t_1 = 0;
float Pdot[4] ={0.007,-0.004,-0.004,0.003};
char  C_0 = 1;



void Kalman_Filter(float angle_m,float gyro_m)
{
	Angle += (gyro_m-q_bias) * dt; 
	
	Pdot[0]=Q_angle - P[0][1] - P[1][0];
	Pdot[1]=- P[1][1];
	Pdot[2]=- P[1][1];
	Pdot[3]=Q_gyro;
	
	P[0][0] += Pdot[0] * dt;
	P[0][1] += Pdot[1] * dt;
	P[1][0] += Pdot[2] * dt;
	P[1][1] += Pdot[3] * dt;
		
	Angle_err = angle_m - Angle;   
		
	PCt_0 = C_0 * P[0][0];
	PCt_1 = C_0 * P[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;      
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * P[0][1];

	P[0][0] -= K_0 * t_0;   
	P[0][1] -= K_0 * t_1;
	P[1][0] -= K_1 * t_0;
	P[1][1] -= K_1 * t_1;
		
	Angle += K_0 * Angle_err;  	
	q_bias+= K_1 * Angle_err;  
	Angle_dot = gyro_m-q_bias;  //角度（ angle）、角速度（ angle_dot ）
     
}
