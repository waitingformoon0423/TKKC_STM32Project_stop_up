#include "key.h"

/***********************************************************************
 * @brief  按键引脚状态宏定义
 * @param  无
 * @retval 无
 ***********************************************************************/
#define KEY0_PinState HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)
#define KEY1_PinState HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)
#define KEY_UP_PinState HAL_GPIO_ReadPin(KEY_UP_GPIO_Port,KEY_UP_Pin)

/***********************************************************************
 * @brief  检测按键按下状态
 * @param  无
 * @retval -1: 无按键按下
 *          0: KEY0 按下
 *          1: KEY1 按下
 *          2: KEY_UP 按下
 *          3: KEY0+KEY1 组合按下
 *          4: KEY0+KEY_UP 组合按下
 ***********************************************************************/
int8_t GetKey_Press(void)
{
    static uint8_t press_flag=1;
    if(press_flag==1&&(KEY0_PinState==0||KEY1_PinState==0||KEY_UP_PinState==1))
    {
        HAL_Delay(10);  // 消抖延时
        if(KEY0_PinState==0||KEY1_PinState==0||KEY_UP_PinState==1)
        {
            press_flag=0;
            if(KEY0_PinState==0)
            {
                if(KEY1_PinState==0) return 3;      // KEY0+KEY1
                if(KEY_UP_PinState==1) return 4;    // KEY0+KEY_UP
                return 0;                           // 仅KEY0
            }
            if(KEY1_PinState==0) return 1;         // 仅KEY1
            if(KEY_UP_PinState==1) return 2;       // 仅KEY_UP
        }
    }
    if(KEY0_PinState==1&&KEY1_PinState==1&&KEY_UP_PinState==0)
    {
        press_flag=1;
    }
    return -1;
}

/***********************************************************************
 * @brief  检测按键松开状态
 * @param  无
 * @retval -1: 按键未松开或无按键
 *          0: KEY0 松开
 *          1: KEY1 松开
 *          2: KEY_UP 松开
 * 这个实验中不使用
 ***********************************************************************/
int8_t GetKey_Loosen(void)
{
    static uint8_t press_flag=1;
    static int8_t key=-1;
    if(press_flag==1&&(KEY0_PinState==0||KEY1_PinState==0||KEY_UP_PinState==1))
    {
        HAL_Delay(10);  // 消抖延时
        if(KEY0_PinState==0||KEY1_PinState==0||KEY_UP_PinState==1)
        {
            press_flag=0;
            if(KEY0_PinState==0) key=0;
            if(KEY1_PinState==0) key=1;
            if(KEY_UP_PinState==1) key=2;
        }
    }
    if(press_flag==0&&KEY0_PinState==1&&KEY1_PinState==1&&KEY_UP_PinState==0)
    {
        press_flag=1;
        return key;
    }
    return -1;
}