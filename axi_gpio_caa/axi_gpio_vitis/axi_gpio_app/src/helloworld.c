/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include "platform.h"
#include "xgpio.h"
#include "xparameters.h"
#include "xstatus.h"
#include "xil_printf.h"

#define LED_GPIO_DEVICE_ID   XPAR_AXI_GPIO_0_DEVICE_ID
#define RGB_GPIO_DEVICE_ID   XPAR_AXI_GPIO_1_DEVICE_ID
#define SW_GPIO_DEVICE_ID    XPAR_AXI_GPIO_2_DEVICE_ID

#define GPIO_CHANNEL 1
#define LED_DELAY    10000000

void delay_loop(void)
{
    volatile int delay;

    for (delay = 0; delay < LED_DELAY; delay++);
}

int main()
{
    XGpio LedGpio;
    XGpio RgbGpio;
    XGpio SwGpio;

    int Status;
    u32 switches;

    u32 binary_count = 0;
    u32 ring_value = 0x1;
    int previous_mode = 0;

    init_platform();

    Status = XGpio_Initialize(&LedGpio, LED_GPIO_DEVICE_ID);
    if (Status != XST_SUCCESS)
    {
        xil_printf("LED GPIO initialization failed.\r\n");
        return XST_FAILURE;
    }

    Status = XGpio_Initialize(&RgbGpio, RGB_GPIO_DEVICE_ID);
    if (Status != XST_SUCCESS)
    {
        xil_printf("RGB GPIO initialization failed.\r\n");
        return XST_FAILURE;
    }

    Status = XGpio_Initialize(&SwGpio, SW_GPIO_DEVICE_ID);
    if (Status != XST_SUCCESS)
    {
        xil_printf("Switch GPIO initialization failed.\r\n");
        return XST_FAILURE;
    }

    // Configure LEDs and RGB LED as outputs
    XGpio_SetDataDirection(&LedGpio, GPIO_CHANNEL, 0x0);
    XGpio_SetDataDirection(&RgbGpio, GPIO_CHANNEL, 0x0);

    // Configure switches as inputs
    XGpio_SetDataDirection(&SwGpio, GPIO_CHANNEL, 0xF);

    while (1)
    {
        switches = XGpio_DiscreteRead(&SwGpio, GPIO_CHANNEL) & 0xF;

        // SW0 only: LED0 and red RGB LED
        if (switches == 0x1)
        {
            XGpio_DiscreteWrite(&LedGpio, GPIO_CHANNEL, 0x1);
            XGpio_DiscreteWrite(&RgbGpio, GPIO_CHANNEL, 0x4);
            previous_mode = 0;
        }

        // SW1 only: LED1 and green RGB LED
        else if (switches == 0x2)
        {
            XGpio_DiscreteWrite(&LedGpio, GPIO_CHANNEL, 0x2);
            XGpio_DiscreteWrite(&RgbGpio, GPIO_CHANNEL, 0x2);
            previous_mode = 0;
        }

        // SW2 only: LED2 and blue RGB LED
        else if (switches == 0x4)
        {
            XGpio_DiscreteWrite(&LedGpio, GPIO_CHANNEL, 0x4);
            XGpio_DiscreteWrite(&RgbGpio, GPIO_CHANNEL, 0x1);
            previous_mode = 0;
        }

        // SW3 only: LED3 and white RGB LED
        else if (switches == 0x8)
        {
            XGpio_DiscreteWrite(&LedGpio, GPIO_CHANNEL, 0x8);
            XGpio_DiscreteWrite(&RgbGpio, GPIO_CHANNEL, 0x7);
            previous_mode = 0;
        }

        // SW0 and SW1: binary counter
        else if (switches == 0x3)
        {
            if (previous_mode != 1)
            {
                binary_count = 0;
            }

            XGpio_DiscreteWrite(&LedGpio, GPIO_CHANNEL, binary_count);
            XGpio_DiscreteWrite(&RgbGpio, GPIO_CHANNEL, 0x0);

            delay_loop();

            binary_count = (binary_count + 1) & 0xF;
            previous_mode = 1;
        }

        // SW2 and SW3: ring counter
        else if (switches == 0xC)
        {
            if (previous_mode != 2)
            {
                ring_value = 0x1;
            }

            XGpio_DiscreteWrite(&LedGpio, GPIO_CHANNEL, ring_value);
            XGpio_DiscreteWrite(&RgbGpio, GPIO_CHANNEL, 0x0);

            delay_loop();

            ring_value = ring_value << 1;

            if (ring_value > 0x8)
            {
                ring_value = 0x1;
            }

            previous_mode = 2;
        }

        // All other switch combinations: turn everything off
        else
        {
            XGpio_DiscreteWrite(&LedGpio, GPIO_CHANNEL, 0x0);
            XGpio_DiscreteWrite(&RgbGpio, GPIO_CHANNEL, 0x0);
            previous_mode = 0;
        }
    }

    cleanup_platform();

    return 0;
}
