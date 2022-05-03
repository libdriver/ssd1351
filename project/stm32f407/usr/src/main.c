/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main souce file
 * @version   2.0.0
 * @author    Shifeng Li
 * @date      2021-04-08
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/04/08  <td>2.0      <td>Shifeng Li  <td>format the code
 * <tr><td>2020/12/18  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ssd1351_basic.h"
#include "driver_ssd1351_advance.h"
#include "driver_ssd1351_display_test.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "shell.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
uint16_t g_len;            /**< uart buffer length */

/**
 * @brief     ssd1351 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t ssd1351(uint8_t argc, char** argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            ssd1351_info_t info;

            /* print ssd1351 info */
            ssd1351_info(&info);
            ssd1351_interface_debug_print("ssd1351: chip is %s.\n", info.chip_name);
            ssd1351_interface_debug_print("ssd1351: manufacturer is %s.\n", info.manufacturer_name);
            ssd1351_interface_debug_print("ssd1351: interface is %s.\n", info.interface);
            ssd1351_interface_debug_print("ssd1351: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
            ssd1351_interface_debug_print("ssd1351: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            ssd1351_interface_debug_print("ssd1351: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            ssd1351_interface_debug_print("ssd1351: max current is %0.2fmA.\n", info.max_current_ma);
            ssd1351_interface_debug_print("ssd1351: max temperature is %0.1fC.\n", info.temperature_max);
            ssd1351_interface_debug_print("ssd1351: min temperature is %0.1fC.\n", info.temperature_min);

            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            ssd1351_interface_debug_print("ssd1351: SCK connected to GPIOA PIN5.\n");
            ssd1351_interface_debug_print("ssd1351: MISO connected to GPIOA PIN6.\n");
            ssd1351_interface_debug_print("ssd1351: MOSI connected to GPIOA PIN7.\n");
            ssd1351_interface_debug_print("ssd1351: CS connected to GPIOA PIN4.\n");
            ssd1351_interface_debug_print("ssd1351: cmd data gpio GPIO connected to GPIOA PIN8.\n");
            ssd1351_interface_debug_print("ssd1351: reset GPIO connected to GPIOB PIN0.\n");

            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show ssd1351 help */

            help:

            ssd1351_interface_debug_print("ssd1351 -i\n\tshow ssd1351 chip and driver information.\n");
            ssd1351_interface_debug_print("ssd1351 -h\n\tshow ssd1351 help.\n");
            ssd1351_interface_debug_print("ssd1351 -p\n\tshow ssd1351 pin connections of the current board.\n");
            ssd1351_interface_debug_print("ssd1351 -t display\n\trun ssd1351 display test.\n");
            ssd1351_interface_debug_print("ssd1351 -c basic -init\n\trun ssd1351 basic init function.\n");
            ssd1351_interface_debug_print("ssd1351 -c basic -deinit\n\trun ssd1351 basic deinit function.\n");
            ssd1351_interface_debug_print("ssd1351 -c basic -str <string>\n\trun ssd1351 show string function.string means the shown string.\n");
            ssd1351_interface_debug_print("ssd1351 -c basic -displayon\n\trun ssd1351 display on function.\n");
            ssd1351_interface_debug_print("ssd1351 -c basic -displayoff\n\trun ssd1351 display off function.\n");
            ssd1351_interface_debug_print("ssd1351 -c basic -clear\n\trun ssd1351 clear screen function.\n");
            ssd1351_interface_debug_print("ssd1351 -c basic -writepoint <x> <y> <color>\n\trun ssd1351 write pont function."
                                          "x and y mean coordinate in screen.color means the filled color.\n");
            ssd1351_interface_debug_print("ssd1351 -c basic -rect <x1> <y1> <x2> <y2> <color>\n\trun ssd1351 draw rectangle function."
                                          "x1 means x start.y1 means y start.x2 means x end.y2 means y end.color means the filled color.\n");
            ssd1351_interface_debug_print("ssd1351 -c advance -init\n\trun ssd1351 advance init function.\n");
            ssd1351_interface_debug_print("ssd1351 -c advance -deinit\n\trun ssd1351 advance deinit function.\n");
            ssd1351_interface_debug_print("ssd1351 -c advance -str <string>\n\trun ssd1351 show string function."
                                          "string means the shown string.\n");
            ssd1351_interface_debug_print("ssd1351 -c advance -displayon\n\trun ssd1351 display on function.\n");
            ssd1351_interface_debug_print("ssd1351 -c advance -displayoff\n\trun ssd1351 display off function.\n");
            ssd1351_interface_debug_print("ssd1351 -c advance -clear\n\trun ssd1351 clear screen function.\n");
            ssd1351_interface_debug_print("ssd1351 -c advance -writepoint <x> <y> <color>\n\trun ssd1351 write pont function."
                                          "x and y mean coordinate in screen.color means the filled color.\n");
            ssd1351_interface_debug_print("ssd1351 -c advance -rect <x1> <y1> <x2> <y2> <color>\n\trun ssd1351 draw rectangle function."
                                          "x1 means x start.y1 means y start.x2 means x end.y2 means y end.color means the filled color.\n");
            ssd1351_interface_debug_print("ssd1351 -c advance -disable_scroll\n\trun ssd1351 disable scroll function.\n");
            ssd1351_interface_debug_print("ssd1351 -c advance -scroll <scroll> <startrow> <rowlen> (TSET | NORMAL | SLOW | SLOWEST)\n\trun ssd1351 scroll function."
                                          "scroll means the scroll number,if >0 left scroll and <0 right scroll.");
            ssd1351_interface_debug_print("startrow means the start row.rowlen means the row lenght and startrow + rowlen max is 128.\n");

            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
            /* display test */
            if (strcmp("display", argv[2]) == 0)
            {
                /* run reg test */
                if (ssd1351_display_test() != 0)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        if (strcmp("-c", argv[1]) == 0)
        {
            /* basic test */
            if (strcmp("basic", argv[2]) == 0)
            {
                uint8_t res;

                if (strcmp("-displayon", argv[3]) == 0)
                {
                    res = ssd1351_basic_display_on();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: display on failed.\n");
                        (void)ssd1351_basic_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: display on.\n");

                    return 0;
                }
                else if (strcmp("-displayoff", argv[3]) == 0)
                {
                    res = ssd1351_basic_display_off();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: display off failed.\n");
                        (void)ssd1351_basic_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: display off.\n");

                    return 0;
                }
                else if (strcmp("-clear", argv[3]) == 0)
                {
                    res = ssd1351_basic_clear();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: clear screen failed.\n");
                        (void)ssd1351_basic_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: clear screen.\n");

                    return 0;
                }
                else if (strcmp("-init", argv[3]) == 0)
                {
                    res = ssd1351_basic_init();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: init failed.\n");

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: init success.\n");

                    return 0;
                }
                else if (strcmp("-deinit", argv[3]) == 0)
                {
                    res = ssd1351_basic_deinit();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: deinit failed.\n");

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: deinit ssd1351.\n");

                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            /* advance test */
            else if (strcmp("advance", argv[2]) == 0)
            {
                uint8_t res;

                if (strcmp("-displayon", argv[3]) == 0)
                {
                    res = ssd1351_advance_display_on();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: display on failed.\n");
                        (void)ssd1351_advance_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: display on.\n");

                    return 0;
                }
                else if (strcmp("-displayoff", argv[3]) == 0)
                {
                    res = ssd1351_advance_display_off();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: display off failed.\n");
                        (void)ssd1351_advance_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: display off.\n");

                    return 0;
                }
                else if (strcmp("-clear", argv[3]) == 0)
                {
                    res = ssd1351_advance_clear();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: clear screen failed.\n");
                        (void)ssd1351_advance_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: clear screen.\n");

                    return 0;
                }
                else if (strcmp("-init", argv[3]) == 0)
                {
                    res = ssd1351_advance_init();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: init failed.\n");

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: init success.\n");

                    return 0;
                }
                else if (strcmp("-deinit", argv[3]) == 0)
                {
                    res = ssd1351_advance_deinit();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: deinit failed.\n");

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: deinit ssd1351.\n");

                    return 0;
                }
                else if (strcmp("-disable_scroll", argv[3]) == 0)
                {
                    res = ssd1351_advance_disable_scroll();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: disable scroll failed.\n");

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: disable scroll.\n");

                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        /* run functions */
        if (strcmp("-c", argv[1]) == 0)
        {
            /* basic test */
            if (strcmp("basic", argv[2]) == 0)
            {
                uint8_t res;

                if (strcmp("-str", argv[3]) == 0)
                {
                    res = ssd1351_basic_clear();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: clear screen failed.\n");
                        (void)ssd1351_basic_deinit();

                        return 1;
                    }
                    res = ssd1351_basic_string(0, 0, argv[4], (uint16_t)strlen(argv[4]), 0xFFFF, SSD1351_FONT_16);
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: show string failed.\n");
                        (void)ssd1351_basic_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: %s.\n", argv[4]);

                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            /* advance test */
            else if (strcmp("advance", argv[2]) == 0)
            {
                uint8_t res;

                if (strcmp("-str", argv[3]) == 0)
                {
                    res = ssd1351_advance_clear();
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: clear screen failed.\n");
                        (void)ssd1351_advance_deinit();

                        return 1;
                    }
                    res = ssd1351_advance_string(0, 0, argv[4], (uint16_t)strlen(argv[4]), 0xFFFF, SSD1351_FONT_16);
                    if (res != 0)
                    {
                        ssd1351_interface_debug_print("ssd1351: show string failed.\n");
                        (void)ssd1351_advance_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: %s.\n", argv[4]);

                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 7)
    {
        /* run functions */
        if (strcmp("-c", argv[1]) == 0)
        {
            /* basic test */
            if (strcmp("basic", argv[2]) == 0)
            {
                if (strcmp("-writepoint", argv[3]) == 0)
                {
                    uint8_t res;

                    res = ssd1351_basic_write_point((uint8_t)atoi(argv[4]), (uint8_t)atoi(argv[5]), (uint32_t)atoi(argv[6]));
                    if (res != 0)
                    {
                        (void)ssd1351_basic_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: write point %d %d %d.\n", (uint8_t)atoi(argv[4]), (uint8_t)atoi(argv[5]), (uint32_t)atoi(argv[6]));

                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            /* advance test */
            else if (strcmp("advance", argv[2]) == 0)
            {
                if (strcmp("-writepoint", argv[3]) == 0)
                {
                    uint8_t res;

                    res = ssd1351_advance_write_point((uint8_t)atoi(argv[4]), (uint8_t)atoi(argv[5]), (uint32_t)atoi(argv[6]));
                    if (res != 0)
                    {
                        (void)ssd1351_advance_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: write point %d %d %d.\n", (uint8_t)atoi(argv[4]), (uint8_t)atoi(argv[5]), (uint32_t)atoi(argv[6]));

                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 8)
    {
        /* run functions */
        if (strcmp("-c", argv[1]) == 0)
        {
            /* advance test */
            if (strcmp("advance", argv[2]) == 0)
            {
                if (strcmp("-scroll", argv[3]) == 0)
                {
                    uint8_t res;
                    ssd1351_scroll_mode_t mode;

                    if (strcmp("TEST", argv[7]) == 0)
                    {
                        mode = SSD1351_SCROLL_MODE_TEST;
                    }
                    else if (strcmp("NORMAL", argv[7]) == 0)
                    {
                        mode = SSD1351_SCROLL_MODE_NORMAL;
                    }
                    else if (strcmp("SLOW", argv[7]) == 0)
                    {
                        mode = SSD1351_SCROLL_MODE_SLOW;
                    }
                    else if (strcmp("SLOWEST", argv[7]) == 0)
                    {
                        mode = SSD1351_SCROLL_MODE_SLOWEST;
                    }
                    else
                    {
                        ssd1351_interface_debug_print("ssd1351: mode is invalide.");

                        return 5;
                    }
                    res = ssd1351_advance_set_scroll((uint8_t)atoi(argv[4]), (uint8_t)atoi(argv[5]), (uint8_t)atoi(argv[6]), mode);
                    if (res != 0)
                    {
                        (void)ssd1351_advance_deinit();

                        return 1;
                    }
                    res = ssd1351_advance_enable_scroll();
                    if (res != 0)
                    {
                        (void)ssd1351_advance_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: set scroll %d %d %d.\n", (int8_t)atoi(argv[4]), (uint8_t)atoi(argv[5]), (uint8_t)atoi(argv[6]));

                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 9)
    {
        /* run functions */
        if (strcmp("-c", argv[1]) == 0)
        {
            /* basic test */
            if (strcmp("basic", argv[2]) == 0)
            {
                if (strcmp("-rect", argv[3]) == 0)
                {
                    uint8_t res;

                    res = ssd1351_basic_rect((uint8_t)atoi(argv[4]), (uint8_t)atoi(argv[5]), (uint8_t)atoi(argv[6]), (uint8_t)atoi(argv[7]), (uint32_t)atoi(argv[8]));
                    if (res != 0)
                    {
                        (void)ssd1351_basic_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: draw rect %d %d %d %d.\n", (uint8_t)atoi(argv[4]), (uint8_t)atoi(argv[5]), (uint8_t)atoi(argv[6]), (uint16_t)atoi(argv[7]));

                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            /* advance fucntion */
            else if (strcmp("advance", argv[2]) == 0)
            {
                if (strcmp("-rect", argv[3]) == 0)
                {
                    uint8_t res;

                    res = ssd1351_advance_rect((uint8_t)atoi(argv[4]), (uint8_t)atoi(argv[5]), (uint8_t)atoi(argv[6]), (uint8_t)atoi(argv[7]), (uint32_t)atoi(argv[8]));
                    if (res != 0)
                    {
                        (void)ssd1351_advance_deinit();

                        return 1;
                    }
                    ssd1351_interface_debug_print("ssd1351: draw rect %d %d %d %d.\n", (uint8_t)atoi(argv[4]), (uint8_t)atoi(argv[5]), (uint8_t)atoi(argv[6]), (uint16_t)atoi(argv[7]));

                    return 0;
                }
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart1 init */
    uart1_init(115200);
    
    /* shell init && register ssd1351 fuction */
    shell_init();
    shell_register("ssd1351", ssd1351);
    uart1_print("ssd1351: welcome to libdriver ssd1351.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart1_read(g_buf, 256);
        if (g_len)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart1_print("ssd1351: run failed.\n");
            }
            else if (res == 2)
            {
                uart1_print("ssd1351: unknow command.\n");
            }
            else if (res == 3)
            {
                uart1_print("ssd1351: length is too long.\n");
            }
            else if (res == 4)
            {
                uart1_print("ssd1351: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart1_print("ssd1351: param is invalid.\n");
            }
            else
            {
                uart1_print("ssd1351: unknow status code.\n");
            }
            uart1_flush();
        }
        delay_ms(100);
    }
}
