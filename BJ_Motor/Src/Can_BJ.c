#include "Can_BJ.h"

__IO CAN_t can = {0};

extern CAN_HandleTypeDef hcan1;

void USER_CAN1_Filter_Init(void)
{
	CAN_FilterTypeDef  sFilterConfig;

	__IO uint8_t id_o, im_o; __IO uint16_t id_l, id_h, im_l, im_h;
	id_o = (0x00);
	id_h = (uint16_t)((uint16_t)id_o >> 5);
	id_l = (uint16_t)((uint16_t)id_o << 11) | CAN_ID_EXT;
	im_o = (0x00);
	im_h = (uint16_t)((uint16_t)im_o >> 5);
	im_l = (uint16_t)((uint16_t)im_o << 11) | CAN_ID_EXT;

	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = id_h;
	sFilterConfig.FilterIdLow = id_l;
	sFilterConfig.FilterMaskIdHigh = im_h;
	sFilterConfig.FilterMaskIdLow = im_l;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;//步进电机进FIFO0中断
	sFilterConfig.FilterActivation = ENABLE;
	//sFilterConfig.SlaveStartFilterBank = 0;
	sFilterConfig.SlaveStartFilterBank = 14;

	while(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK);
}

void can_SendCmd(__IO uint8_t *cmd, uint8_t len)
{
	static uint32_t TxMailbox; __IO uint8_t i = 0, j = 0, k = 0, l = 0, packNum = 0;

	j = len - 2;

	while(i < j)
	{
		k = j - i;

		can.CAN_TxMsg.StdId = 0x00;
		can.CAN_TxMsg.ExtId = ((uint32_t)cmd[0] << 8) | (uint32_t)packNum;
		can.txData[0] = cmd[1];
		can.CAN_TxMsg.IDE = CAN_ID_EXT;
		can.CAN_TxMsg.RTR = CAN_RTR_DATA;

		if(k < 8)
		{
			for(l=0; l < k; l++,i++) { can.txData[l + 1] = cmd[i + 2]; } can.CAN_TxMsg.DLC = k + 1;
		}
		else
		{
			for(l=0; l < 7; l++,i++) { can.txData[l + 1] = cmd[i + 2]; } can.CAN_TxMsg.DLC = 8;
		}

		//uint32_t t_start = HAL_GetTick();
		//while(HAL_CAN_AddTxMessage((&hcan1), (CAN_TxHeaderTypeDef *)(&can.CAN_TxMsg), (uint8_t *)(&can.txData), (&TxMailbox)) != HAL_OK)
		//{
		//	if (HAL_GetTick() - t_start > 10U) break;   // 10ms 超时，别死等
		//}

		++packNum;
	}
}