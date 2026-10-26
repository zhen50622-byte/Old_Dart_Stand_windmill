//
// Created by 28416 on 2026/9/7.
//
#include "FreeRTOS.h"
#include "task.h"
#include "BJ_Motor_task.h"
void Start_BJ_Task(void *argument)
{
    for (;;)
    {
        // if (RC_CtrlData.rc.s1 == 3 || RC_CtrlData.rc.s1 == 2)//手动挡
        // {
        //     if (RC_CtrlData.rc.ch1>1034.0f)
        //     {
        //         X_V2_Vel_Control(8, 0, 1000,((RC_CtrlData.rc.ch1-1024.0f)/660.0f)*90.0f, 0);//测试代码,正转
        //     }
        //     else if (RC_CtrlData.rc.ch1<1014.0f)
        //     {
        //         X_V2_Vel_Control(8, 1, 1000,((1024.0f-RC_CtrlData.rc.ch1)/660.0f)*90.0f, 0);//测试代码,反转
        //     }
        //     else
        //     {
        //         //do nothing
        //         X_V2_Stop_Now(8, 0);
        //
        //     }
        // }
        // else if (RC_CtrlData.rc.s1 == 1 &&  RC_CtrlData.rc.s2 != 1)//调参版模式调参
        // {
        //
        //         //taskENTER_CRITICAL();//屏蔽中断 防止数据没读取完 又写入新数据 (虽然我一次只发一次 不太可能出现这种情况)
        //         switch (Dart_ID_state)
        //         {
        //         case 0x11:
        //             if (D1_Data_Yaw.f > 0.0f)
        //             {
        //                 X_V2_Traj_Pos_Control(8,0,1000,1000,70.0f,D1_Data_Yaw.f*10.0f,1,0);//第三个和1第四个参数分别为加/减速度
        //             }//这*10是因为 实测 我发360 电机只转36 所以乘以10
        //             else
        //             {
        //                 X_V2_Traj_Pos_Control(8,1,1000,1000,70.0f,D1_Data_Yaw.f*10.0f,1,0);
        //             }
        //             break;
        //         case 0x12:
        //             if (D2_Data_Yaw.f > 0.0f)
        //             {
        //                 X_V2_Traj_Pos_Control(8,0,1000,1000,70.0f,D2_Data_Yaw.f*10.0f,1,0);
        //             }
        //             else
        //             {
        //                 X_V2_Traj_Pos_Control(8,1,1000,1000,70.0f,D2_Data_Yaw.f*10.0f,1,0);
        //             }
        //             break;
        //         case 0x13:
        //             if (D3_Data_Yaw.f > 0.0f)
        //             {
        //                 X_V2_Traj_Pos_Control(8,0,1000,1000,70.0f,D3_Data_Yaw.f*10.0f,1,0);
        //             }
        //             else
        //             {
        //                 X_V2_Traj_Pos_Control(8,1,1000,1000,70.0f,D3_Data_Yaw.f*10.0f,1,0);
        //             }
        //             break;
        //         case 0x14:
        //             if (D4_Data_Yaw.f > 0.0f)
        //             {
        //                 X_V2_Traj_Pos_Control(8,0,1000,1000,70.0f,D4_Data_Yaw.f*10.0f,1,0);
        //             }
        //             else
        //             {
        //                 X_V2_Traj_Pos_Control(8,1,1000,1000,70.0f,D4_Data_Yaw.f*10.0f,1,0);
        //             }
        //             break;
        //         default:
        //             break;
        //         }
        //     //taskEXIT_CRITICAL();
        // }
        osDelay(1);
    }
}