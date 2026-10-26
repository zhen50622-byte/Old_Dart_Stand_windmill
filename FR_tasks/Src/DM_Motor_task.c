
//
// Created by 28416 on 2026/8/27
//
#include "FreeRTOS.h"
#include "task.h"
#include "DM_Motor_task.h"
volatile float DM_3519_position = 0.0f;
static uint8_t ch3_flag_last = 1;          // ch3摇杆上一次的状态
void DM_Motor_TaskStart(void *argument)
{
    DM_Motor_Init(&hcan2,0x00,POSITION);
    DM_Motor_Init(&hcan2,0x01,POSITION);

    for (;;)
    {
        if (RC_CtrlData.rc.s1 != s1_last)
        {
            if (RC_CtrlData.rc.s1 == 1)
            {
                DM_Motor_Init(&hcan2,0x02,POSITION);//初始化2325 用调参板调参时
            }
            else
            {
                DM_Motor_Init(&hcan2,0x02,SPEED);//初始化2325
            }
        }
        if (RC_CtrlData.rc.s1==3)//手动挡
        {
            //三为手动挡发镖
            flag_one_step = 0;//自动挡中途切回手动挡位 重置状态
            if (flag==0 && flag_3519_back != 1){

                DM_Motor_Setposition(&hcan2,0x00,POSITION,(750.0f),18.0f);//右,正方向 680 750测试用
                DM_Motor_Setposition(&hcan2,0x01,POSITION,-(750.0f),18.0f);//左，正方向
                if (ch3_flag_last != 0)
                {
                    ////DM_Motor_Setposition(&hcan2,0x00,POSITION,(35.0f*3.1415926f),4.0f*3.1415926f);//右,正方向
                    ////DM_Motor_Setposition(&hcan2,0x01,POSITION,-(35.0f*3.1415926f),4.0f*3.1415926f);//左，正方向

                    if (Windmill_flag >= 3)
                    {
                        Windmill_flag = 1;
                    }
                    else
                    {
                        Windmill_flag++;//大风车标志位
                    }
                }

            }
            else if (flag == 1)
            {
                DM_Motor_Setposition(&hcan2,0x00,POSITION,0,18.0f);
                DM_Motor_Setposition(&hcan2,0x01,POSITION,0,18.0f);
                flag_3519_back = 0;//复位标志位
            }
            DM_Motor_Setspeed(&hcan2,0x02,SPEED,((RC_CtrlData.rc.ch0-1024.0f)/660.0f)*3.0f*3.1415926f);//动态设置板机电机位置
            //左，后退/右，前进
            //暂时插入板机舵机
            if (RC_CtrlData.rc.dial<564)
            {
                __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1,780.0f);//上膛
            }
            else if (RC_CtrlData.rc.dial>1484)
            {
                __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1,620.0f);//发镖
            }
            else
            {
                //do nothing
            }
            // //让0出CPU
        }
        else if (RC_CtrlData.rc.s1==2)//下拨左摇杆 设置一键打镖 s2由下到中打一镖
        {
            DM_Motor_Setspeed(&hcan2,0x02,SPEED,((RC_CtrlData.rc.ch0-1024.0f)/660.0f)*3.0f*3.1415926f);//动态设置板机电机
            if (RC_CtrlData.rc.s2 == 2 && s2_last != 2)//摇杆以ms的速度进这个判断 但是flag_one_step和if (osKernelGetTickCount()-t0>=2700u)会防止一直进入case0 导致t0一直读取出现问题
            {
                flag_one_step = 1;//开始打镖
                t0 = osKernelGetTickCount();//更新时间
            }
                switch (flag_one_step)
                {
                case 0:
                    //do nothing
                    DM_Motor_Setposition(&hcan2,0x00,POSITION,0,18.0f);//默认上电回0
                    DM_Motor_Setposition(&hcan2,0x01,POSITION,0,18.0f);
                    break;
                case 1://电机向下
                    DM_Motor_Setposition(&hcan2,0x00,POSITION,(750.0f),18.0f);//右,正方向
                    DM_Motor_Setposition(&hcan2,0x01,POSITION,-(750.0f),18.0f);//左，正方向
                    if (osKernelGetTickCount()-t0>=3000u)
                    {
                        flag_one_step = 2;
                        t0 = osKernelGetTickCount();
                    }
                    break;
                case 2://电机回转
                    DM_Motor_Setposition(&hcan2,0x00,POSITION,0,18.0f);
                    DM_Motor_Setposition(&hcan2,0x01,POSITION,0,18.0f);
                    if (osKernelGetTickCount()-t0>=3000u)
                    {
                        flag_one_step = 3;
                        t0 = osKernelGetTickCount();
                    }
                    break;
                case 3://发镖
                    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1,620.0f);//发镖
                    if (osKernelGetTickCount()-t0>=500u)//停留500ms保证发镖
                    {
                        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1,780.0f);//上膛
                        flag_one_step = 0;
                    }
                    break;
            }
        }
        else if (RC_CtrlData.rc.s1 == 1 &&  RC_CtrlData.rc.s2 != 1)//在S1值为1或者3时 重置状态last_s2值 且模式为调参板调参
        {
             flag_one_step = 0;//重置状态 保证 自动挡不会跳步 正常进行
            // //s2_last = RC_CtrlData.rc.s2;
            //taskENTER_CRITICAL();
            switch (Dart_ID_state)
            {
            case 0x11:
                DM_Motor_Setposition(&hcan2,0x02,POSITION,D1_Data_Force.f,9.0f);
                break;
            case 0x12:
                DM_Motor_Setposition(&hcan2,0x02,POSITION,D2_Data_Force.f,9.0f);
                break;
            case 0x13:
                DM_Motor_Setposition(&hcan2,0x02,POSITION,D3_Data_Force.f,9.0f);
                break;
            case 0x14:
                DM_Motor_Setposition(&hcan2,0x02,POSITION,D4_Data_Force.f,9.0f);
                break;
            default:
                break;
            }
            //taskEXIT_CRITICAL();
            //发镖switch判断
            if (RC_CtrlData.rc.s2==2 && s2_last!=2)//摇杆以ms的速度进这个判断 但是flag_one_step和if (osKernelGetTickCount()-t0>=2700u)会防止一直进入case0 导致t0一直读取出现问题
            {
                flag_one_step = 1;//开始打镖
                t0 = osKernelGetTickCount();//更新时间
            }
            switch (flag_one_step)
            {
            case 0:
                //do nothing
                DM_Motor_Setposition(&hcan2,0x00,POSITION,0,18.0f);//默认上电回0
                DM_Motor_Setposition(&hcan2,0x01,POSITION,0,18.0f);
                break;
            case 1://电机向下
                DM_Motor_Setposition(&hcan2,0x00,POSITION,(800.0f),18.0f);//右,正方向
                DM_Motor_Setposition(&hcan2,0x01,POSITION,-(800.0f),18.0f);//左，正方向
                if (osKernelGetTickCount()-t0>=3000u)
                {
                    flag_one_step = 2;
                    t0 = osKernelGetTickCount();
                }
                break;
            case 2://电机回转
                DM_Motor_Setposition(&hcan2,0x00,POSITION,0,18.0f);
                DM_Motor_Setposition(&hcan2,0x01,POSITION,0,18.0f);
                if (osKernelGetTickCount()-t0>=3000u)
                {
                    flag_one_step = 3;
                    t0 = osKernelGetTickCount();
                }
                break;
            case 3://发镖
                __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1,620.0f);//发镖
                if (osKernelGetTickCount()-t0>=500u)//停留500ms保证发镖
                {
                    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1,780.0f);//上膛
                    flag_one_step = 0;
                }
                break;
            }

        }

        ch3_flag_last = flag;
        s2_last = RC_CtrlData.rc.s2;
        s1_last = RC_CtrlData.rc.s1;
        osDelay(2);
    }

}