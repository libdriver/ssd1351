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
 * @file      driver_ssd1351_display_test.c
 * @brief     driver ssd1351 display test source file
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

#include "driver_ssd1351_display_test.h"
#include "driver_ssd1351_display_image.h"
#include <stdlib.h>

static ssd1351_handle_t gs_handle;        /**< ssd1351 handle */
static const uint8_t gsc_gamma[64] = { 0x02, 0x03, 0x04, 0x05,
                                       0x06, 0x07, 0x08, 0x09,
                                       0x0A, 0x0B, 0x0C, 0x0D,
                                       0x0E, 0x0F, 0x10, 0x11,
                                       0x12, 0x13, 0x15, 0x17,
                                       0x19, 0x1B, 0x1D, 0x1F,
                                       0x21, 0x23, 0x25, 0x27,
                                       0x2A, 0x2D, 0x30, 0x33,
                                       0x36, 0x39, 0x3C, 0x3F,
                                       0x42, 0x45, 0x48, 0x4C,
                                       0x50, 0x54, 0x58, 0x5C,
                                       0x60, 0x64, 0x68, 0x6C,
                                       0x70, 0x74, 0x78, 0x7D,
                                       0x82, 0x87, 0x8C, 0x91,
                                       0x96, 0x9B, 0xA0, 0xA5,
                                       0xAA, 0xAF, 0xB4 };

/**
 * @brief  display test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t ssd1351_display_test(void)
{
    volatile uint8_t res;
    volatile uint8_t i ,j;
    ssd1351_info_t info;
    const char test_str1[] ="libdriver";
    const char test_str2[] ="ssd1306";
    const char test_str3[] ="ABCabc";
    const char test_str4[] ="123?!#$%";
    
    /* link functions */
    DRIVER_SSD1351_LINK_INIT(&gs_handle, ssd1351_handle_t);
    DRIVER_SSD1351_LINK_SPI_INIT(&gs_handle, ssd1351_interface_spi_init);
    DRIVER_SSD1351_LINK_SPI_DEINIT(&gs_handle, ssd1351_interface_spi_deinit);
    DRIVER_SSD1351_LINK_SPI_WRITE_COMMAND(&gs_handle, ssd1351_interface_spi_write_cmd);
    DRIVER_SSD1351_LINK_COMMAND_DATA_GPIO_INIT(&gs_handle, ssd1351_interface_cmd_data_gpio_init);
    DRIVER_SSD1351_LINK_COMMAND_DATA_GPIO_DEINIT(&gs_handle, ssd1351_interface_cmd_data_gpio_deinit);
    DRIVER_SSD1351_LINK_COMMAND_DATA_GPIO_WRITE(&gs_handle, ssd1351_interface_cmd_data_gpio_write);
    DRIVER_SSD1351_LINK_RESET_GPIO_INIT(&gs_handle, ssd1351_interface_reset_gpio_init);
    DRIVER_SSD1351_LINK_RESET_GPIO_DEINIT(&gs_handle, ssd1351_interface_reset_gpio_deinit);
    DRIVER_SSD1351_LINK_RESET_GPIO_WRITE(&gs_handle, ssd1351_interface_reset_gpio_write);
    DRIVER_SSD1351_LINK_DELAY_MS(&gs_handle, ssd1351_interface_delay_ms);
    DRIVER_SSD1351_LINK_DEBUG_PRINT(&gs_handle, ssd1351_interface_debug_print);
    
    /* ssd1351 info */
    res = ssd1351_info(&info);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        ssd1351_interface_debug_print("ssd1351: chip is %s.\n", info.chip_name);
        ssd1351_interface_debug_print("ssd1351: manufacturer is %s.\n", info.manufacturer_name);
        ssd1351_interface_debug_print("ssd1351: interface is %s.\n", info.interface);
        ssd1351_interface_debug_print("ssd1351: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
        ssd1351_interface_debug_print("ssd1351: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ssd1351_interface_debug_print("ssd1351: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ssd1351_interface_debug_print("ssd1351: max current is %0.2fmA.\n", info.max_current_ma);
        ssd1351_interface_debug_print("ssd1351: max temperature is %0.1fC.\n", info.temperature_max);
        ssd1351_interface_debug_print("ssd1351: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* ssd1351 init */
    res = ssd1351_init(&gs_handle);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: ssd1351 init.\n");
        
        return 1;
    }
    
    /* unlock oled driver ic */
    res = ssd1351_set_command(&gs_handle, SSD1351_COMMAND_UNLOCK_OLED_DRIVER_IC);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set command failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable command */
    res = ssd1351_set_command(&gs_handle, SSD1351_COMMAND_A2_B1_B3_BB_BE_ACCESSIBLE);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set command failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enter sleep mode */
    res = ssd1351_set_sleep_mode(&gs_handle, SSD1351_BOOL_TRUE);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set sleep mode failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default front clock oscillator frequency */
    res = ssd1351_set_front_clock_oscillator_frequency(&gs_handle, 0x01, 0x0F);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set front clock oscillator failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default mux ratio */
    res = ssd1351_set_mux_ratio(&gs_handle, 0x7F);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set mux ratio failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default display offset */
    res = ssd1351_set_display_offset(&gs_handle, 0x00);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set display offset failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default display start line */
    res = ssd1351_set_display_start_line(&gs_handle, 0x00);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set display start line failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default address increment */
    res = ssd1351_set_address_increment(&gs_handle, SSD1351_ADDRESS_INCREMENT_HORIZONTAL);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set address increment failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default color depth */
    res = ssd1351_set_color_depth(&gs_handle, SSD1351_COLOR_DEPTH_65K);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set color depth failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default seg0 map */
    res = ssd1351_set_seg0_map(&gs_handle, SSD1351_SEG0_ADDRESS_0);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set seg0 map failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default color sequence */
    res =  ssd1351_set_color_sequence(&gs_handle, SSD1351_COLOR_SEQUENCE_C_B_A);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set color sequence failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default scan mode */
    res = ssd1351_set_scan_mode(&gs_handle, SSD1351_SCAN_MODE_COM_N_1_COM_0);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set scan mode failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default com split odd even */
    res = ssd1351_set_com_split_odd_even(&gs_handle, SSD1351_BOOL_TRUE);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set com split odd even failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default gpio */
    res = ssd1351_set_gpio(&gs_handle, SSD1351_GPIO_PIN_INPUT_DISABLED, SSD1351_GPIO_PIN_INPUT_DISABLED);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set gpio failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default select vdd */
    res = ssd1351_set_select_vdd(&gs_handle, SSD1351_SELECT_VDD_INTERNAL);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set select vdd failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default parallel bits */
    res = ssd1351_set_parallel_bits(&gs_handle, SSD1351_SELECT_PARALLEL_8_BIT);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set parallel bits failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default segment low voltage */
    res = ssd1351_set_segment_low_voltage(&gs_handle, SSD1351_SEGMENT_LOW_VOLTAGE_EXTERNAL_VSL);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set segment low voltage failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default contrast */
    res = ssd1351_set_contrast(&gs_handle, 0xC8, 0x80, 0xC8);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set contrast failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default master contrast current */
    res = ssd1351_set_master_contrast_current(&gs_handle, 0x0A);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set master contrast failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default gray scale pulse width */
    res = ssd1351_set_gray_scale_pulse_width(&gs_handle, (uint8_t *)gsc_gamma);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set gray scale pulse width failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default phase period */
    res = ssd1351_set_phase_period(&gs_handle, 0x02, 0x03);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set phase period failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default pre charge voltage */
    res = ssd1351_set_pre_charge_voltage(&gs_handle, 0x17);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set pre charge voltage failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default vcomh voltage */
    res = ssd1351_set_vcomh_voltage(&gs_handle, 0x05);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set vcomh voltage failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default second pre charge period */
    res = ssd1351_set_second_pre_charge_period(&gs_handle, 0x01);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set second pre charge period failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default display mode */
    res = ssd1351_set_display_mode(&gs_handle, SSD1351_DISPLAY_MODE_NORMAL);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set display mode failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* exit sleep mode */
    res = ssd1351_set_sleep_mode(&gs_handle, SSD1351_BOOL_FALSE);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set sleep mode failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* ssd1351 stop moving */
    res = ssd1351_stop_moving(&gs_handle);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: stop moving failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear screen */
    res = ssd1351_clear(&gs_handle);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: clear failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    ssd1351_interface_delay_ms(1000);
    
    /* start display test */
    ssd1351_interface_debug_print("ssd1351: start display test.\n");
    
    /* font 12 test */
    ssd1351_interface_debug_print("ssd1351: font 12 test.\n");
    res = ssd1351_write_string(&gs_handle, 0, 0, (char *)test_str1, strlen(test_str1), 0xF800, SSD1351_FONT_12);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: write string failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_write_string(&gs_handle, 0, 32, (char *)test_str2, strlen(test_str2), 0x07E0, SSD1351_FONT_12);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: write string failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_write_string(&gs_handle, 0, 64, (char *)test_str3, strlen(test_str3), 0x001F, SSD1351_FONT_12);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: write string failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_write_string(&gs_handle, 0, 96, (char *)test_str4, strlen(test_str4), 0xFFFF, SSD1351_FONT_12);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: write string failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 3s */
    ssd1351_interface_delay_ms(3000);
    
    /* clear screen */
    res = ssd1351_clear(&gs_handle);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: clear failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* font 16 test */
    ssd1351_interface_debug_print("ssd1351: font 16 test.\n");
    res = ssd1351_write_string(&gs_handle, 0, 0, (char *)test_str1, strlen(test_str1), 0xF800, SSD1351_FONT_16);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: write string failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_write_string(&gs_handle, 0, 32, (char *)test_str2, strlen(test_str2), 0x07E0, SSD1351_FONT_16);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: write string failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_write_string(&gs_handle, 0, 64, (char *)test_str3, strlen(test_str3), 0x001F, SSD1351_FONT_16);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: write string failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_write_string(&gs_handle, 0, 96, (char *)test_str4, strlen(test_str4), 0xFFFF, SSD1351_FONT_16);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: write string failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 3s */
    ssd1351_interface_delay_ms(3000);
    
    /* clear screen */
    res = ssd1351_clear(&gs_handle);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: clear failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* font 24 test */
    ssd1351_interface_debug_print("ssd1351: font 24 test.\n");
    res = ssd1351_write_string(&gs_handle, 0, 0, (char *)test_str1, strlen(test_str1), 0xF800, SSD1351_FONT_24);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: write string failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_write_string(&gs_handle, 0, 32, (char *)test_str2, strlen(test_str2), 0x07E0, SSD1351_FONT_24);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: write string failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_write_string(&gs_handle, 0, 64, (char *)test_str3, strlen(test_str3), 0x001F, SSD1351_FONT_24);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: write string failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_write_string(&gs_handle, 0, 96, (char *)test_str4, strlen(test_str4), 0xFFFF, SSD1351_FONT_24);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: write string failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 3s */
    ssd1351_interface_delay_ms(3000);
    
    /* clear screen */
    res = ssd1351_clear(&gs_handle);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: clear failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* rectangle test */
    ssd1351_interface_debug_print("ssd1351: rectangle test.\n");
    res = ssd1351_fill_rect(&gs_handle, 0, 0, 31, 31, 0xF800);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: fill rect failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_fill_rect(&gs_handle, 0, 32, 63, 63, 0x07E0);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: fill rect failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_fill_rect(&gs_handle, 0, 64, 95, 95, 0x001F);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: fill rect failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_fill_rect(&gs_handle, 0, 96, 127, 127, 0xFFFF);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: fill rect failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 3s */
    ssd1351_interface_delay_ms(3000);
    
    /* clear screen */
    res = ssd1351_clear(&gs_handle);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: clear failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* rand point test */
    ssd1351_interface_debug_print("ssd1351: rand point test.\n");
    for (i = 0; i < 128; i++)
    {
        for (j = 0; j < 128; j++)
        {
            res = ssd1351_draw_point(&gs_handle, i, j, rand()%0xFFFF);
            if (res)
            {
                ssd1351_interface_debug_print("ssd1351: draw point failed.\n");
                ssd1351_deinit(&gs_handle);
                
                return 1;
            }
        }
    }
    
    /* delay 3s */
    ssd1351_interface_delay_ms(3000);
    
    /* clear screen */
    res = ssd1351_clear(&gs_handle);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: clear failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* picture test */
    ssd1351_interface_debug_print("ssd1351: picture test.\n");
    res = ssd1351_draw_picture_16bits(&gs_handle, 0, 0, 127, 127, (uint16_t *)gsc_image);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: draw picture 16bits failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 3s */
    ssd1351_interface_delay_ms(3000);
    
    /* graphic acceleration test */
    ssd1351_interface_debug_print("ssd1351: graphic acceleration test.\n");
    
    /* left horizontal scroll */
    ssd1351_interface_debug_print("ssd1351: left horizontal scroll.\n");
    res = ssd1351_set_scroll(&gs_handle, 1, 0, 128, SSD1351_SCROLL_MODE_NORMAL);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set scroll failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_start_moving(&gs_handle);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: start moving failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 5s */
    ssd1351_interface_delay_ms(5000);
    
    /* right horizontal scroll */
    ssd1351_interface_debug_print("ssd1351: right horizontal scroll.\n");
    res = ssd1351_set_scroll(&gs_handle, -1, 0, 128, SSD1351_SCROLL_MODE_NORMAL);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set scroll failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    res = ssd1351_start_moving(&gs_handle);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: start moving failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* delay 5s */
    ssd1351_interface_delay_ms(5000);
    res = ssd1351_stop_moving(&gs_handle);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: stop moving failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish display test */
    ssd1351_interface_debug_print("ssd1351: finish display test.\n");
    ssd1351_deinit(&gs_handle);
    
    return 0;
}
