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
 * @file      driver_ssd1351_advance.h
 * @brief     driver ssd1351 advance header file
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

#ifndef DRIVER_SSD1351_ADVANCE_H
#define DRIVER_SSD1351_ADVANCE_H

#include "driver_ssd1351_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup ssd1351_example_driver
 * @{
 */

/**
 * @brief ssd1351 advance example default definition
 */
#define SSD1351_ADVANCE_DEFAULT_CLOCK_DIV                      0x01                                            /* div 2 */
#define SSD1351_ADVANCE_DEFAULT_OSCILLATOR_FREQUENCY           0x0F                                            /* set 0xF */
#define SSD1351_ADVANCE_DEFAULT_MUX_RATIO                      0x7F                                            /* set 127 */
#define SSD1351_ADVANCE_DEFAULT_DISPLAY_OFFSET                 0x00                                            /* set 0 */
#define SSD1351_ADVANCE_DEFAULT_DISPLAY_START_LINE             0x00                                            /* set 0 */
#define SSD1351_ADVANCE_DEFAULT_ADDRESS_INCREMENT              SSD1351_ADDRESS_INCREMENT_HORIZONTAL            /* horizontal increment */
#define SSD1351_ADVANCE_DEFAULT_COLOR_DEPTH                    SSD1351_COLOR_DEPTH_65K                         /* set RGB565 */
#define SSD1351_ADVANCE_DEFAULT_SEG0_MAP                       SSD1351_SEG0_ADDRESS_0                          /* seg0 map to 0 */
#define SSD1351_ADVANCE_DEFAULT_COLOR_SEQUENCE                 SSD1351_COLOR_SEQUENCE_C_B_A                    /* set sequence RGB */
#define SSD1351_ADVANCE_DEFAULT_SCAN_MODE                      SSD1351_SCAN_MODE_COM_N_1_COM_0                 /* set scan mode com127 -com0 */
#define SSD1351_ADVANCE_DEFAULT_COM_SPLIT_ODD_EVEN             SSD1351_BOOL_TRUE                               /* enable split odd even */
#define SSD1351_ADVANCE_DEFAULT_GPIO0_MODE                     SSD1351_GPIO_PIN_INPUT_DISABLED                 /* disable gpio0 input */
#define SSD1351_ADVANCE_DEFAULT_GPIO1_MODE                     SSD1351_GPIO_PIN_INPUT_DISABLED                 /* disable gpio1 input */
#define SSD1351_ADVANCE_DEFAULT_SELECT_VDD                     SSD1351_SELECT_VDD_INTERNAL                     /* vdd internal */
#define SSD1351_ADVANCE_DEFAULT_SELECT_PARALLEL                SSD1351_SELECT_PARALLEL_8_BIT                   /* set parallel 8 bit */
#define SSD1351_ADVANCE_DEFAULT_SEGMENT_LOW_VOLTAGE            SSD1351_SEGMENT_LOW_VOLTAGE_EXTERNAL_VSL        /* set segment low voltage external VSL */
#define SSD1351_ADVANCE_DEFAULT_CONTRAST_A                     0xC8                                            /* set B contrast */
#define SSD1351_ADVANCE_DEFAULT_CONTRAST_B                     0x80                                            /* set G contrast */
#define SSD1351_ADVANCE_DEFAULT_CONTRAST_C                     0xC8                                            /* set R contrast */
#define SSD1351_ADVANCE_DEFAULT_MASTER_CONTRAST_CURRENT        0x0A                                            /* set master current 0x0A */
#define SSD1351_ADVANCE_DEFAULT_PHASE1_PERIOD                  0x02                                            /* set phase1 period 0x02 */
#define SSD1351_ADVANCE_DEFAULT_PHASE2_PERIOD                  0x03                                            /* set phase2 period 0x03 */
#define SSD1351_ADVANCE_DEFAULT_PRE_CHARGE_VOLTAGE             0x17                                            /* set pre charge voltage 0x17 */
#define SSD1351_ADVANCE_DEFAULT_VCOMH_VOLTAGE                  0x05                                            /* set vcomh voltage 0x05 */
#define SSD1351_ADVANCE_DEFAULT_SECOND_PRE_CHARGE_PERIOD       0x01                                            /* set second pre-charge period 0x001 */

/**
 * @brief  advance example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t ssd1351_advance_init(void);

/**
 * @brief  advance example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ssd1351_advance_deinit(void);

/**
 * @brief  advance example clear
 * @return status code
 *         - 0 success
 *         - 1 clear failed
 * @note   none
 */
uint8_t ssd1351_advance_clear(void);

/**
 * @brief  advance example display on
 * @return status code
 *         - 0 success
 *         - 1 display on failed
 * @note   none
 */
uint8_t ssd1351_advance_display_on(void);

/**
 * @brief  advance example display off
 * @return status code
 *         - 0 success
 *         - 1 display off failed
 * @note   none
 */
uint8_t ssd1351_advance_display_off(void);

/**
 * @brief     advance example draw a string
 * @param[in] x coordinate x
 * @param[in] y coordinate y
 * @param[in] *str pointer to a written string address
 * @param[in] len length of the string
 * @param[in] color display color
 * @param[in] font display font size
 * @return    status code
 *            - 0 success
 *            - 1 draw string failed
 * @note      none
 */
uint8_t ssd1351_advance_string(uint8_t x, uint8_t y, char *str, uint16_t len, uint32_t color, ssd1351_font_t font);

/**
 * @brief     advance example write a point
 * @param[in] x coordinate x
 * @param[in] y coordinate y
 * @param[in] color written color
 * @return    status code
 *            - 0 success
 *            - 1 write point failed
 * @note      none
 */
uint8_t ssd1351_advance_write_point(uint8_t x, uint8_t y, uint32_t color);

/**
 * @brief     advance example fill a rectangle
 * @param[in] left left coordinate x
 * @param[in] top top coordinate y
 * @param[in] right right coordinate x
 * @param[in] bottom bottom coordinate y
 * @param[in] color display color
 * @return    status code
 *            - 0 success
 *            - 1 fill rect failed
 * @note      none
 */
uint8_t ssd1351_advance_rect(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint32_t color);

/**
 * @brief     advance example draw a picture
 * @param[in] left left coordinate x
 * @param[in] top top coordinate y
 * @param[in] right right coordinate x
 * @param[in] bottom bottom coordinate y
 * @param[in] *img pointer to a image buffer
 * @return    status code
 *            - 0 success
 *            - 1 draw picture failed
 * @note      none
 */
uint8_t ssd1351_advance_draw_picture(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint32_t *img);

/**
 * @brief     advance example draw a 16 bits picture
 * @param[in] left left coordinate x
 * @param[in] top top coordinate y
 * @param[in] right right coordinate x
 * @param[in] bottom bottom coordinate y
 * @param[in] *img pointer to a image buffer
 * @return    status code
 *            - 0 success
 *            - 1 draw picture 16 bits failed
 * @note      none
 */
uint8_t ssd1351_advance_draw_picture_16bits(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint16_t *img);

/**
 * @brief     advance example set scroll
 * @param[in] scroll scroll number
 * @param[in] start_row start row
 * @param[in] row_len scrolling row length
 * @param[in] mode scroll mode
 * @return    status code
 *            - 0 success
 *            - 1 set scroll failed
 * @note      start_row <= 127 && start_row + row_len <= 128
 */
uint8_t ssd1351_advance_set_scroll(int8_t scroll, uint8_t start_row, uint8_t row_len, ssd1351_scroll_mode_t mode);

/**
 * @brief  advance example enable scroll
 * @return status code
 *         - 0 success
 *         - 1 enable scroll failed
 * @note   none
 */
uint8_t ssd1351_advance_enable_scroll(void);

/**
 * @brief  advance example disable scroll
 * @return status code
 *         - 0 success
 *         - 1 disable scroll failed
 * @note   none
 */
uint8_t ssd1351_advance_disable_scroll(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
