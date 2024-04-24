#include "plib035.h"

/* Инициализация констант и переменных */
uint32_t sys_tick_flag = 0;													// Инициализация флага таймера
uint32_t Time_Block = 450000;												// Точка инверсирование пина
uint32_t x = 0;																			// Итерируемая переменная

int main(void)
{
	/* Настройка системного таймера */
	SysTick->VAL = 0; 																// Установка значения
	SysTick->LOAD = 480000000 - 1; 										// Исходное значение
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;			// Откуда брать тики (в данном случае выставляется 1, т. е. с частоты процесосра)
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;				// Разрешение прерывания системного таймера

	NVIC_EnableIRQ(SysTick_IRQn); 										// Установка прерывания системного таймера.
	// В данном случае устанавливается разрешение прерывания с помощью регистра ISER (разрешение прерываний от источников)

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;					// Разрешаем таймеру считать
	
	/* Настройка тактирования для порта А */
	RCU_AHBClkCmd(RCU_AHBClk_GPIOA, ENABLE); 					// RCU->HCLKCFG_bit.GPIOAEN = 1;
  RCU_AHBRstCmd(RCU_AHBClk_GPIOA, ENABLE); 					// RCU->HRSTCFG_bit.GPIOAEN = 1;

	/* Настройка пина 8 порта А	*/
	GPIO_Init_TypeDef GPIO_InitStruct;								// Инициализация структуры для GPIO
	
	// Устанавливаем необходимый пин
	GPIO_InitStruct.Pin = GPIO_Pin_8;
	GPIO_InitStruct.Out = ENABLE; 										// GPIOA->OUTENSET |= (1 << 8);
	// Передаём параметры в структуру GPIO_Init
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	GPIO_DigitalCmd(GPIOA, GPIO_Pin_8, ENABLE); 			// GPIOA->DENSET |= (1 << 8);
	
	GPIO_InitStruct.Pin = GPIO_Pin_9;
	GPIO_InitStruct.Out = ENABLE; 										// GPIOA->OUTENSET |= (1 << 9);
	// Передаём параметры в структуру GPIO_Init
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_DigitalCmd(GPIOA, GPIO_Pin_9, ENABLE); 			// GPIOA->DENSET |= (1 << 9);
	
	GPIO_InitStruct.Pin = GPIO_Pin_10;
	GPIO_InitStruct.Out = ENABLE; 										// GPIOA->OUTENSET |= (1 << 10);
	// Передаём параметры в структуру GPIO_Init
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_DigitalCmd(GPIOA, GPIO_Pin_10, ENABLE); 			// GPIOA->DENSET |= (1 << 10);

  while (1)
  {
		// Если x равен Time_Block
		if (x == Time_Block) 
		{
			// Инверсируем все биты 8-го пина
			GPIO_ToggleBits(GPIOA, GPIO_Pin_8);						// GPIOA->DATAOUTTGL |= (1 << 8);
			// Обнуляем x
			x = 0;
		}
		
		// Итерируем x
		x++;
		
		// Если значение таймера достигает 0
		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
			GPIO_ToggleBits(GPIOA, GPIO_Pin_9);						// GPIOA->DATAOUTTGL |= (1 << 9);
		
		// Если флаг прерывания равен 1
    if (sys_tick_flag)
    {
			// Инверсируем все биты 9-го пина
      GPIO_ToggleBits(GPIOA, GPIO_Pin_10);					// GPIOA->DATAOUTTGL |= (1 << 9);						
			// Обнуляем флаг
			sys_tick_flag = 0;
    }																	
  }
}

/* Обработчик прерывания системного таймера */
void SysTick_Handler()
{
	// Ставим флаг прерывания в 1
	sys_tick_flag = 1; 
}

