/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes */
#include "main.h"
#include "cmsis_os.h"

/* Private includes */
/* USER CODE BEGIN Includes */
/* Здесь можно подключать дополнительные пользовательские заголовки. */
/* USER CODE END Includes */

/* Private typedef */
/* USER CODE BEGIN PTD */
/* Здесь размещают пользовательские typedef-определения. */
/* USER CODE END PTD */

/* Private define */
/* USER CODE BEGIN PD */
/* Здесь размещают пользовательские #define-константы. */
/* USER CODE END PD */

/* Private macro */
/* USER CODE BEGIN PM */
/* Здесь размещают пользовательские макросы. */
/* USER CODE END PM */

/* Private variables */
osThreadId defaultTaskHandle;    /* Хэндл фоновой задачи по умолчанию. */
osThreadId liCounterTaskHandle;  /* Хэндл задачи-счетчика для отслеживания времени. */
osThreadId liLoadTaskHandle;     /* Хэндл задачи, создающей вычислительную нагрузку. */
osThreadId liLEDTaskHandle;      /* Хэндл задачи управления светодиодами. */
osThreadId liButtonTaskHandle;   /* Хэндл задачи опроса кнопки. */
osThreadId liControlTaskHandle;  /* Хэндл задачи управления тактированием. */
osSemaphoreId liSemaphoreHandle; /* Семафор для сигнализации о нажатии кнопки. */

/* USER CODE BEGIN PV */
/* Глобальный счетчик тиков: увеличивается задачей counterTask. */
int liTickCounter = 0;
/* USER CODE END PV */

/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const *argument);
void counterTask(void const *argument);
void loadTask(void const *argument);
void ledTask(void const *argument);
void buttonTask(void const *argument);
void controlTask(void const *argument);
/* USER CODE BEGIN PFP */
/* Здесь можно объявлять дополнительные приватные функции. */
/* USER CODE END PFP */

/* Private user code */
/* USER CODE BEGIN 0 */
/* Здесь размещают вспомогательный пользовательский код и функции. */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* Здесь можно выполнить ранние пользовательские инициализации. */
  /* USER CODE END 1 */

  /* MCU Configuration */
  /* Сброс всех периферий, настройка Flash и SysTick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* Здесь можно выполнить действия до настройки тактирования. */
  /* USER CODE END Init */

  /* Configure the system clock */
  /* Настройка системного тактирования (низкая частота). */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* Дополнительные действия сразу после настройки тактирования. */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* Инициализация GPIO. */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  /* Здесь можно инициализировать пользовательские драйверы. */
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* Создание и настройка мьютексов RTOS (если нужны). */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of liSemaphore */
  /* Семафор создается с начальным количеством токенов = 1. */
  osSemaphoreDef(liSemaphore);
  liSemaphoreHandle = osSemaphoreCreate(osSemaphore(liSemaphore), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* Если нужны дополнительные семафоры — создаются здесь. */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* RTOS-таймеры можно создавать и запускать здесь. */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* RTOS-очереди можно создавать здесь. */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of liCounterTask */
  osThreadDef(liCounterTask, counterTask, osPriorityNormal, 0, 128);
  liCounterTaskHandle = osThreadCreate(osThread(liCounterTask), NULL);

  /* definition and creation of liLoadTask */
  osThreadDef(liLoadTask, loadTask, osPriorityAboveNormal, 0, 128);
  liLoadTaskHandle = osThreadCreate(osThread(liLoadTask), NULL);

  /* definition and creation of liLEDTask */
  osThreadDef(liLEDTask, ledTask, osPriorityHigh, 0, 128);
  liLEDTaskHandle = osThreadCreate(osThread(liLEDTask), NULL);

  /* definition and creation of liButtonTask */
  osThreadDef(liButtonTask, buttonTask, osPriorityHigh, 0, 128);
  liButtonTaskHandle = osThreadCreate(osThread(liButtonTask), NULL);

  /* definition and creation of liControlTask */
  osThreadDef(liControlTask, controlTask, osPriorityRealtime, 0, 128);
  liControlTaskHandle = osThreadCreate(osThread(liControlTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* Если нужны дополнительные задачи — создаются здесь. */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  /* Запуск планировщика RTOS — далее управление переходит задачам. */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* Обычно сюда не попадаем, RTOS захватывает управление. */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief  System Clock Configuration (низкая частота).
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Configure the main internal regulator output voltage */
  /* Включаем тактирование блока питания и настраиваем масштабирование. */
  HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /* Initializes the RCC Oscillators according to the specified parameters */
  /* Настраиваем внешний кварц (HSE) и PLL. */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE; /* Используем HSE. */
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                  /* Включаем HSE. */
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;              /* Включаем PLL. */
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;      /* PLL от HSE. */
  RCC_OscInitStruct.PLL.PLLM = 4;                           /* Делитель входа PLL. */
  RCC_OscInitStruct.PLL.PLLN = 64;                          /* Умножитель PLL. */
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;               /* Делитель выхода PLL. */
  RCC_OscInitStruct.PLL.PLLQ = 4;                           /* Делитель для USB/SDIO. */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Если настройка не удалась — переходим в обработчик ошибки. */
    Error_Handler();
  }

  /* Initializes the CPU, AHB and APB buses clocks */
  /* Настраиваем источники и делители системных шин. */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; /* SYSCLK от PLL. */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;        /* Делитель AHB. */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;         /* Делитель APB1. */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;         /* Делитель APB2. */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    /* Если настройка не удалась — переходим в обработчик ошибки. */
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  /* Включаем тактирование портов, которые будем использовать. */
  HAL_RCC_GPIOH_CLK_ENABLE();
  HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_RCC_GPIOG_CLK_ENABLE();

  /* Configure GPIO pin Output Level */
  /* Устанавливаем стартовый уровень для светодиодов. */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_RESET);

  /* Configure GPIO pin : PA0 */
  /* Настраиваем PA0 как вход (кнопка). */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configure GPIO pins : PG13 PG14 */
  /* Настраиваем PG13 и PG14 как выходы (светодиоды). */
  GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
/**
  * @brief  System Clock Configuration (высокая частота).
  * @retval None
  */
void SystemClock_Config2(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Configure the main internal regulator output voltage */
  /* Для высокой частоты нужен более высокий уровень питания. */
  HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Initializes the RCC Oscillators according to the specified parameters */
  /* Настройка HSE + PLL на максимальную частоту. */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initializes the CPU, AHB and APB buses clocks */
  /* Максимальная скорость шин, флэш с большей задержкой. */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief Function implementing the defaultTask thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const *argument)
{
  /* USER CODE BEGIN 5 */
  /* Бесконечный цикл: задача по умолчанию ничего не делает и отдает CPU. */
  for (;;)
  {
    /* Небольшая задержка, чтобы не занимать процессор впустую. */
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_counterTask */
/**
  * @brief Function implementing the liCounterTask thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_counterTask */
void counterTask(void const *argument)
{
  /* USER CODE BEGIN counterTask */
  /* Бесконечный цикл подсчета "тиков" (итераций). */
  for (;;)
  {
    /* Увеличиваем глобальный счетчик, используется в controlTask. */
    liTickCounter++;
    /* Даем другим задачам выполнить свою работу. */
    osDelay(1);
  }
  /* USER CODE END counterTask */
}

/* USER CODE BEGIN Header_loadTask */
/**
  * @brief Function implementing the liLoadTask thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_loadTask */
void loadTask(void const *argument)
{
  /* USER CODE BEGIN loadTask */
  /* Бесконечный цикл имитации нагрузки на CPU. */
  for (;;)
  {
    /* Пустой цикл для "сжигания" времени процессора. */
    for (uint32_t i = 0; i < 10000; i++)
    {
      /* Намеренно пусто: создаем вычислительную нагрузку. */
    }
    /* Небольшая задержка, чтобы не загружать CPU на 100%. */
    osDelay(10);
  }
  /* USER CODE END loadTask */
}

/* USER CODE BEGIN Header_ledTask */
/**
  * @brief Function implementing the liLEDTask thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_ledTask */
void ledTask(void const *argument)
{
  /* USER CODE BEGIN ledTask */
  /* Бесконечный цикл, обновляющий состояние светодиодов. */
  for (;;)
  {
    /* Если системная частота высокая (>= 168 МГц) — включаем PG14. */
    if (SystemCoreClock >= 168000000)
    {
      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
    }
    /* Иначе считаем, что частота ниже — включаем PG13. */
    else
    {
      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
    }

    /* Пауза между обновлениями, чтобы не дергать GPIO слишком часто. */
    osDelay(10);
  }
  /* USER CODE END ledTask */
}

/* USER CODE BEGIN Header_buttonTask */
/**
  * @brief Function implementing the liButtonTask thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_buttonTask */
void buttonTask(void const *argument)
{
  /* USER CODE BEGIN buttonTask */
  /* Бесконечный цикл опроса кнопки. */
  for (;;)
  {
    /* Читаем состояние кнопки на PA0. */
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
    {
      /* Если кнопка нажата — выдаем семафор для controlTask. */
      xSemaphoreGive(liSemaphoreHandle);
    }

    /* Задержка 100 мс для разгрузки CPU и простейшего "дребезга". */
    osDelay(100);
  }
  /* USER CODE END buttonTask */
}

/* USER CODE BEGIN Header_controlTask */
/**
  * @brief Function implementing the liControlTask thread.
  * @param argument: Not used
  * @retval None
  */
/* USER CODE END Header_controlTask */
void controlTask(void const *argument)
{
  /* USER CODE BEGIN controlTask */
  /* Бесконечный цикл управления частотой системы. */
  for (;;)
  {
    /* Ожидаем семафор до 5000 тиков: сигнал от buttonTask. */
    if (xSemaphoreTake(liSemaphoreHandle, 5000) &&
        (SystemCoreClock != 168000000))
    {
      /* При нажатии кнопки переключаемся на высокую частоту. */
      HAL_RCC_DeInit();
      SystemClock_Config2();
    }

    /* Если счетчик превысил порог — включаем низкую частоту. */
    if (liTickCounter > 3500)
    {
      if (SystemCoreClock != 10000000)
      {
        HAL_RCC_DeInit();
        SystemClock_Config();
      }
    }
    else
    {
      /* Иначе поддерживаем/включаем высокую частоту. */
      if (SystemCoreClock != 168000000)
      {
        HAL_RCC_DeInit();
        SystemClock_Config2();
      }
    }

    /* Сбрасываем счетчик после обработки логики. */
    liTickCounter = 0;
  }
  /* USER CODE END controlTask */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* Здесь можно реализовать пользовательский вывод ошибки/логирование. */
  __disable_irq();
  while (1)
  {
    /* Бесконечный цикл — остановка системы при критической ошибке. */
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
  /* Пример: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
