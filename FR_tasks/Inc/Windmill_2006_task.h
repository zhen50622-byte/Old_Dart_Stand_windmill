//
// Created by 28416 on 2026/10/19.
//

#ifndef OLD_DART_STAND_WINDMILL_2006_TASK_H
#define OLD_DART_STAND_WINDMILL_2006_TASK_H
#include "cmsis_os2.h"
#include "PID.h"
#include "drv_can.h"
extern PID_Struct DJI_2006_speed;
extern PID_Struct DJI_2006_position;
extern volatile float actuall_position_2006;   // CAN中断里写、任务里读
extern volatile float actuall_speed_2006;
extern volatile uint8_t flag_FIF0;
extern volatile uint16_t actuall_position_2006_raw;
extern volatile float actuall_speed_2006_raw;
extern float target_position_2006;
#endif //OLD_DART_STAND_WINDMILL_2006_TASK_H