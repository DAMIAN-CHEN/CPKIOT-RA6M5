#include "ws2812.h"

#define WS2812_LowLevel    0xC0     // 0
#define WS2812_HighLevel   0xF0     // 1

 
enum color
{
	RED_H=0,
	GREEN_H=1,
	BLUE_H=2,
	RED_L=3,
	GREEN_L=4,
	BLUE_L=5,	
};

void WS2812_Ctrl(uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t txbuf[24];
    uint8_t res = 0;
    for (int i = 0; i < 8; i++)
    {
        txbuf[7-i]  = (((g>>i)&0x01) ? WS2812_HighLevel : WS2812_LowLevel)>>1;
        txbuf[15-i] = (((r>>i)&0x01) ? WS2812_HighLevel : WS2812_LowLevel)>>1;
        txbuf[23-i] = (((b>>i)&0x01) ? WS2812_HighLevel : WS2812_LowLevel)>>1;
    }

    R_SPI_Write(&g_spi1_ctrl,txbuf,24,SPI_BIT_WIDTH_8_BITS);
    for (int i = 0; i < 100; i++)
    {
        R_SPI_Write(&g_spi1_ctrl,&res,1,SPI_BIT_WIDTH_8_BITS);
    }
}

void ws_2812_init(void)
{
    R_SPI_Open(&g_spi1_ctrl,&g_spi1_cfg);//打开 SPI 模块
}

void spi1_callback(spi_callback_args_t * p_args)
{
    if(p_args->event == SPI_EVENT_TRANSFER_COMPLETE)
    {

    }
}
