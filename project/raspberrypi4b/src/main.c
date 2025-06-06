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
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-04-08
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/04/08  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ssd1351_basic.h"
#include "driver_ssd1351_advance.h"
#include "driver_ssd1351_display_test.h"
#include "shell.h"
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                              /**< uart buffer */
volatile uint16_t g_len;                         /**< uart buffer length */
static int gs_listen_fd;                         /**< network handle */
static int gs_conn_fd;                           /**< network handle */
static struct sockaddr_in gs_server_addr;        /**< server address */

/**
 * @brief     ssd1351 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t ssd1351(uint8_t argc, char** argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"color", required_argument, NULL, 1},
        {"mode", required_argument, NULL, 2},
        {"length", required_argument, NULL, 3},
        {"scroll", required_argument, NULL, 4},
        {"start", required_argument, NULL, 5},
        {"str", required_argument, NULL, 6},
        {"x0", required_argument, NULL, 7},
        {"x1", required_argument, NULL, 8},
        {"x2", required_argument, NULL, 9},
        {"y0", required_argument, NULL, 10},
        {"y1", required_argument, NULL, 11},
        {"y2", required_argument, NULL, 12},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    ssd1351_scroll_mode_t mode = SSD1351_SCROLL_MODE_NORMAL;
    uint32_t color = 65535;
    uint8_t length = 128;
    uint8_t start = 0;
    uint8_t scroll = 1;
    uint8_t x0 = 0;
    uint8_t x1 = 0;
    uint8_t x2 = 0;
    uint8_t y0 = 0;
    uint8_t y1 = 0;
    uint8_t y2 = 0;
    uint8_t x0_flag = 0;
    uint8_t x1_flag = 0;
    uint8_t x2_flag = 0;
    uint8_t y0_flag = 0;
    uint8_t y1_flag = 0;
    uint8_t y2_flag = 0;
    char str[49] = "libdriver";
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* color */
            case 1 :
            {
                /* set the color */
                color = atol(optarg);
                
                break;
            }
            
            /* mode */
            case 2 :
            {
                /* set the mode */
                if (strcmp("TEST", optarg) == 0)
                {
                    mode = SSD1351_SCROLL_MODE_TEST;
                }
                else if (strcmp("NORMAL", optarg) == 0)
                {
                    mode = SSD1351_SCROLL_MODE_NORMAL;
                }
                else if (strcmp("SLOW", optarg) == 0)
                {
                    mode = SSD1351_SCROLL_MODE_SLOW;
                }
                else if (strcmp("SLOWEST", optarg) == 0)
                {
                    mode = SSD1351_SCROLL_MODE_SLOWEST;
                }
                else
                {
                    return 5;
                }
                
                break;
            } 
            
            /* length */
            case 3 :
            {
                /* set the length */
                length = atol(optarg);
                
                break;
            }
            
            /* scroll */
            case 4 :
            {
                /* set the scroll */
                scroll = atol(optarg);
                
                break;
            }
            
            /* start */
            case 5 :
            {
                /* set the start */
                start = atol(optarg);
                
                break;
            }
            
            /* str */
            case 6 :
            {
                /* set the str */
                memset(str, 0, sizeof(char) * 49);
                strncpy(str, optarg, 48);
                
                break;
            }
            
            /* x0 */
            case 7 :
            {
                /* convert */
                x0 = atol(optarg);
                x0_flag = 1;
                
                break;
            }
            
            /* x1 */
            case 8 :
            {
                /* convert */
                x1 = atol(optarg);
                x1_flag = 1;
                
                break;
            }
            
            /* x2 */
            case 9 :
            {
                /* convert */
                x2 = atol(optarg);
                x2_flag = 1;
                
                break;
            }
            /* y0 */
            case 10 :
            {
                /* convert */
                y0 = atol(optarg);
                y0_flag = 1;
                
                break;
            }
            
            /* y1 */
            case 11 :
            {
                /* convert */
                y1 = atol(optarg);
                y1_flag = 1;
                
                break;
            }
            
            /* y2 */
            case 12 :
            {
                /* convert */
                y2 = atol(optarg);
                y2_flag = 1;
                
                break;
            }
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);
    
    /* run the function */
    if (strcmp("t_display", type) == 0)
    {
        /* run display test */
        if (ssd1351_display_test() != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("e_basic-str", type) == 0)
    {
        uint8_t res;
        
        /* basic clear */
        res = ssd1351_basic_clear();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: clear screen failed.\n");
            (void)ssd1351_basic_deinit();
            
            return 1;
        }
        
        /* show the string */
        res = ssd1351_basic_string(0, 0, str, (uint16_t)strlen(str), color, SSD1351_FONT_16);
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: show string failed.\n");
            (void)ssd1351_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: %s.\n", str);
        
        return 0;
    }
    else if (strcmp("e_basic-init", type) == 0)
    {
        uint8_t res;
        
        /* basic init */
        res = ssd1351_basic_init();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: init failed.\n");
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: init success.\n");
        
        return 0;
    }
    else if (strcmp("e_basic-display-off", type) == 0)
    {
        uint8_t res;
        
        /* basic display off */
        res = ssd1351_basic_display_off();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: display off failed.\n");
            (void)ssd1351_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: display off.\n");
        
        return 0;
    }
    else if (strcmp("e_basic-display-on", type) == 0)
    {
        uint8_t res;
        
        /* basic display on */
        res = ssd1351_basic_display_on();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: display on failed.\n");
            (void)ssd1351_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: display on.\n");
        
        return 0;
    }
    else if (strcmp("e_basic-clear", type) == 0)
    {
        uint8_t res;
        
        /* basic clear */
        res = ssd1351_basic_clear();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: clear screen failed.\n");
            (void)ssd1351_basic_deinit();
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: clear screen.\n");
        
        return 0;
    }
    else if (strcmp("e_basic-deinit", type) == 0)
    {
        uint8_t res;
        
        /* basic deinit */
        res = ssd1351_basic_deinit();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: deinit failed.\n");
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: deinit ssd1351.\n");
        
        return 0;
    }
    else if (strcmp("e_basic-point", type) == 0)
    {
        uint8_t res;
        
        /* check the flag */
        if ((x0_flag != 1) || (y0_flag != 1))
        {
            return 5;
        }

        /* write point */
        res = ssd1351_basic_write_point(x0, y0, color);
        if (res != 0)
        {
            (void)ssd1351_basic_deinit();

            return 1;
        }

        /* output */
        ssd1351_interface_debug_print("ssd1351: write point %d %d %d.\n", x0, y0, color);
        
        return 0;
    }
    else if (strcmp("e_basic-rect", type) == 0)
    {
        uint8_t res;
        
        /* check the param */
        if ((x1_flag != 1) || (y1_flag != 1) || (x2_flag != 1) || (y2_flag != 1))
        {
            return 5;
        }

        /* basic rect */
        res = ssd1351_basic_rect(x1, y1, x2, y2, color);
        if (res != 0)
        {
            (void)ssd1351_basic_deinit();

            return 1;
        }

        /* output */
        ssd1351_interface_debug_print("ssd1351: draw rect %d %d %d %d.\n", x1, y1, x2, y2);

        return 0;
    }
    else if (strcmp("e_advance-init", type) == 0)
    {
        uint8_t res;
        
        /* advance init */
        res = ssd1351_advance_init();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: init failed.\n");
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: init success.\n");
        
        return 0;
    }
    else if (strcmp("e_advance-str", type) == 0)
    {
        uint8_t res;
        
        /* advance clear */
        res = ssd1351_advance_clear();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: clear screen failed.\n");
            (void)ssd1351_advance_deinit();
            
            return 1;
        }
        
        /* show the string */
        res = ssd1351_advance_string(0, 0, str, (uint16_t)strlen(str), color, SSD1351_FONT_16);
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: show string failed.\n");
            (void)ssd1351_advance_deinit();
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: %s.\n", str);
        
        return 0;
    }
    else if (strcmp("e_advance-display-off", type) == 0)
    {
        uint8_t res;
        
        /* advance display off */
        res = ssd1351_advance_display_off();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: display off failed.\n");
            (void)ssd1351_advance_deinit();
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: display off.\n");
        
        return 0;
    }
    else if (strcmp("e_advance-display-on", type) == 0)
    {
        uint8_t res;
        
        /* advance display on */
        res = ssd1351_advance_display_on();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: display on failed.\n");
            (void)ssd1351_advance_deinit();
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: display on.\n");
        
        return 0;
    }
    else if (strcmp("e_advance-clear", type) == 0)
    {
        uint8_t res;
        
        /* advance clear */
        res = ssd1351_advance_clear();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: clear screen failed.\n");
            (void)ssd1351_advance_deinit();
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: clear screen.\n");
        
        return 0;
    }
    else if (strcmp("e_advance-point", type) == 0)
    {
        uint8_t res;
        
        /* check the flag */
        if ((x0_flag != 1) || (y0_flag != 1))
        {
            return 5;
        }

        /* advance point */
        res = ssd1351_advance_write_point(x0, y0, color);
        if (res != 0)
        {
            (void)ssd1351_advance_deinit();

            return 1;
        }

        /* output */
        ssd1351_interface_debug_print("ssd1351: write point %d %d %d.\n", x0, y0, color);

        return 0;
    }
    else if (strcmp("e_advance-rect", type) == 0)
    {
        uint8_t res;

        /* check the param */
        if ((x1_flag != 1) || (y1_flag != 1) || (x2_flag != 1) || (y2_flag != 1))
        {
            return 5;
        }
        
        /* advance rect */
        res = ssd1351_advance_rect(x1, y1, x2, y2, color);
        if (res != 0)
        {
            (void)ssd1351_advance_deinit();

            return 1;
        }

        /* output */
        ssd1351_interface_debug_print("ssd1351: draw rect %d %d %d %d.\n", x1, y1, x2, y2);

        return 0;
    }
    else if (strcmp("e_advance-deinit", type) == 0)
    {
        uint8_t res;
        
        /* advance deinit */
        res = ssd1351_advance_deinit();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: deinit failed.\n");
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: deinit ssd1351.\n");
        
        return 0;
    }
    else if (strcmp("e_advance-disable-scroll", type) == 0)
    {
        uint8_t res;
        
        /* advance disable scroll */
        res = ssd1351_advance_disable_scroll();
        if (res != 0)
        {
            ssd1351_interface_debug_print("ssd1351: disable scroll failed.\n");
            
            return 1;
        }
        
        /* output */
        ssd1351_interface_debug_print("ssd1351: disable scroll.\n");
        
        return 0;
    }
    else if (strcmp("e_advance-scroll", type) == 0)
    {
        uint8_t res;
        
        /* set the scroll param */
        res = ssd1351_advance_set_scroll(scroll, start, length, mode);
        if (res != 0)
        {
            (void)ssd1351_advance_deinit();

            return 1;
        }

        /* enable scroll */
        res = ssd1351_advance_enable_scroll();
        if (res != 0)
        {
            (void)ssd1351_advance_deinit();

            return 1;
        }

        /* output */
        ssd1351_interface_debug_print("ssd1351: set scroll %d %d %d.\n", scroll, start, length);

        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ssd1351_interface_debug_print("Usage:\n");
        ssd1351_interface_debug_print("  ssd1351 (-i | --information)\n");
        ssd1351_interface_debug_print("  ssd1351 (-h | --help)\n");
        ssd1351_interface_debug_print("  ssd1351 (-p | --port)\n");
        ssd1351_interface_debug_print("  ssd1351 (-t display | --test=display)\n");
        ssd1351_interface_debug_print("  ssd1351 (-e basic-init | --example=basic-init)\n");
        ssd1351_interface_debug_print("  ssd1351 (-e basic-deinit | --example=basic-deinit)\n");
        ssd1351_interface_debug_print("  ssd1351 (-e basic-str | --example=basic-str) [--str=<string>]\n");
        ssd1351_interface_debug_print("  ssd1351 (-e basic-display-on | --example=basic-display-on)\n");
        ssd1351_interface_debug_print("  ssd1351 (-e basic-display-off | --example=basic-display-off)\n");
        ssd1351_interface_debug_print("  ssd1351 (-e basic-clear | --example=basic-clear)\n");
        ssd1351_interface_debug_print("  ssd1351 (-e basic-point | --example=basic-point) --x0=<x0> --y0=<y0> [--color=<rgb565>]\n");
        ssd1351_interface_debug_print("  ssd1351 (-e basic-rect | --example=basic-rect) --x1=<x1> --y1=<y1> --x2=<x2> --y2=<y2> [--color=<rgb565>]\n");
        ssd1351_interface_debug_print("  ssd1351 (-e advance-init | --example=advance-init)\n");
        ssd1351_interface_debug_print("  ssd1351 (-e advance-deinit | --example=advance-deinit)\n");
        ssd1351_interface_debug_print("  ssd1351 (-e advance-str | --example=advance-str) [--str=<string>]\n");
        ssd1351_interface_debug_print("  ssd1351 (-e advance-display-on | --example=advance-display-on)\n");
        ssd1351_interface_debug_print("  ssd1351 (-e advance-display-off | --example=advance-display-off)\n");
        ssd1351_interface_debug_print("  ssd1351 (-e advance-clear | --example=advance-clear)\n");
        ssd1351_interface_debug_print("  ssd1351 (-e advance-point | --example=advance-point) --x0=<x0> --y0=<y0> [--color=<rgb565>]\n");
        ssd1351_interface_debug_print("  ssd1351 (-e advance-rect | --example=advance-rect) --x1=<x1> --y1=<y1> --x2=<x2> --y2=<y2> [--color=<rgb565>]\n");
        ssd1351_interface_debug_print("  ssd1351 (-e advance-disable-scroll | --example=advance-disable-scroll)\n");
        ssd1351_interface_debug_print("  ssd1351 (-e advance-scroll | --example=advance-scroll) [--scroll=<num>] [--start=<srow>] [--length=<len>]\n");
        ssd1351_interface_debug_print("          [--mode=<TEST | NORMAL | SLOW | SLOWEST>]\n");
        ssd1351_interface_debug_print("\n");  
        ssd1351_interface_debug_print("Options:\n");
        ssd1351_interface_debug_print("     --color=<rgb565>     Set the chip color.([default: 65535])\n");
        ssd1351_interface_debug_print("  -e <basic-init | basic-deinit | basic-str | basic-display-on | basic-display-off | basic-clear\n"); 
        ssd1351_interface_debug_print("     | basic-point | basic-rect>, --example=<advance-init | advance-deinit | advance-str | advance-display-on\n");
        ssd1351_interface_debug_print("     | advance-display-off | advance-clear | advance-point | advance-rect | advance-disable-scroll | advance-scroll>\n");
        ssd1351_interface_debug_print("                          Run the driver example.\n");
        ssd1351_interface_debug_print("  -h, --help              Show the help.\n");
        ssd1351_interface_debug_print("  -i, --information       Show the chip information.\n");
        ssd1351_interface_debug_print("      --mode=<TEST | NORMAL | SLOW | SLOWEST>\n");
        ssd1351_interface_debug_print("                          Set the scrolling mode.([default: NORMAL])\n");
        ssd1351_interface_debug_print("      --length=<len>      Set the scrolling row length and len + srow <= 128.([default: 128])\n");
        ssd1351_interface_debug_print("  -p, --port              Display the pin connections of the current board.\n");
        ssd1351_interface_debug_print("      --scroll            Set the scrolling number, if > 0 left scroll and < 0 right scroll.([default: 1])\n");
        ssd1351_interface_debug_print("      --start=<srow>      Set the scrolling start row and len + srow <= 128.([default: 0])\n");
        ssd1351_interface_debug_print("      --str=<string>      Set the display string.([default: libdriver])\n");
        ssd1351_interface_debug_print("  -t <display>, --test=<display>\n");
        ssd1351_interface_debug_print("                          Run the driver test.\n");
        ssd1351_interface_debug_print("      --x0=<x0>           Set the x0 and it is the x of the point.\n");
        ssd1351_interface_debug_print("      --x1=<x1>           Set the x1 and it is the top left x of the rect.\n");
        ssd1351_interface_debug_print("      --x2=<x2>           Set the x2 and it is the bottom right x of the rect.\n");
        ssd1351_interface_debug_print("      --y0=<y0>           Set the y0 and it is the y of the point.\n");
        ssd1351_interface_debug_print("      --y1=<y1>           Set the y1 and it is the top left y of the rect.\n");
        ssd1351_interface_debug_print("      --y2=<y2>           Set the y2 and it is the bottom right y of the rect.\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
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
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ssd1351_interface_debug_print("ssd1351: SCK connected to GPIO11(BCM).\n");
        ssd1351_interface_debug_print("ssd1351: MISO connected to GPIO9(BCM).\n");
        ssd1351_interface_debug_print("ssd1351: MOSI connected to GPIO10(BCM).\n");
        ssd1351_interface_debug_print("ssd1351: CS connected to GPIO8(BCM).\n");
        ssd1351_interface_debug_print("ssd1351: cmd data gpio GPIO connected to GPIO17(BCM).\n");
        ssd1351_interface_debug_print("ssd1351: reset GPIO connected to GPIO27(BCM).\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief  socket init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
static uint8_t a_socket_init(void)
{
    int optval;
    
    /* creat a socket */
    gs_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (gs_listen_fd < 0) 
    {
        ssd1351_interface_debug_print("ssd1351: create socket failed.\n");
        
        return 1;
    }
    
    /* set the server port */
    memset(&gs_server_addr, 0, sizeof(gs_server_addr));
    gs_server_addr.sin_family = AF_INET;
    gs_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    gs_server_addr.sin_port = htons(6666);
    
    /* enable same port binding */
    optval = 1;
    if (setsockopt(gs_listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval)) < 0)
    {
        ssd1351_interface_debug_print("ssd1351: create socket failed.\n");
        (void)close(gs_listen_fd);
        
        return 1;
    }
    
    /* bind the port */
    if (bind(gs_listen_fd, (struct sockaddr*)&gs_server_addr, sizeof(gs_server_addr)) < 0) 
    {
        ssd1351_interface_debug_print("ssd1351: bind failed.\n");
        (void)close(gs_listen_fd);
        
        return 1;
    }
    
    /* listen the port */
    if (listen(gs_listen_fd, 10) < 0) 
    {
        ssd1351_interface_debug_print("ssd1351: listen failed.\n");
        (void)close(gs_listen_fd);
        
        return 1;
    } 
    
    return 0;
}

/**
 * @brief     socket read
 * @param[in] *buf pointer to a buffer address
 * @param[in] len buffer length
 * @return    status code
 *             - 0 success
 *             - 1 read failed
 * @note      none
 */
static uint16_t a_socket_read(uint8_t *buf, uint16_t len)
{
    int n;
    
    /* wait data */
    gs_conn_fd = accept(gs_listen_fd, (struct sockaddr *)NULL, NULL);
    if (gs_conn_fd < 0) 
    {
        ssd1351_interface_debug_print("ssd1351: accept failed.\n");
        (void)close(gs_conn_fd);

        return 1;
    }
    
    /* read data */
    n = recv(gs_conn_fd, buf, len, 0);
    
    /* close the socket */
    (void)close(gs_conn_fd);
    
    return n;
}

/**
 * @brief     signal handler
 * @param[in] signum signal number
 * @note      none
 */
static void a_sig_handler(int signum)
{
    if (SIGINT == signum)
    {
        ssd1351_interface_debug_print("ssd1351: close the server.\n");
        (void)close(gs_listen_fd);
        exit(0);
    }
    
    return;
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* socket init */
    res = a_socket_init();
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: socket init failed.\n");

        return 1;
    }

    /* shell init && register ssd1306 function */
    shell_init();
    shell_register("ssd1351", ssd1351);
    ssd1351_interface_debug_print("ssd1351: welcome to libdriver ssd1351.\n");
    
    /* set the signal */
    signal(SIGINT, a_sig_handler);
    
    while (1)
    {
        /* read uart */
        g_len = a_socket_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                ssd1351_interface_debug_print("ssd1351: run failed.\n");
            }
            else if (res == 2)
            {
                ssd1351_interface_debug_print("ssd1351: unknown command.\n");
            }
            else if (res == 3)
            {
                ssd1351_interface_debug_print("ssd1351: length is too long.\n");
            }
            else if (res == 4)
            {
                ssd1351_interface_debug_print("ssd1351: pretreat failed.\n");
            }
            else if (res == 5)
            {
                ssd1351_interface_debug_print("ssd1351: param is invalid.\n");
            }
            else
            {
                ssd1351_interface_debug_print("ssd1351: unknown status code.\n");
            }
        }
    }
}
