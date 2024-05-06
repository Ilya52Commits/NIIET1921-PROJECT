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
	SysTick->LOAD = 50001001 - 1; 								// Исходное значение
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
	
  while (1)
  {	
		// Если x равен Time_Block
		if (x == Time_Block) 
		{
			// Инверсируем все биты 8-го пина
			GPIO_ToggleBits(GPIOA, GPIO_Pin_8);	// GPIOA->DATAOUTTGL |= (1 << 8);
		
			// Обнуляем x
			x = 0;
		}
	
		// Итерируем x
		x++;
		
		// Если значение таймера достигает 0
		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
			GPIO_ToggleBits(GPIOA, GPIO_Pin_9);	// GPIOA->DATAOUTTGL |= (1 << 9);
	
		// Если флаг прерывания равен 1
    if (sys_tick_flag)
    {
			// Инверсируем все биты 9-го пина
      GPIO_ToggleBits(GPIOA, GPIO_Pin_10);	// GPIOA->DATAOUTTGL |= (1 << 9);				
			// Обнуляем флаг
			sys_tick_flag = 0;
    }		
		
		// Если кнопка была нажата
		if (GPIOA->DATA & (1 << 7)) 
		{
			// Если статус нажатия равен 1
			if (status_button_click)
			{
				// Инверсируем все биты 12-го пина
				GPIO_ToggleBits(GPIOA, GPIO_Pin_12);	
				// Обнуляем статус нажатия
				status_button_click = 0; 
			}
			else 
			{
				// Устанавливаем пин на выход
				GPIO_OutCmd(GPIOA, GPIO_Pin_12, ENABLE);
				// Ставим статус нажатия 
				status_button_click = 1;
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
	GPIO_OutCmd(GPIOA, number_pin, ENABLE);				// Настройка пина на выход
  GPIO_DigitalCmd(GPIOA, number_pin, ENABLE);		// Включение цифровой функции
}
