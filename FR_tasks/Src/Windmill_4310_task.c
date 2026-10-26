//
// Created by 28416 on 2026/10/19.
//
#include "Windmill_4310_task.h"
uint8_t flag_3519_back = 0;
uint8_t flag_servo = 0;
uint8_t flag_4310_stop = 0;//4310 if 判断停下标志位
uint8_t Windmill_Target_stop = 0;
uint8_t Windmill_flag = 0;//大风车的状态机变量
uint8_t flag_2006_test = 0;
float Windmill_Target = -(PI/3.0f);//4310 位置目标的变量 首次目标为-60°
volatile float DM_4310_position = 0.0f;//大风车达妙电机的位置
uint32_t t0_4310 = 0;//计时的变量
uint32_t t_3519 = 0;//此为等待3519的时间
uint8_t flag_wait_3519 = 0;
void Windmill_4310_Start(void *argument)
{
    /* USER CODE BEGIN Windmill_4310_Start */
    /* Infinite loop */
    for(;;)
    {
        //DM_Motor_Setposition(&hcan1, 0x51, POSITION, (PI/3.0f), 10.0f);   // 一直保持 60°
        //  switch (Windmill_flag)
        // {
        // case  1:
        //      if (Windmill_Target_stop == 0)
        //      {
        //          Windmill_Target_stop = 1;//第一个if只执行一次 就不会累加目标值
        //          Windmill_Target = Windmill_Target + (PI/3.0f);
        //          DM_Motor_Setposition(&hcan1,0x51,POSITION,Windmill_Target,7.0f);//右,正方向/大风车的方向到时候具体看
        //          target_position_2006 = -3600.0f;
        //          flag_2006_test = 1;//此标志位没用了 可以考虑删除
        //      }
        //
        //     //if (fabsf(DM_4310_position - Windmill_Target) < 0.05f && flag_4310_stop == 0)//当误差小于0.05时 判定为真 该判断可以不用以快速放置飞镖
        //     //{
        //         //当达妙电机转120°时 2006开始下降到目标位置
        //
        //         if (fabsf(actuall_position_2006 - target_position_2006) < 0.5f)
        //         {
        //            if (flag_2006_test == 1)
        //           {
        //          //HAL_Delay(500);
        //          flag_servo = 1;
        //          flag_4310_stop = 1;//这样第二个if只会执行一次
        //          flag_2006_test = 0;
        //                flag_wait_3519 = 1;
        //                t_3519 = osKernelGetTickCount();
        //          t0_4310 = osKernelGetTickCount();//获取当前时间
        //
        //          //}//当误差小于0.05时 2006到达了指定位置 可以放飞镖
        //          //舵机代码+暂时延时处理
        //           }
        //         }
        //      if (osKernelGetTickCount()-t_3519>=1700u && flag_wait_3519 == 1)// 经过3.0s  确保放镖 且滑块到达指定放镖位置 舵机恢复初始状态
        //      {
        //          flag_wait_3519 = 0;//确保只进一次
        //          //__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1,200.0f);//放飞镖
        //      }
        //     if (osKernelGetTickCount()-t0_4310>=3500u && flag_servo == 1)// 经过3.5s  确保放镖 且滑块到达指定放镖位置 舵机恢复初始状态
        //     {
        //         flag_servo = 0;//确保该if只进一次
        //
        //         //3s 过后 2006可以回到初始位置
        //         Windmill_Target =Windmill_Target + (PI/3.0f);
        //         DM_Motor_Setposition(&hcan1,0x51,POSITION,Windmill_Target,7.0f);//回到不干涉飞镖的位置
        //         target_position_2006 = 0.0f;
        //         __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1,320.0f);//爪子恢复初始状态
        //         flag_3519_back = 1;//3519后退标志位 当有飞镖时往后退到板机后面的位置
        //         // DM_Motor_Setposition(&hcan2,0x00,POSITION,(80.0f),18.0f);//右,正方向 680
        //         // DM_Motor_Setposition(&hcan2,0x01,POSITION,-(80.0f),18.0f);//左，正方向 电机继续后退
        //
        //
        //         //t0 = osKernelGetTickCount();//获取当前时间
        //
        //         if (fabsf(actuall_position_2006 - target_position_2006) < 0.5f)//2006回到原点之后 所做的操作
        //         {
        //
        //             //Windmill_Target = Windmill_Target + (2*PI)/3.0f;//因为达妙电机带多圈处理 所以位置目标值在每次的基础上加三分之一圈 放在这儿 防止一直进if判断一直加角度
        //             //Windmill_flag = 1;
        //             //do nothing
        //         }//当误差小于0.05时 即2006回到原点时 DM电机标志位更新 转到240°
        //
        //     }
        //     break;
        // case  2:
        //      if (Windmill_Target_stop == 1)
        //      {
        //          Windmill_Target_stop = 2;
        //          flag_2006_test = 2;
        //          Windmill_Target = Windmill_Target + (PI/3.0f);
        //          DM_Motor_Setposition(&hcan1,0x51,POSITION,Windmill_Target,7.0f);//右,正方向/大风车的方向到时候具体看
        //          target_position_2006 = -3600.0f;
        //      }
        //     //舵机代码+暂时延时处理
        //     //if (fabsf(DM_4310_position - Windmill_Target) < 0.05f && flag_4310_stop == 1)//当误差小于0.05时 判定为真
        //     //{
        //         //当达妙电机转240°时 2006开始下降到目标位置
        //
        //         if (fabsf(actuall_position_2006 - target_position_2006) < 0.5f)
        //         {
        //             if (flag_2006_test == 2)
        //           {
        //          //HAL_Delay(500);
        //          flag_servo = 2;
        //          flag_4310_stop = 2;
        //          flag_2006_test = 0;
        //                 flag_wait_3519 = 2;
        //                 t_3519 = osKernelGetTickCount();
        //          t0_4310 = osKernelGetTickCount();//获取当前时间
        //          //}//当误差小于0.05时 2006到达了指定位置 可以放飞镖
        //          //舵机代码+暂时延时处理
        //           }
        //         }
        //
        //      if (osKernelGetTickCount()-t_3519>=1700u && flag_wait_3519 == 2)// 经过3.5s  确保放镖 且滑块到达指定放镖位置 舵机恢复初始状态
        //      {
        //          flag_wait_3519 = 0;//确保只进一次
        //          //__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2,750.0f);//放飞镖
        //      }
        //     if (osKernelGetTickCount()-t0_4310>=3500u && flag_servo ==2)//当时间为1秒时或太短时 电机运行时间恰为1s就会把__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2,680.0f);覆盖掉
        //     {
        //         flag_servo = 0;//确保该if只进一次
        //
        //
        //         //3s 过后 2006可以回到初始位置
        //         Windmill_Target =Windmill_Target + (PI/3.0f);
        //         DM_Motor_Setposition(&hcan1,0x51,POSITION,Windmill_Target,7.0f);//回到不干涉飞镖的位置
        //         target_position_2006 = 0.0f;
        //         __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2,600.0f);//爪子恢复初始状态
        //         flag_3519_back = 1;
        //         // DM_Motor_Setposition(&hcan2,0x00,POSITION,(80.0f),18.0f);//右,正方向 680
        //         // DM_Motor_Setposition(&hcan2,0x01,POSITION,-(80.0f),18.0f);//左，正方向 电机继续后退
        //
        //
        //
        //         if (fabsf(actuall_position_2006 - target_position_2006) < 0.5f)
        //         {
        //
        //             // Windmill_Target = Windmill_Target + (2*PI)/3.0f;
        //             // Windmill_flag = 2;
        //             //do nothing
        //         }//当误差小于0.05时 即2006回到原点时 DM电机标志位更新 转到360°
        //     }
        //     break;
        // case  3:
        //      if (Windmill_Target_stop == 2)
        //      {
        //          Windmill_Target_stop = 0;
        //          flag_2006_test = 3;
        //          Windmill_Target = Windmill_Target + (PI/3.0f);
        //          DM_Motor_Setposition(&hcan1,0x51,POSITION,Windmill_Target,7.0f);//右,正方向/大风车的方向到时候具体看
        //          target_position_2006 = -3600.0f;
        //      }
        //     //if (fabsf(DM_4310_position - Windmill_Target) < 0.05f && flag_4310_stop == 2)//当误差小于0.05时 判定为真
        //     //{
        //         //当达妙电机转360°时 2006开始下降到目标位置
        //
        //         //舵机代码+暂时延时处理
        //         if (fabsf(actuall_position_2006 - target_position_2006) < 0.5f)
        //         {
        //             if (flag_2006_test == 3)
        //            {
        //          //HAL_Delay(500);
        //          flag_servo = 3;
        //          flag_4310_stop = 0;
        //          flag_2006_test = 0;
        //                 flag_wait_3519 = 3;
        //                 t_3519 = osKernelGetTickCount();
        //          t0_4310 = osKernelGetTickCount();//获取当前时间
        //          //}//当误差小于0.05时 2006到达了指定位置 可以放飞镖
        //            }
        //     }
        //      if (osKernelGetTickCount()-t_3519>=1700u && flag_wait_3519 == 3)// 经过3.5s  确保放镖 且滑块到达指定放镖位置 舵机恢复初始状态
        //      {
        //          flag_wait_3519 = 0;//确保只进一次
        //          //__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3,630.0f);//放飞镖
        //      }
        //     if (osKernelGetTickCount()-t0_4310>=3500u && flag_servo == 3)//
        //     {
        //         flag_servo = 0;//确保该if只进一次
        //
        //
        //         //3s 过后 2006可以回到初始位置
        //         Windmill_Target =Windmill_Target + (PI/3.0f);
        //         DM_Motor_Setposition(&hcan1,0x51,POSITION,Windmill_Target,7.0f);//回到不干涉飞镖的位置
        //         target_position_2006 = 0.0f;
        //         __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3,400.0f);//爪子恢复初始状态
        //         flag_3519_back = 1;
        //
        //
        //
        //         if (fabsf(actuall_position_2006 - target_position_2006) < 0.5f)
        //         {
        //
        //            //do nothing
        //         }//当误差小于0.05时 即2006回到原点时
        //     }
        //     break;
        // }
        // if (flag_3519_back == 1)//达妙电机后退到指定位置以达到板机后
        // {
        //     DM_Motor_Setposition(&hcan2,0x00,POSITION,(700.0f),18.0f);//右,正方向 680
        //     DM_Motor_Setposition(&hcan2,0x01,POSITION,-(700.0f),18.0f);//左，正方向 电机继续后退
        // }
        osDelay(4);
    }
    /* USER CODE END Windmill_4310_Start */
}