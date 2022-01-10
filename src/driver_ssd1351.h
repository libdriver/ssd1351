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
 * @file      driver_ssd1351.h
 * @brief     driver ssd1351 header file
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

#ifndef _DRIVER_SSD1351_H_
#define _DRIVER_SSD1351_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ssd1351_driver ssd1351 driver function
 * @brief    ssd1351 driver modules
 * @{
 */


/**
 * @addtogroup ssd1351_base_driver
 * @{
 */

/**
 * @brief ssd1351 auto color convert definition
 */
#ifndef SSD1351_AUTO_COLOR_CONVERT
    #define SSD1351_AUTO_COLOR_CONVERT  0        /**< not auto convert */
#endif

/**
 * @brief ssd1351 color depth enumeration definition
 */
typedef enum
{
    SSD1351_COLOR_DEPTH_256      = 0x00,        /**< 256 color depth */
    SSD1351_COLOR_DEPTH_65K      = 0x01,        /**< 65K color depth */
    SSD1351_COLOR_DEPTH_262K_1ST = 0x02,        /**< 262K 1ST color depth */
    SSD1351_COLOR_DEPTH_262K_2ND = 0x03,        /**< 262K 2ND color depth */
} ssd1351_color_depth_t;

/**
 * @brief ssd1351 address increment enumeration definition
 */
typedef enum
{
    SSD1351_ADDRESS_INCREMENT_HORIZONTAL = 0x00,        /**< horizontal increment */
    SSD1351_ADDRESS_INCREMENT_VERTICAL   = 0x01,        /**< vertical increment */
} ssd1351_address_increment_t;

/**
 * @brief ssd1351 seg0 mapped enumeration definition
 */
typedef enum
{
    SSD1351_SEG0_ADDRESS_0   = 0x00,        /**< seg0 mapped to address 0 */
    SSD1351_SEG0_ADDRESS_127 = 0x01,        /**< seg0 mapped to address 127 */
} ssd1351_seg0_mapped_t;

/**
 * @brief ssd1351 color sequence enumeration definition
 */
typedef enum
{
    SSD1351_COLOR_SEQUENCE_A_B_C = 0x00,        /**< color sequence a b c */
    SSD1351_COLOR_SEQUENCE_C_B_A = 0x01,        /**< color sequence c b a */
} ssd1351_color_sequence_t;

/**
 * @brief ssd1351 scan mode enumeration definition
 */
typedef enum
{
    SSD1351_SCAN_MODE_COM_0_COM_N_1 = 0x00,        /**< scan mode com0 - com n-1 */
    SSD1351_SCAN_MODE_COM_N_1_COM_0 = 0x01,        /**< scan mode com n-1 com 0 */
} ssd1351_scan_mode_t;

/**
 * @brief ssd1351 bool enumeration definition
 */
typedef enum
{
    SSD1351_BOOL_FALSE = 0x00,        /**< false */
    SSD1351_BOOL_TRUE  = 0x01,        /**< true */
} ssd1351_bool_t;

/**
 * @brief ssd1351 display mode enumeration definition
 */
typedef enum
{
    SSD1351_DISPLAY_MODE_ALL_OFF = 0xA4,        /**< all off mode */
    SSD1351_DISPLAY_MODE_ALL_ON  = 0xA5,        /**< all on mode */
    SSD1351_DISPLAY_MODE_NORMAL  = 0xA6,        /**< normal mode */
    SSD1351_DISPLAY_MODE_INVERSE = 0xA7,        /**< inverse mode */
} ssd1351_display_mode_t;

/**
 * @brief ssd1351 select vdd enumeration definition
 */
typedef enum
{
    SSD1351_SELECT_VDD_EXTERNAL = 0x00,        /**< select external vdd */
    SSD1351_SELECT_VDD_INTERNAL = 0x01,        /**< select internal vdd */
} ssd1351_select_vdd_t;

/**
 * @brief ssd1351 select parallel enumeration definition
 */
typedef enum
{
    SSD1351_SELECT_PARALLEL_8_BIT = 0x00,        /**< select parallel 8 bit */
    SSD1351_SELECT_PARALLER_16_BIT= 0x01,        /**< select parallel 16 bit */
    SSD1351_SELECT_PARALLER_18_BIT= 0x03,        /**< select parallel 18 bit */
} ssd1351_select_parallel_bits_t;

/**
 * @brief ssd1351 segment low voltage enumeration definition
 */
typedef enum
{
    SSD1351_SEGMENT_LOW_VOLTAGE_EXTERNAL_VSL = 0x00,        /**< segment low voltage external vsl */
    SSD1351_SEGMENT_LOW_VOLTAGE_INTERNAL_VSL = 0x02,        /**< segment low voltage internal vsl */
} ssd1351_segment_low_voltage_t;

/**
 * @brief ssd1351 gpio pin enumeration definition
 */
typedef enum
{
    SSD1351_GPIO_PIN_INPUT_DISABLED = 0x00,        /**< gpio pin input disabled */
    SSD1351_GPIO_PIN_INPUT_ENABLED  = 0x01,        /**< gpio pin input enabled */
    SSD1351_GPIO_PIN_OUTPUT_LOW     = 0x02,        /**< gpio pin output low */
    SSD1351_GPIO_PIN_OUTPUT_HIGH    = 0x03,        /**< gpio pin output high */
} ssd1351_gpio_pin_t;

/**
 * @brief ssd1351 command enumeration definition
 */
typedef enum
{
    SSD1351_COMMAND_UNLOCK_OLED_DRIVER_IC       = 0x12,        /**< unlock the oled driver ic */
    SSD1351_COMMAND_LOCK_OLED_DRIVER_IC         = 0x16,        /**< lock the oled driver ic */
    SSD1351_COMMAND_A2_B1_B3_BB_BE_INACCESSIBLE = 0xB0,        /**< a2 b1 b3 bb be inaccessible */
    SSD1351_COMMAND_A2_B1_B3_BB_BE_ACCESSIBLE   = 0xB1,        /**< a2 b1 b3 bb be accessible */
} ssd1351_command_t;

/**
 * @brief ssd1351 scroll mode enumeration definition
 */
typedef enum
{
    SSD1351_SCROLL_MODE_TEST    = 0x00,        /**< test scroll mode */
    SSD1351_SCROLL_MODE_NORMAL  = 0x01,        /**< normal scroll mode */
    SSD1351_SCROLL_MODE_SLOW    = 0x01,        /**< slow scroll mode */
    SSD1351_SCROLL_MODE_SLOWEST = 0x03,        /**< slowest scroll mode */
} ssd1351_scroll_mode_t;

/**
 * @brief ssd1351 font size enumeration definition
 */
typedef enum
{
    SSD1351_FONT_12 = 0x0C,        /**< font 12 */
    SSD1351_FONT_16 = 0x10,        /**< font 16 */
    SSD1351_FONT_24 = 0x18,        /**< font 24 */
} ssd1351_font_t;

/**
 * @brief ssd1351 handle structure definition
 */
typedef struct ssd1351_handle_s
{
    uint8_t (*spi_init)(void);                                   /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                 /**< point to a spi_deinit function address */
    uint8_t (*spi_write_cmd)(uint8_t *buf, uint16_t len);        /**< point to a spi_write_cmd function address */
    uint8_t (*cmd_data_gpio_init)(void);                         /**< point to a cmd_data_gpio_init function address */
    uint8_t (*cmd_data_gpio_deinit)(void);                       /**< point to a cmd_data_gpio_deinit function address */
    uint8_t (*cmd_data_gpio_write)(uint8_t value);               /**< point to a cmd_data_gpio_write function address */
    uint8_t (*reset_gpio_init)(void);                            /**< point to a reset_gpio_init function address */
    uint8_t (*reset_gpio_deinit)(void);                          /**< point to a reset_gpio_deinit function address */
    uint8_t (*reset_gpio_write)(uint8_t value);                  /**< point to a reset_gpio_write function address */
    uint16_t (*debug_print)(char *fmt, ...);                     /**< point to a debug_print function address */
    void (*delay_ms)(uint32_t ms);                               /**< point to a delay_ms function address */
    uint8_t inited;                                              /**< inited flag */
    uint8_t conf_1;                                              /**< config 1 */
    uint8_t conf_2;                                              /**< config 2 */
} ssd1351_handle_t;

/**
 * @brief ssd1351 information structure definition
 */
typedef struct ssd1351info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ssd1351_info_t;

/**
 * @}
 */

/**
 * @defgroup ssd1351_link_driver ssd1351 link driver function
 * @brief    ssd1351 link driver modules
 * @ingroup  ssd1351_driver
 * @{
 */

/**
 * @brief     initialize ssd1351_handle_t structure
 * @param[in] HANDLE points to a ssd1351 handle structure
 * @param[in] STRUCTURE is ssd1351_handle_t
 * @note      none
 */
#define DRIVER_SSD1351_LINK_INIT(HANDLE, STRUCTURE)                 memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link spi_init function
 * @param[in] HANDLE points to a ssd1351 handle structure
 * @param[in] FUC points to a spi_init function address
 * @note      none
 */
#define DRIVER_SSD1351_LINK_SPI_INIT(HANDLE, FUC)                  (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE points to a ssd1351 handle structure
 * @param[in] FUC points to a spi_deinit function address
 * @note      none
 */
#define DRIVER_SSD1351_LINK_SPI_DEINIT(HANDLE, FUC)                (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_write_cmd function
 * @param[in] HANDLE points to a ssd1351 handle structure
 * @param[in] FUC points to a spi_write_cmd function address
 * @note      none
 */
#define DRIVER_SSD1351_LINK_SPI_WRITE_COMMAND(HANDLE, FUC)         (HANDLE)->spi_write_cmd = FUC

/**
 * @brief     link cmd_data_gpio_init function
 * @param[in] HANDLE points to a ssd1351 handle structure
 * @param[in] FUC points to a cmd_data_gpio_init function address
 * @note      none
 */
#define DRIVER_SSD1351_LINK_COMMAND_DATA_GPIO_INIT(HANDLE, FUC)    (HANDLE)->cmd_data_gpio_init = FUC

/**
 * @brief     link cmd_data_gpio_deinit function
 * @param[in] HANDLE points to a ssd1351 handle structure
 * @param[in] FUC points to a cmd_data_gpio_deinit function address
 * @note      none
 */
#define DRIVER_SSD1351_LINK_COMMAND_DATA_GPIO_DEINIT(HANDLE, FUC)  (HANDLE)->cmd_data_gpio_deinit = FUC

/**
 * @brief     link cmd_data_gpio_write function
 * @param[in] HANDLE points to a ssd1351 handle structure
 * @param[in] FUC points to a cmd_data_gpio_write function address
 * @note      none
 */
#define DRIVER_SSD1351_LINK_COMMAND_DATA_GPIO_WRITE(HANDLE, FUC)   (HANDLE)->cmd_data_gpio_write = FUC

/**
 * @brief     link reset_gpio_init function
 * @param[in] HANDLE points to a ssd1351 handle structure
 * @param[in] FUC points to a reset_gpio_init function address
 * @note      none
 */
#define DRIVER_SSD1351_LINK_RESET_GPIO_INIT(HANDLE, FUC)           (HANDLE)->reset_gpio_init = FUC

/**
 * @brief     link reset_gpio_deinit function
 * @param[in] HANDLE points to a ssd1351 handle structure
 * @param[in] FUC points to a reset_gpio_deinit function address
 * @note      none
 */
#define DRIVER_SSD1351_LINK_RESET_GPIO_DEINIT(HANDLE, FUC)         (HANDLE)->reset_gpio_deinit = FUC

/**
 * @brief     link reset_gpio_write function
 * @param[in] HANDLE points to a ssd1351 handle structure
 * @param[in] FUC points to a reset_gpio_write function address
 * @note      none
 */
#define DRIVER_SSD1351_LINK_RESET_GPIO_WRITE(HANDLE, FUC)          (HANDLE)->reset_gpio_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to a ssd1351 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_SSD1351_LINK_DELAY_MS(HANDLE, FUC)                  (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to a ssd1351 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_SSD1351_LINK_DEBUG_PRINT(HANDLE, FUC)               (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup ssd1351_base_driver ssd1351 base driver function
 * @brief    ssd1351 base driver modules
 * @ingroup  ssd1351_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to a ssd1351 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ssd1351_info(ssd1351_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a ssd1351 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 *            - 5 command && data init failed
 * @note      none
 */
uint8_t ssd1351_init(ssd1351_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to a ssd1351 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 *            - 5 reset gpio deinit failed
 *            - 6 command && data deinit failed
 * @note      none
 */
uint8_t ssd1351_deinit(ssd1351_handle_t *handle);

/**
 * @brief     clear the display
 * @param[in] *handle points to a ssd1351 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear failed
 * @note      none
 */
uint8_t ssd1351_clear(ssd1351_handle_t *handle);

/**
 * @brief     draw a point in the display
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] color is the point color
 * @return    status code
 *            - 0 success
 *            - 1 draw point failed
 * @note      x <= 127 && y <= 127
 */
uint8_t ssd1351_draw_point(ssd1351_handle_t *handle, uint8_t x, uint8_t y, uint32_t color);

/**
 * @brief     write a string in the display
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] *str points to a written string address
 * @param[in] len is the length of the string
 * @param[in] color is the display color
 * @param[in] font is the string font
 * @return    status code
 *            - 0 success
 *            - 1 draw point failed
 * @note      x <= 127 && y <= 127
 */
uint8_t ssd1351_write_string(ssd1351_handle_t *handle, uint8_t x, uint8_t y, char *str, uint16_t len, uint32_t color, ssd1351_font_t font);

/**
 * @brief     fill the rect
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] left is the left coordinate x
 * @param[in] top is the left coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the right coordinate y
 * @param[in] color is the display color
 * @return    status code
 *            - 0 success
 *            - 1 fill rect failed
 * @note      left <= 127 && right <= 127 && left < right && top <= 127 && bottom <= 127 && top < bottom
 */
uint8_t ssd1351_fill_rect(ssd1351_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint32_t color);

/**
 * @brief     draw a picture
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] left is the left coordinate x
 * @param[in] top is the left coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the right coordinate y
 * @param[in] *image points to a display image buffer
 * @return    status code
 *            - 0 success
 *            - 1 draw picture failed
 * @note      left <= 127 && right <= 127 && left < right && top <= 127 && bottom <= 127 && top < bottom
 */
uint8_t ssd1351_draw_picture(ssd1351_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint32_t *image);

/**
 * @brief     draw a 16 bits picture
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] left is the left coordinate x
 * @param[in] top is the left coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the right coordinate y
 * @param[in] *image points to a display image buffer
 * @return    status code
 *            - 0 success
 *            - 1 draw picture failed
 * @note      left <= 127 && right <= 127 && left < right && top <= 127 && bottom <= 127 && top < bottom
 */
uint8_t ssd1351_draw_picture_16bits(ssd1351_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint16_t *image);

/**
 * @brief     set the display column address
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] start_address is the start address
 * @param[in] end_address is the end address
 * @return    status code
 *            - 0 success
 *            - 1 set column address failed
 * @note      start_address <= 127 && end_address <= 127 && start_address >= start_address
 */
uint8_t ssd1351_set_column_address(ssd1351_handle_t *handle, uint8_t start_address, uint8_t end_address);

/**
 * @brief     set the row address
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] start_address is the start address
 * @param[in] end_address is the end address
 * @return    status code
 *            - 0 success
 *            - 1 set row address failed
 * @note      start_address <= 127 && end_address <= 127 && start_address >= start_address
 */
uint8_t ssd1351_set_row_address(ssd1351_handle_t *handle, uint8_t start_address, uint8_t end_address);

/**
 * @brief     sent the write ram command
 * @param[in] *handle points to a ssd1351 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 write ram failed
 * @note      none
 */
uint8_t ssd1351_write_ram(ssd1351_handle_t *handle);

/**
 * @brief     sent the read ram command
 * @param[in] *handle points to a ssd1351 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 read ram failed
 * @note      none
 */
uint8_t ssd1351_read_ram(ssd1351_handle_t *handle);

/**
 * @brief     sent the read ram command
 * @param[in] *handle points to a ssd1351 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 read ram failed
 * @note      none
 */
uint8_t ssd1351_read_ram(ssd1351_handle_t *handle);

/**
 * @brief     set the display color depth
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] color_depth is the color depth
 * @return    status code
 *            - 0 success
 *            - 1 set color depth failed
 * @note      none
 */
uint8_t ssd1351_set_color_depth(ssd1351_handle_t *handle, ssd1351_color_depth_t color_depth);

/**
 * @brief     set the address increment
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] increment is the address increment param
 * @return    status code
 *            - 0 success
 *            - 1 set address increment failed
 * @note      none
 */
uint8_t ssd1351_set_address_increment(ssd1351_handle_t *handle, ssd1351_address_increment_t increment);

/**
 * @brief     set the seg0 map
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] seg0_map is the seg0 map
 * @return    status code
 *            - 0 success
 *            - 1 set seg0 map failed
 * @note      none
 */
uint8_t ssd1351_set_seg0_map(ssd1351_handle_t *handle, ssd1351_seg0_mapped_t seg0_map);

/**
 * @brief     set the color sequence
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] color_sequence is the display color sequence
 * @return    status code
 *            - 0 success
 *            - 1 set color sequence failed
 * @note      none
 */
uint8_t ssd1351_set_color_sequence(ssd1351_handle_t *handle, ssd1351_color_sequence_t color_sequence);

/**
 * @brief     set the scan mode
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] mode is the scan mode
 * @return    status code
 *            - 0 success
 *            - 1 set scan mode failed
 * @note      none
 */
uint8_t ssd1351_set_scan_mode(ssd1351_handle_t *handle, ssd1351_scan_mode_t mode);

/**
 * @brief     set the com split odd or even
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set com split odd even failed
 * @note      none
 */
uint8_t ssd1351_set_com_split_odd_even(ssd1351_handle_t *handle, ssd1351_bool_t enable);

/**
 * @brief     set the display start line
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] line is the start line
 * @return    status code
 *            - 0 success
 *            - 1 set display start line failed
 * @note      line <= 127
 */
uint8_t ssd1351_set_display_start_line(ssd1351_handle_t *handle, uint8_t line);

/**
 * @brief     set the display offset
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] offset is the display offset
 * @return    status code
 *            - 0 success
 *            - 1 set display offset failed
 * @note      offset <= 127
 */
uint8_t ssd1351_set_display_offset(ssd1351_handle_t *handle, uint8_t offset);

/**
 * @brief     set the display mode
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] mode is the display mode
 * @return    status code
 *            - 0 success
 *            - 1 set display mode failed
 * @note      none
 */
uint8_t ssd1351_set_display_mode(ssd1351_handle_t *handle, ssd1351_display_mode_t mode);

/**
 * @brief     set the select vdd
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] vdd is the chip vdd
 * @return    status code
 *            - 0 success
 *            - 1 set select vdd failed
 * @note      none
 */
uint8_t ssd1351_set_select_vdd(ssd1351_handle_t *handle, ssd1351_select_vdd_t vdd);

/**
 * @brief     set the interface parallel bits
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] parallel_bits is the interface parallel bits
 * @return    status code
 *            - 0 success
 *            - 1 set parallel bits failed
 * @note      none
 */
uint8_t ssd1351_set_parallel_bits(ssd1351_handle_t *handle, ssd1351_select_parallel_bits_t parallel_bits);

/**
 * @brief     set the sleep mode
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set sleep mode failed
 * @note      none
 */
uint8_t ssd1351_set_sleep_mode(ssd1351_handle_t *handle, ssd1351_bool_t enable);

/**
 * @brief     set the phase period
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] phase1_period is the phase 1 period
 * @param[in] phase2_period is the phase 2 period
 * @return    status code
 *            - 0 success
 *            - 1 set phase period failed
 * @note      2 <= phase1_period <=15 && 3 <= phase1_period <= 15
 */
uint8_t ssd1351_set_phase_period(ssd1351_handle_t *handle, uint8_t phase1_period, uint8_t phase2_period);

/**
 * @brief     set the front clock oscillator frequency
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] div is the clock div
 * @param[in] frequency is the clock frequency
 * @return    status code
 *            - 0 success
 *            - 1 set front clock oscillator frequency failed
 * @note      div < 11 && frequency <= 15
 */
uint8_t ssd1351_set_front_clock_oscillator_frequency(ssd1351_handle_t *handle, uint8_t div, uint8_t frequency);

/**
 * @brief     set the segment low voltage
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] segment is the segment low voltage
 * @return    status code
 *            - 0 success
 *            - 1 set segment low voltage failed
 * @note      none
 */
uint8_t ssd1351_set_segment_low_voltage(ssd1351_handle_t *handle, ssd1351_segment_low_voltage_t segment);

/**
 * @brief     set the gpio pin
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] gpio0 is the gpio 0 pin
 * @param[in] gpio1 is the gpio 1 pin
 * @return    status code
 *            - 0 success
 *            - 1 set gpio failed
 * @note      none
 */
uint8_t ssd1351_set_gpio(ssd1351_handle_t *handle, ssd1351_gpio_pin_t gpio0, ssd1351_gpio_pin_t gpio1);

/**
 * @brief     set the second pre charge period
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] period is the pre charge period
 * @return    status code
 *            - 0 success
 *            - 1 set second pre charge period failed
 * @note      none
 */
uint8_t ssd1351_set_second_pre_charge_period(ssd1351_handle_t *handle, uint8_t period);

/**
 * @brief     set the gray scale pulse width
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] *gamma points to a gamma table buffer
 * @return    status code
 *            - 0 success
 *            - 1 set gray scale pulse width failed
 * @note      none
 */
uint8_t ssd1351_set_gray_scale_pulse_width(ssd1351_handle_t *handle, uint8_t gamma[63]);

/**
 * @brief     use built in linear lut
 * @param[in] *handle points to a ssd1351 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set use built in linear lut failed
 * @note      none
 */
uint8_t ssd1351_set_use_built_in_linear_lut(ssd1351_handle_t *handle);

/**
 * @brief     set the pre charge voltage
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] voltage_level is the pre charge voltage level
 * @return    status code
 *            - 0 success
 *            - 1 set pre charge voltage failed
 * @note      voltage_level <= 0x1F
 */
uint8_t ssd1351_set_pre_charge_voltage(ssd1351_handle_t *handle, uint8_t voltage_level);

/**
 * @brief     set the vcomh voltage
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] voltage_level is the vcomh voltage level
 * @return    status code
 *            - 0 success
 *            - 1 set vcomh voltage failed
 * @note      voltage_level <= 0x07
 */
uint8_t ssd1351_set_vcomh_voltage(ssd1351_handle_t *handle, uint8_t voltage_level);

/**
 * @brief     set the display contrast
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] a is the contrast a
 * @param[in] b is the contrast b
 * @param[in] c is the contrast c
 * @return    status code
 *            - 0 success
 *            - 1 set contrast failed
 * @note      none
 */
uint8_t ssd1351_set_contrast(ssd1351_handle_t *handle, uint8_t a, uint8_t b, uint8_t c);

/**
 * @brief     set the master contrast current
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] current is the master contrast current
 * @return    status code
 *            - 0 success
 *            - 1 set master contrast current failed
 * @note      current <= 0x0F
 */
uint8_t ssd1351_set_master_contrast_current(ssd1351_handle_t *handle, uint8_t current);

/**
 * @brief     set the mux ratio
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] ratio is the mux ratio
 * @return    status code
 *            - 0 success
 *            - 1 set mux ratio failed
 * @note      15 <= ratio <= 127
 */
uint8_t ssd1351_set_mux_ratio(ssd1351_handle_t *handle, uint8_t ratio);

/**
 * @brief     set the command
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] command is the send command
 * @return    status code
 *            - 0 success
 *            - 1 set command failed
 * @note      none
 */
uint8_t ssd1351_set_command(ssd1351_handle_t *handle, ssd1351_command_t command);

/**
 * @brief     set the scroll
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] scroll is the display scroll
 * @param[in] start_row is the scroll start row
 * @param[in] row_len is the scroll row len
 * @param[in] mode is the scroll mode
 * @return    status code
 *            - 0 success
 *            - 1 set scroll failed
 * @note      start_row <= 127 && start_row + row_len <= 128
 */
uint8_t ssd1351_set_scroll(ssd1351_handle_t *handle, int8_t scroll, uint8_t start_row, uint8_t row_len, ssd1351_scroll_mode_t mode);

/**
 * @brief  start the display moving
 * @return status code
 *         - 0 success
 *         - 1 start moving failed
 * @note   none
 */
uint8_t ssd1351_start_moving(ssd1351_handle_t *handle);

/**
 * @brief  stop the display moving
 * @return status code
 *         - 0 success
 *         - 1 stop moving failed
 * @note   none
 */
uint8_t ssd1351_stop_moving(ssd1351_handle_t *handle);

/**
 * @}
 */

/**
 * @defgroup ssd1351_extend_driver ssd1351 extend driver function
 * @brief    ssd1351 extend driver modules
 * @ingroup  ssd1351_driver
 * @{
 */

/**
 * @brief     write the command
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] cmd is the send command
 * @return    status code
 *            - 0 success
 *            - 1 write command failed
 * @note      none
 */
uint8_t ssd1351_write_cmd(ssd1351_handle_t *handle, uint8_t cmd);

/**
 * @brief     write the data
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] data is the send data
 * @return    status code
 *            - 0 success
 *            - 1 write data failed
 * @note      none
 */
uint8_t ssd1351_write_data(ssd1351_handle_t *handle, uint8_t data);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
