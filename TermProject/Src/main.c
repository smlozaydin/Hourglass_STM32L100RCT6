/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void port_yaz(int s){
		GPIOA -> ODR = s;
}
void write_byte(uint8_t byte){
	for(int i =0; i<8; i++){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,byte &0x80);
		byte = byte<<1;	
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);		
	}
}

void write_max(uint8_t address,uint8_t data){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		write_byte(address);
		write_byte(data);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
}
void max_init(void)
{
 write_max(0x09, 0x00);       //  no decoding
 write_max(0x0a, 0x03);       //  brightness intensity
 write_max(0x0b, 0x07);       //  scan limit = 8 LEDs
 write_max(0x0c, 0x01);       //  power down =0,normal mode = 1
 write_max(0x0f, 0x00);       //  no test display
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

HAL_StatusTypeDef ret;

//MPU6050 degilkenleri
#define MPU6050_ADDR 0xD0
#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75
int16_t Accel_X_RAW= 0;
int16_t Accel_Y_RAW= 0;
int16_t Accel_Z_RAW= 0;

int16_t Gyro_X_RAW = 0;
int16_t Gyro_Y_RAW = 0;
int16_t Gyro_Z_RAW = 0;
float Ax, Ay, Az, Gx, Gy, Gz;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*uint8_t disp1ayDeneme[48][8]={
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},//FULL
{0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
{0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
{0xFC,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},

{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0
{0x10,0x30,0x50,0x10,0x10,0x10,0x10,0x7c},//1
{0x7E,0x2,0x2,0x7E,0x40,0x40,0x40,0x7E},//2
{0x3E,0x2,0x2,0x3E,0x2,0x2,0x3E,0x0},//3
{0x8,0x18,0x28,0x48,0xFE,0x8,0x8,0x8},//4
{0x3C,0x20,0x20,0x3C,0x4,0x4,0x3C,0x0},//5
{0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x0},//6
{0x3E,0x22,0x4,0x8,0x8,0x8,0x8,0x8},//7
{0x0,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E},//8
{0x3E,0x22,0x22,0x3E,0x2,0x2,0x2,0x3E},//9
{0x18,0x24,0x42,0x42,0x7E,0x42,0x42,0x42},//A
{0x3C,0x22,0x22,0x3c,0x22,0x22,0x3C,0x0},//B
{0x3C,0x40,0x40,0x40,0x40,0x40,0x40,0x3C},//C
{0x7C,0x22,0x22,0x22,0x22,0x22,0x22,0x7C},//D
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C},//E
{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40},//F
{0x3C,0x40,0x40,0x40,0x4c,0x44,0x44,0x3C},//G
{0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44},//H
{0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C},//I
{0x3C,0x8,0x8,0x8,0x8,0x8,0x48,0x30},//J
{0x0,0x24,0x28,0x30,0x20,0x30,0x28,0x24},//K
{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C},//L
{0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81},//M
{0x0,0x42,0x62,0x52,0x4A,0x46,0x42,0x0},//N
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//O
{0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20},//P
{0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D},//Q
{0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21},//R
{0x0,0x1E,0x20,0x20,0x3E,0x2,0x2,0x3C},//S
{0x0,0x3E,0x8,0x8,0x8,0x8,0x8,0x8},//T
{0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C},//U
{0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},//V
{0x0,0x49,0x49,0x49,0x49,0x2A,0x1C,0x0},//W
{0x0,0x41,0x22,0x14,0x8,0x14,0x22,0x41},//X
{0x41,0x22,0x14,0x8,0x8,0x8,0x8,0x8},//Y
{0x0,0x7F,0x2,0x4,0x8,0x10,0x20,0x7F},//Z
};*/

uint8_t disp1ay[4][8]={
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},//FULL
{0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
{0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
{0xFC,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}

};
//Burada bir algorimta gelistirme uzerinde durulabilir. Tek tek yazmaktansa
uint8_t disp1ay2 [24][8] = 
{
	{0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
	{0x0,0x2,0x0,0x0,0x0,0x0,0x0,0x0},
	{0x0,0x0,0x4,0x0,0x0,0x0,0x0,0x0},
	{0x0,0x0,0x0,0x8,0x0,0x0,0x0,0x0},
	{0x0,0x0,0x0,0x0,0x10,0x0,0x0,0x0},
	{0x0,0x0,0x0,0x0,0x0,0x20,0x0,0x0},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x40,0x0},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80},
	
	
	{0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x80},
	{0x0,0x2,0x0,0x0,0x0,0x0,0x0,0x80},
	{0x0,0x0,0x4,0x0,0x0,0x0,0x0,0x80},
	{0x0,0x0,0x0,0x8,0x0,0x0,0x0,0x80},
	{0x0,0x0,0x0,0x0,0x10,0x0,0x0,0x80},
	{0x0,0x0,0x0,0x0,0x0,0x20,0x0,0x80},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x40,0x80},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0},

	
	{0x1,0x0,0x0,0x0,0x0,0x0,0x0,0xC0},
	{0x0,0x2,0x0,0x0,0x0,0x0,0x0,0xC0},
	{0x0,0x0,0x4,0x0,0x0,0x0,0x0,0xC0},
	{0x0,0x0,0x0,0x8,0x0,0x0,0x0,0xC0},
	{0x0,0x0,0x0,0x0,0x10,0x0,0x0,0xC0},
	{0x0,0x0,0x0,0x0,0x0,0x20,0x0,0xC0},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x40,0xC0},
	{0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xC0},
	
	
	/*{0b00000001,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b10000000},
	{0b00000000,0b00000010,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b10000000},
	{0b00000000,0b00000000,0b00000100,0b00000000,0b00000000,0b00000000,0b00000000,0b10000000},
	{0b00000000,0b00000000,0b00000000,0b00001000,0b00000000,0b00000000,0b00000000,0b10000000},
	{0b00000000,0b00000000,0b00000000,0b00000000,0b00010000,0b00000000,0b00000000,0b10000000},
	{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00100000,0b00000000,0b10000000},
	{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b01000000,0b10000000},
	{0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b11000000,0b11000000}*/
};

//----------------------------------------------------
void MPU6050_Init(void){ 
 	uint8_t check, Data; 
 	 
 	// check device id 
 	HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, WHO_AM_I_REG,1, &check, 1, 1000);  	
	
	if (check == 0x68){ // boyle bir cihaz varsa 
 	 	    // power management regisgter 0x6b power up 
 	 	 	 	Data = 0; 
 	 	 	 	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 
					PWR_MGMT_1_REG,1, &Data, 1, 1000); 
 	 	 	 	// 
 	 	 	 	Data = 0x07; 
 	 	 	 	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 
					SMPLRT_DIV_REG,1, &Data, 1, 1000); 
 	 	 	 	// set acc. meter configuration in ACCEL_CONFIG reg. 
 	 	 	 	Data = 0x00; 
 	 	 	 	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 
					ACCEL_CONFIG_REG,1, &Data, 1, 1000); 
 	 	 	 	 
 	 	 	 	// Set Gyroscopic configuration in GYRO_CONFIG Register 
 	 	 	 	// XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ? 250 ?/s 
 	 	 	 	Data = 0x00; 
 	 	 	 	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 
					GYRO_CONFIG_REG, 1, &Data, 1, 1000); 
 	} 
} 
void MPU6050_Read_Accel (void) 
{ 
 	uint8_t Rx_data[6]; 
 	// Read 6 BYTES of data starting from ACCEL_XOUT_H register 
 	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, ACCEL_XOUT_H_REG,1, Rx_data, 6, 1000); 
 	Accel_X_RAW = (int16_t)(Rx_data[0] << 8 | Rx_data [1]); 
 	Accel_Y_RAW = (int16_t)(Rx_data[2] << 8 | Rx_data [3]); 
 	Accel_Z_RAW = (int16_t)(Rx_data[4] << 8 | Rx_data [5]); 
 	Ax = Accel_X_RAW/16384.0; 
 	Ay = Accel_Y_RAW/16384.0; 
 	Az = Accel_Z_RAW/16384.0; 
 	} 
 void MPU6050_Read_Gyro (void) 
{ 
 	uint8_t Rx_data[6]; 
 	// read 6 bytes of data starting from GYRO_XOUT_H reg. 
 	 	HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, GYRO_XOUT_H_REG,1, Rx_data, 6, 1000); 
 	 
 	 	Gyro_X_RAW = (int16_t)(Rx_data[0] << 8 | Rx_data [1]); 
 	 	Gyro_Y_RAW = (int16_t)(Rx_data[2] << 8 | Rx_data [3]); 
 	 	Gyro_Z_RAW = (int16_t)(Rx_data[4] << 8 | Rx_data [5]); 
 	   
 	  Gx = Gyro_X_RAW/131.0; 
 	 	Gy = Gyro_Y_RAW/131.0; 
 	 	Gz = Gyro_Z_RAW/131.0; 
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	max_init();
//	MPU6050_Init();
	//uint8_t MPUtest;
	//HAL_I2C_Mem_Read(&hi2c1,MPU6050_ADDR,WHO_AM_I_REG,1,&MPUtest,1,1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		/*HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		port_yaz(0x01);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);

		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
		port_yaz(0x07);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);*/
		for (int j=0; j<24; j++)
		{
			for (int i=1; i<9; i++)
			{
			//write_max (i , disp1ay[j][i-1]);
				write_max (i , disp1ay2[j][i-1]);
				MPU6050_Read_Accel();
				MPU6050_Read_Gyro();	
			}
			
			HAL_Delay (700);

			}
		}
	//	HAL_Delay(3000);

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Clock_Pin|CS_Pin|Data_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Clock_Pin CS_Pin Data_Pin */
  GPIO_InitStruct.Pin = Clock_Pin|CS_Pin|Data_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

