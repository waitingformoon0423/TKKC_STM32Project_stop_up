/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *     [代码进度: ████░░░░░░ 40%]     // 这里应该是Bug free的
  *     [发量状态: ░░░░░░░░░░ 10%]     // 这里本来是有头发的
  *     [复习进度: ▁▂▃▄▅▆▇█ 99%]     // 这里本来是要复习的
  *          \ (×_×) /                 // 这里本来是笑脸的
  *           \   /                     // 这里本来是要睡觉的
  *            ----                     // 这里本来是要摸鱼的
  *    
  *    实践周の真相:
  *    【Week 1】
  *    - 周一: 立项     // 震惊！实践周只有2节课！
  *    - 周二: 摸鱼     // 震惊！还在等PPT模板！
  *    - 周三: 改进     // 震惊！原来要做这么多！
  *    - 周四: 优化     // 震惊！怎么还没做完！
  *    - 周五: 答辩     // 震惊！答辩还要改！
  *    【Week 2】
  *    - 重复 Week 1    // 震惊！为什么还要重复！
  *    
  *    校园生活の日常:
  *    1. 小绿自行车套餐:
  *       - 龙头歪得像实践周的进度
  *       - 刹车坏得像我的代码逻辑
  *       - 坐垫硬得像我的头发
  *       - 链条响得像项目演示时的心跳
  *    2. 漳州校区の真相:
  *       - 想回厦门要看轮渡时刻表
  *       - 学生证省6块能买两个臭豆腐
  *       - 来回就花了一天实践时间
  *       - 期待海底隧道通车.jpg
  *    
  *    3. 宁波の思念:
  *       - 想吃汤圆年糕配咸鸭蛋
  *       - 想坐地铁听"书藏古今,港通天下"
  *       - 想去北仑看看大海发发呆
  *       - 暑假要去宁波图书馆学习
  *       - 顺便去吃吃阿宁面馆的面条
  *    
  *    4. 南区生存指南:
  *       - 每天都在想为什么要选南区
  *       - 每天都在后悔为什么要选南区
  *       - 每天都在庆幸选了南区(食堂比中区好，离大门近)
  *    
  *    5. 实验室の日志:
  *       [08:00] 想写代码
  *       [10:00] 在写代码
  *       [12:00] 还在写代码
  *       [14:00] 代码写不完
  *       [16:00] 代码写不对
  *       [18:00] 代码写不动
  *       [20:00] 不想写代码
  *    
  *    6. 洗衣机の收费表:
  *       - 上上年: 3元/次
  *       - 上年: 4元/次
  *       - 今年: 5元/次
  *       - 下下年: 不敢想(破产)
  *                         
  * 学院      : 厦门大学嘉庚学院(在漳州的小岛)
  * 学号      : EMC23048
  * 邮箱      : lifelife919@qq.com
  * 项目名称  : 单片机实验A期末课设-秒表(附打点器)
  * 版本      : V1.0（头发掉光纪念版）
  * 日期      : 2025年6月（还在等实践周结束）
  * 
  * 注：致敬那些...
  *     - 被实践周耽误了假期的
  *     - 被小绿车伤害过的
  *     - 被洗衣机掏空过钱包的
  *     - 被南区锻炼出腿肌的
  *     - 还在等海底隧道通车的
  *     
  *     _____     ____    ____    _____
  *    |     |   |    |  |    |  |     |
  *    |     |   |    |  |    |  |     |
  *    |_____|   |____|  |____|  |_____|
  *    
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "seg.h"
#include "key.h"
#include "stdbool.h"

/* Private variables ---------------------------------------------------------*/
/***********************************************************************
 * @brief  秒表及打点相关全局变量定义
 ***********************************************************************/
uint8_t time[] = {0,0,16,0,0,16,0,0};    // 用于数码管显示的时间数组

// 秒表计时变量
uint16_t stopwatchSeconds = 0;            // 秒
uint16_t stopwatchMinutes = 0;            // 分
uint16_t stopwatchHours   = 0;            // 时
uint16_t count           = 0;             // 定时器计数，用于1Hz计时

bool RunningState = 0;                    // 秒表运行状态，0:停止，1:运行
bool LapLookState = 0;                    // 查看打点状态，0：普通，1:查看

#define MAX_LAPS 10                       // 最大打点（圈）数

typedef struct {
    uint16_t hours;                       // 圈的小时
    uint16_t minutes;                     // 圈的分钟
    uint16_t seconds;                     // 圈的秒
} LapTime;

LapTime laps[MAX_LAPS];                   // 打点（圈）时间数组
uint8_t lapCount = 0;                     // 当前已记录圈数
uint8_t lapIndex = 0;                     // 当前显示的圈索引

/* Private function prototypes ---------------------------------------------*/
void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* MCU Configuration--------------------------------------------------------*/
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM3_Init();

    /* 初始化显示 */
    time[2] = 16;
    time[5] = 16; 

    /* 启动定时器和显示 */
    SEG_Dynamic_Display(time, sizeof(time));
    __HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&htim3);

    /* Infinite loop */
    while (1) {
        int8_t keyPress = GetKey_Press();     // 检查按键
        
        switch (keyPress) {
            case -1:    // 无按键处理
                if (!LapLookState) {
                    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
                }
                break;
            
            case 0:     // 启动/暂停功能
                HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                RunningState = !RunningState;
                LapLookState = false;
                HAL_Delay(200);
                break;

            case 1:     // 复位功能：清零当前计时
                LapLookState = false;
                RunningState = 0;
                for (uint8_t i = 0; i < 3; i++) {
                    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
                    HAL_Delay(50);
                    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
                    HAL_Delay(50);
                }
                stopwatchHours = stopwatchMinutes = stopwatchSeconds = count = 0;
                HAL_Delay(200);
                break;
            
            case 2:     // 打点功能：记录当前时间
                LapLookState = false;
                for (uint8_t i = 0; i < 3; i++) {
                    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
                    HAL_Delay(10);
                    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
                    HAL_Delay(10);
                }
                if (RunningState && lapCount < MAX_LAPS) {
                    laps[lapCount].hours = stopwatchHours;
                    laps[lapCount].minutes = stopwatchMinutes;
                    laps[lapCount].seconds = stopwatchSeconds;
                    lapCount++;
                }
                HAL_Delay(200);
                break;
            
            case 3:     // 切换显示已记录的圈时间
                LapLookState = true;
                HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

                if (lapCount > 0) {
                    RunningState = 0;
                    lapIndex = (lapIndex >= lapCount) ? 0 : lapIndex;
                    
                    stopwatchHours = laps[lapIndex].hours;
                    stopwatchMinutes = laps[lapIndex].minutes;
                    stopwatchSeconds = laps[lapIndex].seconds;
                    
                    lapIndex = (++lapIndex >= lapCount) ? 0 : lapIndex;
                }
                HAL_Delay(200);
                break;
            
            case 4:     // 清空所有打点记录和计时器
                LapLookState = false;
                for (uint8_t i = 0; i < 6; i++) {
                    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
                    HAL_Delay(10);
                }
                
                for (uint8_t i = 0; i < MAX_LAPS; i++) {
                    laps[i].hours = laps[i].minutes = laps[i].seconds = 0;
                }
                
                stopwatchHours = stopwatchMinutes = stopwatchSeconds = count = 0;
                RunningState = 0;
                lapCount = lapIndex = 0;
                break;
        }

        /* 更新显示 */
        time[0] = stopwatchSeconds % 10;
        time[1] = stopwatchSeconds / 10;
        time[3] = stopwatchMinutes %10;
        time[4] = stopwatchMinutes / 10;
        time[6] = stopwatchHours % 10;
        time[7] = stopwatchHours / 10;
        
        SEG_Dynamic_Display(time, sizeof(time));
    }
}

/**
  * @brief  定时器中断回调函数
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3 && RunningState) {
        if (++count >= 60) {
            count = 0;
            stopwatchSeconds = 0;
            if (++stopwatchMinutes >= 60) {
                stopwatchMinutes = 0;
                if (++stopwatchHours >= 100) {
                    stopwatchHours = 0;
                }
            }
        }
        stopwatchSeconds++;
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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

#ifdef  USE_FULL_ASSERT
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
