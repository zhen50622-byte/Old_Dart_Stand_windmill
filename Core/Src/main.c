/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"DM_Motor.h"
#include "drv_can.h"
#include "RC_remoto_control.h"
#include "X_V2.h"
#include "PID.h"
#define PI 3.1415926
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//遥控器的全局变量(以后优化)
int flag=2;//达妙电机标志位,默认上电不动
RC_Ctl_t RC_CtrlData;
//uint8_t RxData[2][18]={0};//接收遥控器数据的数组，双
uint8_t RxData[18] = {0};//接收遥控器数据的数,单缓冲区
int flag_one_step = 0;//此为全局变量
uint32_t t0 = 0;
uint8_t s2_last = 0;
uint8_t s1_last = 0;
extern PID_Struct DJI_2006_speed;
extern PID_Struct DJI_2006_position;
extern uint8_t Windmill_flag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void RemoteDataProcess(uint8_t *pData)//解析数据
{
  if(pData == NULL)
  {
    return;
  }

  RC_CtrlData.rc.ch0 = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF;
  RC_CtrlData.rc.ch1 = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5))
& 0x07FF;
  RC_CtrlData.rc.ch2 = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) |
                       ((int16_t)pData[4] << 10)) & 0x07FF;
  RC_CtrlData.rc.ch3 = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5]<<7)) &
0x07FF;

  RC_CtrlData.rc.s1 = ((pData[5] >> 4) & 0x000C) >> 2;
  RC_CtrlData.rc.s2 = ((pData[5] >> 4) & 0x0003);

  RC_CtrlData.mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8);
  RC_CtrlData.mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8);
  RC_CtrlData.mouse.z = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8);

  RC_CtrlData.mouse.press_l = pData[12];
  RC_CtrlData.mouse.press_r = pData[13];

  RC_CtrlData.key.v = ((int16_t)pData[14]);// | ((int16_t)pData[15] << 8); ;//上拨最小，下拨最大
  RC_CtrlData.rc.dial = pData[16] | (pData[17] << 8);//上拨最小，下拨最大
  //your control code ….
  if (RC_CtrlData.rc.ch3>1024)//向上拨摇杆，
  {
    flag=1;
  }
  else if (RC_CtrlData.rc.ch3<1024)//向下拨摇杆
  {
    flag=0;
  }
  else
  {
    //do nothing
  }
}
//重写串口空闲中断
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if(huart==&huart1)
  {
    uint8_t*pData=RxData;
    if (Size==18)//判断是否接收的是完整的数据据
    {
      RemoteDataProcess(pData);
    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxData, 18);//持续接收数据
    __HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
  }
  //优化为双缓冲区
   // if (huart == &huart1)
   // {
   //   static uint8_t idx = 0;// 记录"刚刚收完"的是哪块缓冲
   //   uint8_t *done = RxData[idx];     // 刚收满的一帧
   //
   //   idx ^= 1;                        // 切到另一块(0<->1)
   //
   //   HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxData[idx], 18); // 先武装,让 DMA 去填另一块
   //   __HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);
   //
   //   // 现在解析上一帧,此时 DMA 正在写另一块 → 真正并行,不会互相覆盖
   //   if (Size==18)
   //   {
   //     RemoteDataProcess(done);
   //   }
   // }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  //舵机初始化代码
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);//扳机舵机开启
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);//大风车舵机1初始化
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);//大风车舵机2初始化
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);//大风车舵机3初始化
  //初始化大风车舵机上电实际角度 具体角度看实际来调
  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1,780.0f);//初始未上膛
  __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1,320.0f);//初始未上膛//200.0放飞镖 320夹飞镖
  __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2,600.0f);//初始未上膛//750放飞镖 600夹紧
  __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3,400.0f);//初始未上膛//630放飞镖 400夹紧  //

  //遥控器串口接收代码
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxData, 18);//开启串口
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx,DMA_IT_HT);//关掉有关DMA的接收一半数据就触发的中断

  CAN_Filter_Mask_Config(&hcan2,CAN_FILTER(16)|CAN_FIFO_1|CAN_STDID|CAN_DATA_TYPE,0x01,0x00);//配置滤波器 接收3519发来的数据
  CAN_Init(&hcan2,User_CANCallback);//初始化CN2 使用达妙电机



  // //步进电机相关代码 并且初始化CAN1
  // USER_CAN1_Filter_Init();																	// 初始化CAN滤波器
  // if(HAL_CAN_Start(&hcan1) != HAL_OK) { Error_Handler(); }	// 启动CAN控制器
  // if(HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) { Error_Handler(); }	// 使能CAN控制器接收中断
  CAN_Filter_Mask_Config(&hcan1,CAN_FILTER(1)|CAN_FIFO_0|CAN_STDID|CAN_DATA_TYPE,0x41,0x00);//配置滤波器//接收4310发来的数据
  CAN_Filter_Mask_Config(&hcan1,CAN_FILTER(2)|CAN_FIFO_0|CAN_STDID|CAN_DATA_TYPE,0x202,0x00);//配置滤波器//接收4310发来的数据
  CAN_Init(&hcan1,User_CANCallback);//初始化CN2 使用达妙电机


   //初始化4310电机,变成位置模式
   DM_Motor_Init(&hcan1,0x51,POSITION);
   DM_Motor_Setposition(&hcan1,0x51,POSITION,0.0f,10.0f);//先让达妙电机上电回到0点
   HAL_Delay(1200);
   DM_Motor_Setposition(&hcan1,0x51,POSITION,-(PI/3.0f),10.0f);//然后让电机转到60°的位置
   HAL_Delay(500);

  // HAL_Delay(500);//上电500ms等初始化完成
  //  //初始化大疆电机2006
  //  PID_Init(&DJI_2006_speed);
  //  PID_Init(&DJI_2006_position);






  ////X_V2_Reset_CurPos_To_Zero(8);//上电 设置步进电机的当前位置为0点 0点学长已经设置过了
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM14)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
