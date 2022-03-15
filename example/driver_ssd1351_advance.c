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
 * @file      driver_ssd1351_advance.c
 * @brief     driver ssd1351 advance source file
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

#include "driver_ssd1351_advance.h"

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
 * @brief  advance example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t ssd1351_advance_init(void)
{
    volatile uint8_t res;

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
    res = ssd1351_set_front_clock_oscillator_frequency(&gs_handle, SSD1351_ADVANCE_DEFAULT_CLOCK_DIV, SSD1351_ADVANCE_DEFAULT_OSCILLATOR_FREQUENCY);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set front clock oscillator failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default mux ratio */
    res = ssd1351_set_mux_ratio(&gs_handle, SSD1351_ADVANCE_DEFAULT_MUX_RATIO);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set mux ratio failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default display offset */
    res = ssd1351_set_display_offset(&gs_handle, SSD1351_ADVANCE_DEFAULT_DISPLAY_OFFSET);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set display offset failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default display start line */
    res = ssd1351_set_display_start_line(&gs_handle, SSD1351_ADVANCE_DEFAULT_DISPLAY_START_LINE);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set display start line failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default address increment */
    res = ssd1351_set_address_increment(&gs_handle, SSD1351_ADVANCE_DEFAULT_ADDRESS_INCREMENT);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set address increment failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default color depth */
    res = ssd1351_set_color_depth(&gs_handle, SSD1351_ADVANCE_DEFAULT_COLOR_DEPTH);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set color depth failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default seg0 map */
    res = ssd1351_set_seg0_map(&gs_handle, SSD1351_ADVANCE_DEFAULT_SEG0_MAP);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set seg0 map failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default color sequence */
    res =  ssd1351_set_color_sequence(&gs_handle, SSD1351_ADVANCE_DEFAULT_COLOR_SEQUENCE);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set color sequence failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default scan mode */
    res = ssd1351_set_scan_mode(&gs_handle, SSD1351_ADVANCE_DEFAULT_SCAN_MODE);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set scan mode failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default com split odd even */
    res = ssd1351_set_com_split_odd_even(&gs_handle, SSD1351_ADVANCE_DEFAULT_COM_SPLIT_ODD_EVEN);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set com split odd even failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default gpio */
    res = ssd1351_set_gpio(&gs_handle, SSD1351_ADVANCE_DEFAULT_GPIO0_MODE, SSD1351_ADVANCE_DEFAULT_GPIO1_MODE);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set gpio failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default select vdd */
    res = ssd1351_set_select_vdd(&gs_handle, SSD1351_ADVANCE_DEFAULT_SELECT_VDD);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set select vdd failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default parallel bits */
    res = ssd1351_set_parallel_bits(&gs_handle, SSD1351_ADVANCE_DEFAULT_SELECT_PARALLEL);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set parallel bits failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default segment low voltage */
    res = ssd1351_set_segment_low_voltage(&gs_handle, SSD1351_ADVANCE_DEFAULT_SEGMENT_LOW_VOLTAGE);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set segment low voltage failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default contrast */
    res = ssd1351_set_contrast(&gs_handle, SSD1351_ADVANCE_DEFAULT_CONTRAST_A, SSD1351_ADVANCE_DEFAULT_CONTRAST_B, SSD1351_ADVANCE_DEFAULT_CONTRAST_C);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set contrast failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default master contrast current */
    res = ssd1351_set_master_contrast_current(&gs_handle, SSD1351_ADVANCE_DEFAULT_MASTER_CONTRAST_CURRENT);
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
    res = ssd1351_set_phase_period(&gs_handle, SSD1351_ADVANCE_DEFAULT_PHASE1_PERIOD, SSD1351_ADVANCE_DEFAULT_PHASE2_PERIOD);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set phase period failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default pre charge voltage */
    res = ssd1351_set_pre_charge_voltage(&gs_handle, SSD1351_ADVANCE_DEFAULT_PRE_CHARGE_VOLTAGE);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set pre charge voltage failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default vcomh voltage */
    res = ssd1351_set_vcomh_voltage(&gs_handle, SSD1351_ADVANCE_DEFAULT_VCOMH_VOLTAGE);
    if (res)
    {
        ssd1351_interface_debug_print("ssd1351: set vcomh voltage failed.\n");
        ssd1351_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default second pre charge period */
    res = ssd1351_set_second_pre_charge_period(&gs_handle, SSD1351_ADVANCE_DEFAULT_SECOND_PRE_CHARGE_PERIOD);
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
    
    /* clear screen */
    return ssd1351_clear(&gs_handle);
}

/**
 * @brief  advance example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ssd1351_advance_deinit(void)
{
    /* ssd1351 deinit */
    if (ssd1351_deinit(&gs_handle))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  advance example clear
 * @return status code
 *         - 0 success
 *         - 1 clear failed
 * @note   none
 */
uint8_t ssd1351_advance_clear(void)
{
    /* ssd1351 clear */
    if (ssd1351_clear(&gs_handle))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  advance example display on
 * @return status code
 *         - 0 success
 *         - 1 display on failed
 * @note   none
 */
uint8_t ssd1351_advance_display_on(void)
{
    /* ssd1351 exit sleep mode */
    if (ssd1351_set_sleep_mode(&gs_handle, SSD1351_BOOL_FALSE))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  advance example display off
 * @return status code
 *         - 0 success
 *         - 1 display off failed
 * @note   none
 */
uint8_t ssd1351_advance_display_off(void)
{
    /* ssd1351 enter sleep mode */
    if (ssd1351_set_sleep_mode(&gs_handle, SSD1351_BOOL_TRUE))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example draw a string
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] *str points to a written string address
 * @param[in] len is the length of the string
 * @param[in] color is the display color
 * @param[in] font is the display font size
 * @return    status code
 *            - 0 success
 *            - 1 draw string failed
 * @note      none
 */
uint8_t ssd1351_advance_string(uint8_t x, uint8_t y, char *str, uint16_t len, uint32_t color, ssd1351_font_t font)
{
    /* write string */
    if (ssd1351_write_string(&gs_handle, x, y, str, len, color, font))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example write a point
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] color is the written color
 * @return    status code
 *            - 0 success
 *            - 1 write point failed
 * @note      none
 */
uint8_t ssd1351_advance_write_point(uint8_t x, uint8_t y, uint32_t color)
{
    /* draw point */
    if (ssd1351_draw_point(&gs_handle, x, y, color))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example fill a rectangle
 * @param[in] left is the left coordinate x
 * @param[in] top is the top coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the bottom coordinate y
 * @param[in] color is the display color
 * @return    status code
 *            - 0 success
 *            - 1 fill rect failed
 * @note      none
 */
uint8_t ssd1351_advance_rect(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint32_t color)
{
    /* fill rect */
    if (ssd1351_fill_rect(&gs_handle, left, top, right, bottom, color))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example draw a picture
 * @param[in] left is the left coordinate x
 * @param[in] top is the top coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the bottom coordinate y
 * @param[in] *img points to a image buffer
 * @return    status code
 *            - 0 success
 *            - 1 draw picture failed
 * @note      none
 */
uint8_t ssd1351_advance_draw_pictrue(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint32_t *img)
{
    /* draw picture */
    if (ssd1351_draw_picture(&gs_handle, left, top, right, bottom, img))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example draw a 16 bits picture
 * @param[in] left is the left coordinate x
 * @param[in] top is the top coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the bottom coordinate y
 * @param[in] *img points to a image buffer
 * @return    status code
 *            - 0 success
 *            - 1 draw picture 16 bits failed
 * @note      none
 */
uint8_t ssd1351_advance_draw_pictrue_16bits(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint16_t *img)
{
    /* draw picture in 16 bits */
    if (ssd1351_draw_picture_16bits(&gs_handle, left, top, right, bottom, img))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     advance example set scroll
 * @param[in] scroll is the scoll number
 * @param[in] start_row is the start row
 * @param[in] row_len is the scrolling row length
 * @param[in] mode is the scroll mode
 * @return    status code
 *            - 0 success
 *            - 1 set scroll failed
 * @note      start_row <= 127 && start_row + row_len <= 128
 */
uint8_t ssd1351_advance_set_scroll(int8_t scroll, uint8_t start_row, uint8_t row_len, ssd1351_scroll_mode_t mode)
{
    /* set scroll */
    if (ssd1351_set_scroll(&gs_handle, scroll, start_row, row_len, mode))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  advance example enable scroll
 * @return status code
 *         - 0 success
 *         - 1 enable scroll failed
 * @note   none
 */
uint8_t ssd1351_advance_enable_scroll(void)
{
    /* ssd1351 start moving */
    if (ssd1351_start_moving(&gs_handle))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  advance example disable scroll
 * @return status code
 *         - 0 success
 *         - 1 disable scroll failed
 * @note   none
 */
uint8_t ssd1351_advance_disable_scroll(void)
{
    /* ssd1351 stop moving */
    if (ssd1351_stop_moving(&gs_handle))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
