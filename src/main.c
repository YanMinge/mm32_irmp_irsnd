#include "stdio.h"
#include "stdint.h"
#include "drv_gpio.h"
#include "drv_uart.h"
#include "drv_ring_buf.h"
#include "drv_timer.h"
#include "irmp.h"


#ifndef F_CPU
#error F_CPU unknown
#endif

// PA.4 used for IR receive, F_INTERRUPTS is 15000

void timer16_init (void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    drv_uart_printf(UART1, "F_CPU:%d\r\n", F_CPU);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_ClockDivision                 = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode                   = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Prescaler                     = 7;
    TIM_TimeBaseStructure.TIM_Period                        = (uint32_t)(((F_CPU / F_INTERRUPTS) / 80) * 1.12f ) - 1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter             = 0;
    TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);

    NVIC_InitStructure.NVIC_IRQChannel                      = TIM16_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPriority              = 0;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM16, ENABLE);
}

void TIM16_IRQHandler(void)                                                       // Timer16 Interrupt Handler
{
  TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
  (void) irmp_ISR();                                                              // call irmp ISR
  // drv_uart_printf(UART1, "test time14\r\n");
  // call other timer interrupt routines...
}

void timer2_init (void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_ClockDivision                 = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode                   = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Prescaler                     = 7;
    TIM_TimeBaseStructure.TIM_Period                        = (uint32_t)(((F_CPU / F_INTERRUPTS) / 80) * 1.12f ) - 1;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel                      = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPriority              = 0;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)                                                       // Timer2 Interrupt Handler
{
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  (void) irmp_ISR();                                                             // call irmp ISR
  // call other timer interrupt routines...
}

int main(void)
{
    IRMP_DATA irmp_data;
    IRMP_DATA irmp_data_old;
    systick_init();
    uart_ringbuf_init();
    drv_uart1_init(115200);
    irmp_init();
    timer16_init();
    // timer2_init();

    log_uart_printf(UART1, "hello yanminge\r\n");
    // unsigned long current_time = millis();
    while(1)
    {
        if (irmp_get_data (&irmp_data))
        {
            if((irmp_data.protocol != irmp_data_old.protocol) ||
               (irmp_data.address != irmp_data_old.address) ||
               (irmp_data.command != irmp_data_old.command) ||
               (irmp_data.flags != irmp_data_old.flags))
            {
                log_uart_printf(UART1, "IRMP_DATA>>  protocol:0x%x, address:0x%x, command:0x%x, flags:0x%x\r\n", irmp_data.protocol, irmp_data.address, irmp_data.command, irmp_data.flags);
                memcpy(&irmp_data_old, &irmp_data, sizeof(irmp_data));
            }
        }
    }
    return 0;
}
