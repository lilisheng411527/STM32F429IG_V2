/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "lwip.h"
#include "stdio.h"
#include "rtc.h"
#include "rng.h"
#include "fmc.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

extern RNG_HandleTypeDef hrng;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osThreadId myTask04Handle;
osThreadId myTask05Handle;
osThreadId myTask06Handle;
osTimerId myTimer01Handle;
osTimerId myTimer02Handle;
osTimerId myTimer03Handle;
osTimerId myTimer04Handle;
osTimerId myTimer05Handle;
osTimerId myTimer06Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);
void StartTask04(void const * argument);
void StartTask05(void const * argument);
void StartTask06(void const * argument);
void myTimer_Callback01(void const * argument);
void myTimer_Callback02(void const * argument);
void myTimer_Callback03(void const * argument);
void myTimer_Callback04(void const * argument);
void myTimer_Callback05(void const * argument);
void myTimer_Callback06(void const * argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];
  
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )  
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}                   
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* definition and creation of myTimer01 */
  osTimerDef(myTimer01, myTimer_Callback01);
  myTimer01Handle = osTimerCreate(osTimer(myTimer01), osTimerPeriodic, NULL);

  /* definition and creation of myTimer02 */
  osTimerDef(myTimer02, myTimer_Callback02);
  myTimer02Handle = osTimerCreate(osTimer(myTimer02), osTimerPeriodic, NULL);

  /* definition and creation of myTimer03 */
  osTimerDef(myTimer03, myTimer_Callback03);
  myTimer03Handle = osTimerCreate(osTimer(myTimer03), osTimerPeriodic, NULL);

  /* definition and creation of myTimer04 */
  osTimerDef(myTimer04, myTimer_Callback04);
  myTimer04Handle = osTimerCreate(osTimer(myTimer04), osTimerPeriodic, NULL);

  /* definition and creation of myTimer05 */
  osTimerDef(myTimer05, myTimer_Callback05);
  myTimer05Handle = osTimerCreate(osTimer(myTimer05), osTimerPeriodic, NULL);

  /* definition and creation of myTimer06 */
  osTimerDef(myTimer06, myTimer_Callback06);
  myTimer06Handle = osTimerCreate(osTimer(myTimer06), osTimerPeriodic, NULL);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 4096);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityIdle, 0, 128);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* definition and creation of myTask04 */
  osThreadDef(myTask04, StartTask04, osPriorityIdle, 0, 128);
  myTask04Handle = osThreadCreate(osThread(myTask04), NULL);

  /* definition and creation of myTask05 */
  osThreadDef(myTask05, StartTask05, osPriorityIdle, 0, 128);
  myTask05Handle = osThreadCreate(osThread(myTask05), NULL);

  /* definition and creation of myTask06 */
  osThreadDef(myTask06, StartTask06, osPriorityIdle, 0, 128);
  myTask06Handle = osThreadCreate(osThread(myTask06), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  
	//添加定时器启动函数
	osStatus	timeResult	=	osOK;
	timeResult	=	osTimerStart(myTimer01Handle,1000);//定时器1000ms
	
	if(osOK	==	timeResult)
	{
		printf("\r\n\r\nStart osTimer OK\r\n\r\n");
	}
	else	printf("\r\n\r\nStart osTimer Error\r\n\r\n"); 
  
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
	lwip_Get_IP();
  /* Infinite loop */
  for(;;)
  {
	  lwip_Get_IP();
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_WritePin(GPIOH,GPIO_PIN_10,GPIO_PIN_RESET);
    osDelay(200);
	  HAL_GPIO_WritePin(GPIOH,GPIO_PIN_10,GPIO_PIN_SET);
    osDelay(200);
	  HAL_GPIO_TogglePin(GPIOH,GPIO_PIN_11);
	osDelay(200);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */
  for(;;)
  {
	  //HAL_Delay(20);//消抖，有硬件消抖所以不使用
	  if ( HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == GPIO_PIN_SET)
	  {
		  while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) == GPIO_PIN_SET);
		  printf("\r\n ---GPIO_PIN_13_按键轮询响应 \r\n");
	  }
    osDelay(1);
  }
  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the myTask04 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void const * argument)
{
  /* USER CODE BEGIN StartTask04 */
  /* Infinite loop */
  for(;;)
  {
	  printf("\r\n PNG随机数Random 32bit Number:0x%08x !!! \r\n",HAL_RNG_GetRandomNumber(&hrng));
    osDelay(1000);
  }
  /* USER CODE END StartTask04 */
}

/* USER CODE BEGIN Header_StartTask05 */
/**
* @brief Function implementing the myTask05 thread.
* @param argument: Not used
* @retval None
*/
#include "stdio.h"

uint32_t RadomBuffer[10000];
uint32_t ReadBuffer[10000];
uint32_t *pSDRAM;
//#define SDRAM_BANK_ADDR ((uint32_t)0xD0000000)
long long count=0;

void SDRAM_Check(void)
{
    printf("开始生成100个SDRAM测试随机数\r\n");   

    pSDRAM=(uint32_t*)SDRAM_BANK_ADDR;
    count=1;
    for(count=1;count<=100;count++)
    {
        RadomBuffer[count]=HAL_RNG_GetRandomNumber(&hrng);
        *pSDRAM=RadomBuffer[count];
        pSDRAM++;
        printf("%8x,",RadomBuffer[count]);
        if (count % 10 == 0)
            printf("\r\n");
    }
    printf("\r\n写入SDRAM完成\r\n"); 
    printf("\r\n\r\n\r\n\r\n\r\n");

    pSDRAM=(uint32_t*)SDRAM_BANK_ADDR;
    count=1;
    for(count=1;count<=100;count++)

    {
        ReadBuffer[count]=*pSDRAM;
        pSDRAM++;
        printf("%8x,",ReadBuffer[count]);
        if (count % 10 == 0)
            printf("\r\n");
    }
    printf("\r\n读取SDRAM完成\r\n"); 

    count=0;
    for(count=0;count<100;count++)
    {
        if (ReadBuffer[count] != RadomBuffer[count])
            printf("\r\n\r\n\r\n ---测试失败（写入读取不一致）----  \r\n");
    }
    printf("\r\n\r\n\r\n ---测试成功----  \r\n");
}

/* USER CODE END Header_StartTask05 */
void StartTask05(void const * argument)
{
  /* USER CODE BEGIN StartTask05 */
	SDRAM_Check();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask05 */
}

/* USER CODE BEGIN Header_StartTask06 */
/**
* @brief Function implementing the myTask06 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask06 */
void StartTask06(void const * argument)
{
  /* USER CODE BEGIN StartTask06 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask06 */
}

/* myTimer_Callback01 function */
void myTimer_Callback01(void const * argument)
{
  /* USER CODE BEGIN myTimer_Callback01 */
	user_RTC_printf();
  /* USER CODE END myTimer_Callback01 */
}

/* myTimer_Callback02 function */
void myTimer_Callback02(void const * argument)
{
  /* USER CODE BEGIN myTimer_Callback02 */
  
  /* USER CODE END myTimer_Callback02 */
}

/* myTimer_Callback03 function */
void myTimer_Callback03(void const * argument)
{
  /* USER CODE BEGIN myTimer_Callback03 */
  
  /* USER CODE END myTimer_Callback03 */
}

/* myTimer_Callback04 function */
void myTimer_Callback04(void const * argument)
{
  /* USER CODE BEGIN myTimer_Callback04 */
  
  /* USER CODE END myTimer_Callback04 */
}

/* myTimer_Callback05 function */
void myTimer_Callback05(void const * argument)
{
  /* USER CODE BEGIN myTimer_Callback05 */
  
  /* USER CODE END myTimer_Callback05 */
}

/* myTimer_Callback06 function */
void myTimer_Callback06(void const * argument)
{
  /* USER CODE BEGIN myTimer_Callback06 */
  
  /* USER CODE END myTimer_Callback06 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
