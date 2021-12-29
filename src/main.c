#include "stdio.h"
#include "stdint.h"
#include "drv_gpio.h"
#include "drv_uart.h"
#include "drv_ring_buf.h"
#include "drv_timer.h"
#include "irmp.h"
#include "irsnd.h"

#ifndef F_CPU
#error F_CPU unknown
#endif

#define ONLY_SEND_CMD                  1

#if (MATATALAB_FEATURE == 1)
// ir_universal_data_type ir_data_buf[76] = 
// {
//     0x00 ,0x02 ,0x00 ,0xCE ,0x01 ,0x58 ,0x00 ,0x0F ,0x01 ,0x0A ,0x00
//    ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0F ,0x01 ,0x09
//    ,0x00 ,0x0F ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01
//    ,0x0A ,0x00 ,0x0F ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x20 ,0x00 ,0x0F
//    ,0x01 ,0x1F ,0x00 ,0x0F ,0x01 ,0x20 ,0x00 ,0x0F ,0x01 ,0x1F ,0x00
//    ,0x0E ,0x01 ,0x20 ,0x00 ,0x0F ,0x01 ,0x20 ,0x00 ,0x0E ,0x01 ,0x20
//    ,0x00 ,0x0E ,0x01 ,0x20 ,0x00 ,0x0F ,0x01 ,0x20 ,0x00 ,0x0E ,0x01
//    ,0x0A ,0x00 ,0x0E ,0x01 ,0x20 ,0x00 ,0x0F ,0x01 ,0x0A ,0x00 ,0x0E
//    ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0F ,0x01 ,0x20 ,0x00
//    ,0x0E ,0x01 ,0x0A ,0x00 ,0x0F ,0x01 ,0x09 ,0x00 ,0x0F ,0x01 ,0x1F
//    ,0x00 ,0x0F ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x20 ,0x00 ,0x0F ,0x01
//    ,0x1F ,0x00 ,0x0F ,0x01 ,0x20 ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0F
//    ,0x01 ,0x1F ,0x00 ,0x0F ,0x01 ,0xFF ,0x01 ,0xFF ,0x01 ,0xFF ,0x01
//    ,0x1A ,0x00 ,0xCF ,0x01 ,0x2C ,0x00 ,0x0E ,0x01 ,0x14
// };
ir_universal_data_type ir_data_buf[286] =  
{
 0x00 ,0x02 ,0x00 ,0xB3 ,0x01 ,0x59 ,0x00 ,0x0E ,0x01 ,0x20 ,0x00
,0x0E ,0x01 ,0x20 ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x21
,0x00 ,0x0D ,0x01 ,0x0B ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01
,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0D ,0x01 ,0x0B ,0x00 ,0x0E
,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x21 ,0x00
,0x0D ,0x01 ,0x0B ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A
,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01
,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0B ,0x00 ,0x0E
,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x20 ,0x00 ,0x0E ,0x01 ,0x0A ,0x00
,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A
,0x00 ,0x0E ,0x01 ,0x0B ,0x00 ,0x0D ,0x01 ,0x0B ,0x00 ,0x0E ,0x01
,0x20 ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x20 ,0x00 ,0x0E
,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x20 ,0x00
,0x0E ,0x01 ,0x0B ,0x00 ,0x0E ,0x01 ,0xFF ,0x01 ,0x8F ,0x00 ,0x0E
,0x01 ,0x0B ,0x00 ,0x0D ,0x01 ,0x0B ,0x00 ,0x0E ,0x01 ,0x0A ,0x00
,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A
,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01
,0x0B ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E
,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00
,0x0E ,0x01 ,0x20 ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0B
,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01
,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E
,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0B ,0x00 ,0x0E ,0x01 ,0x0A ,0x00
,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A
,0x00 ,0x0E ,0x01 ,0x20 ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01
,0x0A ,0x00 ,0x0E ,0x01 ,0x0B ,0x00 ,0x0E ,0x01 ,0xFF ,0x01 ,0xFF
,0x01 ,0xFF ,0x01 ,0x22 ,0x00 ,0xB4 ,0x01 ,0x59 ,0x00 ,0x0E ,0x01
,0x20 ,0x00 ,0x0E ,0x01 ,0x20 ,0x00 ,0x0E ,0x01 ,0x0B ,0x00 ,0x0E
,0x01 ,0x20 ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00
,0x0D ,0x01 ,0x0B ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A
,0x00 ,0x0E ,0x01 ,0x0B ,0x00 ,0x0D ,0x01 ,0x0B ,0x00 ,0x0E ,0x01
,0x20 ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E
,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00
,0x0E ,0x01 ,0x0B ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A
,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x20 ,0x00 ,0x0E ,0x01
,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0B ,0x00 ,0x0D
,0x01 ,0x0B ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00
,0x0E ,0x01 ,0x20 ,0x00 ,0x0E ,0x01 ,0x20 ,0x00 ,0x0E ,0x01 ,0x20
,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0B ,0x00 ,0x0E ,0x01
,0x20 ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0xFF ,0x01 ,0x90
,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01
,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E
,0x01 ,0x0B ,0x00 ,0x0D ,0x01 ,0x0B ,0x00 ,0x0E ,0x01 ,0x0A ,0x00
,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A
,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01
,0x0B ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0D ,0x01 ,0x0B ,0x00 ,0x0E
,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00
,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x0B
,0x00 ,0x0D ,0x01 ,0x0B ,0x00 ,0x0D ,0x01 ,0x0B ,0x00 ,0x0D ,0x01
,0x0B ,0x00 ,0x0D ,0x01 ,0x0B ,0x00 ,0x0E ,0x01 ,0x0A ,0x00 ,0x0E
,0x01 ,0x0A ,0x00 ,0x0E ,0x01 ,0x20 ,0x00 ,0x0E ,0x01 ,0x0B ,0x00
,0x0D ,0x01 ,0x21 ,0x00 ,0x0E ,0x01 ,0x20 ,0x00 ,0x0D ,0x01 ,0x14
}; 
#endif

// PA.4 used for IR receive, F_INTERRUPTS is 15000

uint32_t count  = 0;

void timer16_init (void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_ClockDivision                 = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode                   = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Prescaler                     = 7;
    TIM_TimeBaseStructure.TIM_Period                        = (uint32_t)((F_CPU / F_INTERRUPTS) / 8) - 1;
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
  //count++;
  // call other timer interrupt routines...
}

void timer17_init (void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_ClockDivision                 = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode                   = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Prescaler                     = 7;
    TIM_TimeBaseStructure.TIM_Period                        = (uint32_t)((F_CPU / F_INTERRUPTS) / 8) - 1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter             = 0;
    TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);

    NVIC_InitStructure.NVIC_IRQChannel                      = TIM17_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPriority              = 0;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM17, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM17, ENABLE);
}

void TIM17_IRQHandler(void)                                                       // Timer17 Interrupt Handler
{
  TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
  (void) irsnd_ISR();                                                             // call irsnd ISR
  count++;
  // call other timer interrupt routines...
}


IRMP_DATA irsnd_data;

void send_nec_data(uint16_t command)
{
    irsnd_data.protocol = IRMP_NEC_PROTOCOL;                             // use NEC protocol
    irsnd_data.address  = 0x00ff;                                        // set address to 0x00FF
    irsnd_data.command  = command;
    irsnd_data.flags    = 0;                                             // don't repeat frame
    irsnd_send_data (&irsnd_data, TRUE);  
    delay_ms(10);
}

int main(void)
{
    IRMP_DATA irmp_data;
    IRMP_DATA irmp_data_old;
    uint8_t key_value;
    uint8_t pre_key_value = HIGH;
    bool key_status = false;
    systick_init();
    uart_ringbuf_init();
    drv_uart1_init(115200);
#if (ONLY_SEND_CMD != 1)
    irmp_init();
    timer16_init();
#endif
    irsnd_init();
    timer17_init();
    drv_gpio_pin_mode(PB_8, GPIO_MODE_INPUT_NONE);

#if (ONLY_SEND_CMD != 1)
    log_uart_printf(UART1, "Test IRMP and IRSND!\r\n");
#else
    log_uart_printf(UART1, "Test IRSND!\r\n");
#endif

#if (MATATALAB_FEATURE == 1)
    set_ir_learn_state(true);
#endif

    unsigned long current_time = millis();
    while(1)
    {
#if (ONLY_SEND_CMD != 1)
        if (irmp_get_data (&irmp_data))
        {
//#if (MATATALAB_FEATURE != 1)
            log_uart_printf(UART1, "IRMP_DATA>>  protocol:0x%x, address:0x%x, command:0x%x, flags:0x%x\r\n", irmp_data.protocol, irmp_data.address, irmp_data.command, irmp_data.flags);
//#endif
            if((irmp_data.protocol != irmp_data_old.protocol) ||
               (irmp_data.address != irmp_data_old.address) ||
               (irmp_data.command != irmp_data_old.command) ||
               (irmp_data.flags != irmp_data_old.flags))
            {
                memcpy(&irmp_data_old, &irmp_data, sizeof(irmp_data));
            }
        }
#endif
        key_value = drv_gpio_digital_read(PB_8);

        if(key_value != pre_key_value)
        {
            pre_key_value = key_value;
            if(key_value == HIGH)
            {
                if(key_value != pre_key_value)
                {
                    pre_key_value = key_value;
                }
                if(key_status == false)
                {
                    key_status = true;
                    log_uart_printf(UART1, "Test IRSND 0x01!\r\n");
#if (MATATALAB_FEATURE == 1)
                    irsnd_send_universal_data(ir_data_buf, 286, true);
#else
                    send_nec_data(0x0001);
#endif
                }
                else
                {
                    key_status = false;
                    log_uart_printf(UART1, "Test IRSND 0x02!\r\n");
#if (MATATALAB_FEATURE == 1)
                    irsnd_send_universal_data(ir_data_buf, 286, true);
#else
                    send_nec_data(0x0002);
#endif
                }
            }
        }

        if(millis() - current_time > 999)
        {
            // irsnd_data.protocol = IRMP_NEC_PROTOCOL;                             // use NEC protocol
            // irsnd_data.address  = 0x1234;                                        // set address to 0x00FF
            // irsnd_data.command  = 0x0001;                                        // set command to 0x0001
            // irsnd_data.flags    = 0;                                             // don't repeat frame
 
            // irsnd_send_data (&irsnd_data, TRUE);                                 // send frame, wait for completion
            // log_uart_printf(UART1, "count: %d\r\n", count);
            current_time = millis();
        }
    }
    return 0;
}
