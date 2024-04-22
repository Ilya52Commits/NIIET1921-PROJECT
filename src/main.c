#include "plib035.h"

int main(void)
{
	/* Инициализация констант и переменных */
  uint32_t Time_Block = 1000000;							// Точка инверсирование пина
  uint32_t x = 0;															// Итерируемая переменная
	
	/* Настройка тактирования для порта А */
	RCU_AHBClkCmd(RCU_AHBClk_GPIOA, ENABLE); 		// RCU->HCLKCFG_bit.GPIOAEN = 1;
  RCU_AHBRstCmd(RCU_AHBClk_GPIOA, ENABLE); 		// RCU->HRSTCFG_bit.GPIOAEN = 1;

	/* Настройка пина 8 порта А	*/
	GPIO_Init_TypeDef GPIO_InitStruct;					// Инициализация структуры для GPIO
	
	// Устанавливаем необходимый пин
	GPIO_InitStruct.Pin = GPIO_Pin_8;
	GPIO_InitStruct.Out = ENABLE; 							// GPIOA->OUTENSET |= (1 << 8);
	// Передаём параметры в структуру GPIO_Init
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_DigitalCmd(GPIOA, GPIO_Pin_8, ENABLE); // GPIOA->DENSET |= (1 << 8);

  while (1)
  {
		// Если x равен Time_Block
    if (x == Time_Block)
    {
			// Инверсируем все биты 8-го пина
      GPIO_ToggleBits(GPIOA, GPIO_Pin_8);			//GPIOA->DATAOUTTGL |= (1 << 8);
			
			// Обнуляем x
      x = 0;																	
    }
		
		// Итерируем x
		x++; 																			
  }
}
