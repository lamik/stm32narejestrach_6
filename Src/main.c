/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Mateusz Salamon
 * @brief          : STM32 na Rejestrach
 ******************************************************************************
 ******************************************************************************

 	 	 	 	 https://msalamon.pl
 	 	 	 	 https://sklep.msalamon.pl
 	 	 	 	 https://kursstm32.pl
 	 	 	 	 https://stm32narejestrach.pl

 */
#include "main.h"

// Constants for Software Timer's actions
#define UART2_TIMER 500

// Tick for System Time
__IO uint32_t Tick;

// UART Configuration
void UART2_Config(void);

// Get current System Time
uint32_t GetSystemTick(void);

// Software Timers variables
uint32_t Timer_UART2;

int main(void)
{
	// 1s = 12 000 000
	// 0,001 = X
	SysTick_Config(12000000 / 1000);

	// Configure UART
	UART2_Config();

	// Software Timers - first fill
	Timer_UART2 = GetSystemTick();

    /* Loop forever */
	while(1)
	{
		// UART2 transmit
		if((GetSystemTick() - Timer_UART2) > UART2_TIMER)
		{
			Timer_UART2 = GetSystemTick();

			// Put data to transmit register
			USART2->TDR = 'A';

			// ISR TXFNF
			while(!(USART2->ISR & USART_ISR_TXE_TXFNF))
			{
				// Do nothing, wait for end of transmisssion
			}
		}

	}
}


// PA2 - TX PA3 - RX

void UART2_Config(void)
{
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

	// Pin PA2 - TX AF Mode
	GPIOA->MODER &= ~(GPIO_MODER_MODE2_0);
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0;

	// Pin PA3 - RX AF Mode
	GPIOA->MODER &= ~(GPIO_MODER_MODE3_0);
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0;
	GPIOA->OTYPER |= GPIO_OTYPER_OT3;

	// Enable USART2 Clock
	RCC->APBENR1 |= RCC_APBENR1_USART2EN;

	// 115200 baudrate
	USART2->BRR = 12000000/115200;

//	USART2->CR1 |= USART_CR1_UE; // UART Enable
//	USART2->CR1 |= USART_CR1_TE; // Transmitter Enable`
//	USART2->CR1 |= USART_CR1_RE; // Receiver Enable

	USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}


void SysTick_Handler(void)
{
	Tick++; // Increase system timer
}

uint32_t GetSystemTick(void)
{
	return Tick; // Return current System Time
}

