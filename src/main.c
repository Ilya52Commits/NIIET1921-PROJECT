#include "plib035.h"

/* Инициализация констант и переменных */
uint32_t sys_tick_flag = 0;				// Инициализация флага таймера
uint32_t Time_Block = 450000;			// Точка инверсирование пина
uint32_t x = 0;										// Итерируемая переменная
uint32_t status_button_click = 0;	// Инициализация флага состояния кнопки

void gpioa_init(unsigned long number_pin);	// Функция для инициализации пинов

int main(void)
{
	/* Настройка тактирования */
	RCU_SysClkChangeCmd(RCU_SysClk_PLLClk);						// Переключение тактирования на PLL
	RCU_PLL_AutoConfig(30000000, RCU_PLL_Ref_OSEClk);	// Конфигурация PLL
	
	/* Настройка системного таймера */
	SysTick->VAL = 0; 														// Установка значения
	SysTick->LOAD = 60001001 - 1; 								// Исходное значение
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;	// Откуда брать тики (в данном случае выставляется 1, т. е. с частоты процесосра)
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;		// Разрешение прерывания системного таймера

	NVIC_EnableIRQ(SysTick_IRQn); 								// Установка прерывания системного таймера.
	// В данном случае устанавливается разрешение прерывания с помощью регистра ISER (разрешение прерываний от источников)

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;			// Разрешаем таймеру считать
	
	/* Настройка тактирования для порта А */
	RCU_AHBClkCmd(RCU_AHBClk_GPIOA, ENABLE);	// RCU->HCLKCFG_bit.GPIOAEN = 1;
  RCU_AHBRstCmd(RCU_AHBClk_GPIOA, ENABLE); 	// RCU->HRSTCFG_bit.GPIOAEN = 1;

	/* Настройка пинов порта А	*/
	// Настройка переферии при нажатии кнопки
	GPIO_DigitalCmd(GPIOA, GPIO_Pin_7, ENABLE);		// Включение цифровой функции для кнопки (пин 7)
	GPIO_DigitalCmd(GPIOA, GPIO_Pin_12, ENABLE);	// Включение цифровой функции для светодиода (пин 12)
	
	// Настройкк светодиодов
	gpioa_init(GPIO_Pin_8);		// Инициализация пина 8
	gpioa_init(GPIO_Pin_9);		// Инициализация пина 9
	gpioa_init(GPIO_Pin_10);	// Инициализация пина 10
	gpioa_init(GPIO_Pin_11);	// Инициализация пина 11
	gpioa_init(GPIO_Pin_12);	// Инициализация пина 12
	gpioa_init(GPIO_Pin_13);	// Инициализация пина 13
	gpioa_init(GPIO_Pin_14);	// Инициализация пина 14
	gpioa_init(GPIO_Pin_15);  // Инициализация пина 15
	
	unsigned long pins[8] = {GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11, 
													 GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};
	int length = sizeof(pins) / sizeof(pins[0]); // Определение длины массива
														
  while (1)
  {	
		if (GPIOA->OUTENSET & (1 << 8)) 
		{
			for (int i = 0; i < length; i++) 
			{
				for (int j = 0; j < 140000; j++)
				GPIO_OutCmd(GPIOA, pins[i], DISABLE);
			}
		}
		else
		{
			for (int i = 0; i < length; i++) 
			{
				for (int j = 0; j < 140000; j++)
				GPIO_OutCmd(GPIOA, pins[i], ENABLE);
			}		
		}
  }     
}

/* Обработчик прерывания системного таймера */
void SysTick_Handler()
{
	// Ставим флаг прерывания в 1
	sys_tick_flag = 1; 
}

/* Инициализация пинов */
void gpioa_init(unsigned long number_pin) 
{
	//GPIO_OutCmd(GPIOA, number_pin, ENABLE);				// Настройка пина на выход
  GPIO_DigitalCmd(GPIOA, number_pin, ENABLE);		// Включение цифровой функции
}
