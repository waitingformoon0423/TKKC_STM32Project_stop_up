#include "seg.h"

/***********************************************************************
 * @brief  数码管显示码表定义
 * @note   共19个字符: 0~9, A~F, -, 空格, 小数点
 **********************************************************************/
uint16_t SegTab[] =
{
0x003f, 0x0006, 0x005b, 0x004f, // 0-3
0x0066, 0x006d, 0x007d, 0x0007, // 4-7 
0x007f, 0x006f, 0x0077, 0x007c, // 8-9,A-B
0x0039, 0x005e, 0x0079, 0x0071, // C-F
0x0040, 0x0000, 0x0080         // -,空格,小数点
};

/***********************************************************************
 * @brief  清除数码管显示
 * @param  None
 * @retval None
 **********************************************************************/
void SEG_ClearScreen(void)
{
    HAL_GPIO_WritePin(LNUM_GPIO_Port, 0x00ff, GPIO_PIN_RESET);
}

/***********************************************************************
 * @brief  设置单个数码管显示
 * @param  Index: SegTab数组的索引值
 * @retval None
 **********************************************************************/
void SEG_SetNum(uint8_t Index)
{
    if(Index<sizeof(SegTab))
    {
        HAL_GPIO_WritePin(LNUM_GPIO_Port, SegTab[Index], GPIO_PIN_SET);
    }
}

/***********************************************************************
 * @brief  静态显示单个字符
 * @param  Index: SegTab数组的索引值 
 * @retval None
 **********************************************************************/
void SEG_Static_Display(uint8_t Index)
{
    SEG_ClearScreen(); //清屏
    SEG_SetNum(Index);
}

/***********************************************************************
 * @brief  动态显示多个字符
 * @param  Indexes: 显示字符的索引数组
 * @param  Len: 显示字符的个数
 * @retval None
 **********************************************************************/
void SEG_Dynamic_Display(uint8_t *Indexes, uint8_t Len)
{
    for(uint8_t i=0; i<Len; i++)
    {
        SEG_ClearScreen(); //清屏
        HAL_GPIO_WritePin(LBIT_GPIO_Port, GPIO_PIN_All&0xff, GPIO_PIN_SET);   //位选全亮
        HAL_GPIO_WritePin(LBIT_GPIO_Port, GPIO_PIN_All&(0x01<<i), GPIO_PIN_RESET);//选中位
        SEG_SetNum(Indexes[i]);        
        HAL_Delay(1);
    }
}

/***********************************************************************
 * @brief  动态显示带小数点的多个字符
 * @param  Indexes: 显示字符的索引数组
 * @param  Len: 显示字符的个数
 * @param  Dp: 小数点位置
 * @retval None
 **********************************************************************/
void SEG_Dynamic_Display_Dot(uint8_t *Indexes, uint8_t Len, uint8_t Dp)
{
    for(uint8_t i=0; i<Len; i++)
    {
        SEG_ClearScreen(); //清屏
        HAL_GPIO_WritePin(LBIT_GPIO_Port, GPIO_PIN_All&0xff, GPIO_PIN_SET);   //位选全亮
        HAL_GPIO_WritePin(LBIT_GPIO_Port, GPIO_PIN_All&(0x01<<i), GPIO_PIN_RESET);//选中位
        if(i==Dp)
         HAL_GPIO_WritePin(LNUM_GPIO_Port, SegTab[Indexes[i]] | 0x80, GPIO_PIN_SET); 
        else
         HAL_GPIO_WritePin(LNUM_GPIO_Port, SegTab[Indexes[i]], GPIO_PIN_SET);
        HAL_Delay(1);
    }
}

/***********************************************************************
 * @brief  循环滚动显示
 * @param  Indexes: 显示字符的索引数组
 * @param  Len: 显示字符的个数
 * @param  Rate: 滚动速度
 * @retval None
 **********************************************************************/
void SEG_Cror(uint8_t *Indexes, uint8_t Len, uint8_t Rate)
{
    //循环左移一位
    uint8_t temp=Indexes[0];
    for(uint8_t i=0;i<(Len-1);i++)
    {
        Indexes[i]=Indexes[i+1];
    }
    Indexes[Len-1]=temp;
    //动态显示
    while(Rate--) SEG_Dynamic_Display(Indexes,Len);
}

/***********************************************************************
 * @brief  数字转换为指定进制的显示索引
 * @param  Number: 要转换的数字
 * @param  Base: 进制基数(2-16)
 * @param  Indexes: 转换后的索引数组
 * @param  Len: 数组长度
 * @retval 转换后的有效位数
 **********************************************************************/
uint8_t SEG_SetNumber_Base(uint32_t Number, uint8_t Base, uint8_t *Indexes, uint8_t Len)
{
	uint8_t quot=Number, count=0;
	if(Base<2||Base>16)return 0;
	do
	{
		Indexes[count]=quot%Base;
		quot=quot/Base;
		count++;
		if(count>(Len-1))break;
	}while(quot);
	for(uint8_t i=count; i<Len; i++)
	{
		Indexes[i]=17;
	}
	return count;
}
