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
 * @file      driver_ssd1351.c
 * @brief     driver ssd1351 source file
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

#include "driver_ssd1351.h"
#include "driver_ssd1351_font.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Solomon Systech SSD1351"        /**< chip name */
#define MANUFACTURER_NAME         "Solomon Systech"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.4f                             /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        2.6f                             /**< chip max supply voltage */
#define MAX_CURRENT               0.70f                            /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                           /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                            /**< chip max operating temperature */
#define DRIVER_VERSION            2000                             /**< driver version */

/**
 * @brief command data type definition
 */
#define SSD1351_CMD         0        /**< command type */
#define SSD1351_DATA        1        /**< data type */

/**
 * @brief command definition
 */
#define SSD1351_CMD_SET_COLUMN_ADDRESS                  0x15        /**< set column address command */ 
#define SSD1351_CMD_SET_ROW_ADDRESS                     0x75        /**< set row address command */
#define SSD1351_CMD_WRITE_RAM                           0x5C        /**< write ram command */
#define SSD1351_CMD_READ_RAM                            0x5D        /**< read ram command */
#define SSD1351_CMD_SET_REMAP_COLOR_DEPTH               0xA0        /**< set remap color depth command */
#define SSD1351_CMD_SET_DISPLAY_START_LINE              0xA1        /**< set display start line command */
#define SSD1351_CMD_SET_DISPLAY_OFFSET                  0xA2        /**< set display offset command */
#define SSD1351_CMD_SET_DISPLAY_ALL_OFF                 0xA4        /**< set display all off command */
#define SSD1351_CMD_SET_DISPLAY_ALL_ON                  0xA5        /**< set display all on command */
#define SSD1351_CMD_SET_DISPLAY_NORMAL                  0xA6        /**< set display normal command */
#define SSD1351_CMD_SET_DISPLAY_INVERSE                 0xA7        /**< set display inverse command */
#define SSD1351_CMD_FUNCTION_SELECTION                  0xAB        /**< set function selection command */
#define SSD1351_CMD_SET_SLEEP_MODE_ON                   0xAE        /**< set sleep mode on command */
#define SSD1351_CMD_SET_SLEEP_MODE_OFF                  0xAF        /**< set sleep mode off command */
#define SSD1351_CMD_SET_RESET_PRE_CHARGE_PERIOD         0xB1        /**< set reset pre charge period command */
#define SSD1351_CMD_FRONT_CLOCK_DIVIDER_OSC_FREQ        0xB3        /**< front clock divider osc freq command */
#define SSD1351_CMD_SET_SEGMENT_LOW_VOLTAGE             0xB4        /**< set segment low voltage command */
#define SSD1351_CMD_SET_GPIO                            0xB5        /**< set gpio command */
#define SSD1351_CMD_SET_SECOND_PRE_CHARGE_PERIOD        0xB6        /**< set second pre charge period command */
#define SSD1351_CMD_GRAY_SCALE_PULSE_WIDTH              0xB8        /**< set gray scale pulse width command */
#define SSD1351_CMD_USE_BUILT_IN_LINEAR_LUT             0xB9        /**< use built in linear lut command */
#define SSD1351_CMD_SET_PRE_CHARGE_VOLTAGE              0xBB        /**< set pre charge voltage command */
#define SSD1351_CMD_SET_VCOMH_VOLTAGE                   0xBE        /**< set vcomh voltage command */
#define SSD1351_CMD_SET_CONTRAST                        0xC1        /**< set contrast command */
#define SSD1351_CMD_MASTER_CONTRAST_CONTROL             0xC7        /**< set master contrast control command */
#define SSD1351_CMD_SET_MUX_RATIO                       0xCA        /**< set mux ratio command */
#define SSD1351_CMD_SET_COMMAND_LOCK                    0xFD        /**< set command lock command */
#define SSD1351_CMD_HORIZONTAL_SCROLL                   0x96        /**< set horizontal scroll command */
#define SSD1351_CMD_STOP_MOVING                         0x9E        /**< stop moving command */
#define SSD1351_CMD_START_MOVING                        0x9F        /**< start moving command */

/**
 * @brief     write one byte
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] data is the written data
 * @param[in] cmd is the data type
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t _ssd1351_write_byte(ssd1351_handle_t *handle, uint8_t data, uint8_t cmd)
{
    volatile uint8_t res;
    
    res = handle->cmd_data_gpio_write(cmd);        /* write gpio */
    if (res)                                       /* check result */
    {
        return 1;                                  /* return error */
    }
    res = handle->spi_write_cmd(&data, 1);         /* write data command */
    if (res)                                       /* check result */
    {
        return 1;                                  /* return error */
    }
    else
    {
        return 0;                                  /* success return 0 */
    }
}

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
uint8_t ssd1351_set_column_address(ssd1351_handle_t *handle, uint8_t start_address, uint8_t end_address)
{
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if ((start_address > 127) || (end_address > 127))                                    /* check range */
    {
        handle->debug_print("ssd1351: address is invalid.\n");                           /* address is invalid */
        
        return 1;                                                                        /* return error */
    }
    if (start_address >= end_address)                                                    /* check range */
    {
        handle->debug_print("ssd1351: start_address >= end_address.\n");                 /* start_address <= end_address */
        
        return 1;                                                                        /* return error */
    }

    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_COLUMN_ADDRESS, SSD1351_CMD))        /* write column address command */
    {
        handle->debug_print("ssd1351: write command failed.\n");                         /* write command failed */
        
        return 1;                                                                        /* return error */
    }
    if (_ssd1351_write_byte(handle, start_address, SSD1351_DATA))                        /* set start address */
    {
        handle->debug_print("ssd1351: write start address failed.\n");                   /* write start address failed */
        
        return 1;                                                                        /* return error */
    }
    if (_ssd1351_write_byte(handle, end_address, SSD1351_DATA))                          /* set end address */
    {
        handle->debug_print("ssd1351: write end address failed.\n");                     /* write end address failed */
        
        return 1;                                                                        /* return error */
    }

    return 0;                                                                            /* success return 0 */
}

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
uint8_t ssd1351_set_row_address(ssd1351_handle_t *handle, uint8_t start_address, uint8_t end_address)
{
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    if ((start_address > 127) || (end_address > 127))                                /* check range */
    {
        handle->debug_print("ssd1351: address is invalid.\n");                       /* address is invalid */
        
        return 1;                                                                    /* return error */
    }
    if (start_address >= end_address)                                                /* check range */
    {
        handle->debug_print("ssd1351: start_address >= end_address.\n");             /* start_address >= end_address */
        
        return 1;                                                                    /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_ROW_ADDRESS, SSD1351_CMD))       /* set row address */
    {
        handle->debug_print("ssd1351: write command failed.\n");                     /* write command failed */
        
        return 1;                                                                    /* return error */
    }
    if (_ssd1351_write_byte(handle, start_address, SSD1351_DATA))                    /* set start address */
    {
        handle->debug_print("ssd1351: write start address failed.\n");               /* write start address failed */
        
        return 1;                                                                    /* return error */
    }
    if (_ssd1351_write_byte(handle, end_address, SSD1351_DATA))                      /* set end address */
    {
        handle->debug_print("ssd1351: write end address failed.\n");                 /* write end address failed */
        
        return 1;                                                                    /* return error */
    }

    return 0;                                                                        /* success return 0 */
}

/**
 * @brief     sent the write ram command
 * @param[in] *handle points to a ssd1351 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 write ram failed
 * @note      none
 */
uint8_t ssd1351_write_ram(ssd1351_handle_t *handle)
{
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_WRITE_RAM, SSD1351_CMD))        /* set write ram */
    {
        handle->debug_print("ssd1351: write ram failed.\n");                    /* write ram failed */
        
        return 1;                                                               /* return error */
    }
    
    return 0;                                                                   /* success return 0 */
}

/**
 * @brief     sent the read ram command
 * @param[in] *handle points to a ssd1351 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 read ram failed
 * @note      none
 */
uint8_t ssd1351_read_ram(ssd1351_handle_t *handle)
{
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
  
    if (_ssd1351_write_byte(handle, SSD1351_CMD_READ_RAM, SSD1351_CMD))        /* set read ram */
    {
        handle->debug_print("ssd1351: read command failed.\n");                /* return error */
        
        return 1;
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief     set the display color depth
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] color_depth is the color depth
 * @return    status code
 *            - 0 success
 *            - 1 set color depth failed
 * @note      none
 */
uint8_t ssd1351_set_color_depth(ssd1351_handle_t *handle, ssd1351_color_depth_t color_depth)
{
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    handle->conf_1 &= ~ (3 << 6);                                                           /* clear conf */
    handle->conf_1 |= color_depth << 6;                                                     /* set conf */
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_REMAP_COLOR_DEPTH, SSD1351_CMD))        /* set color depth */
    {
        handle->debug_print("ssd1351: write color depth failed.\n");                        /* write color depth failed */
        
        return 1;                                                                           /* return error */
    }
    if (_ssd1351_write_byte(handle, handle->conf_1, SSD1351_DATA))                          /* set conf */
    {
        handle->debug_print("ssd1351: write color depth failed.\n");                        /* write color depth failed */
        
        return 1;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     set the address increment
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] increment is the address increment param
 * @return    status code
 *            - 0 success
 *            - 1 set address increment failed
 * @note      none
 */
uint8_t ssd1351_set_address_increment(ssd1351_handle_t *handle, ssd1351_address_increment_t increment)
{
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    handle->conf_1 &= ~ (1 << 0);                                                           /* clear conf */
    handle->conf_1 |= increment << 0;                                                       /* set conf */
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_REMAP_COLOR_DEPTH, SSD1351_CMD))        /* set address increment */
    {
        handle->debug_print("ssd1351: write address increment failed.\n");                  /* write address increment failed */
        
        return 1;                                                                           /* return error */
    }
    if (_ssd1351_write_byte(handle, handle->conf_1, SSD1351_DATA))                          /* set conf */
    {
        handle->debug_print("ssd1351: write address increment failed.\n");                  /* write address increment failed */
        
        return 1;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     set the seg0 map
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] seg0_map is the seg0 map
 * @return    status code
 *            - 0 success
 *            - 1 set seg0 map failed
 * @note      none
 */
uint8_t ssd1351_set_seg0_map(ssd1351_handle_t *handle, ssd1351_seg0_mapped_t seg0_map)
{
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    handle->conf_1 &= ~ (1 << 1);                                                           /* clear conf */
    handle->conf_1 |= seg0_map << 1;                                                        /* set conf */
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_REMAP_COLOR_DEPTH, SSD1351_CMD))        /* set seg0 map */
    {
        handle->debug_print("ssd1351: wrire seg0 map failed.\n");                           /* write seg0 map failed */
        
        return 1;                                                                           /* return error */
    }
    if (_ssd1351_write_byte(handle, handle->conf_1, SSD1351_DATA))                          /* set conf */
    {
        handle->debug_print("ssd1351: write seg0 map failed.\n");                           /* write seg0 map failed */
        
        return 1;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     set the color sequence
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] color_sequence is the display color sequence
 * @return    status code
 *            - 0 success
 *            - 1 set color sequence failed
 * @note      none
 */
uint8_t ssd1351_set_color_sequence(ssd1351_handle_t *handle, ssd1351_color_sequence_t color_sequence)
{
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    handle->conf_1 &= ~ (1 << 2);                                                           /* clear conf */
    handle->conf_1 |= color_sequence << 2;                                                  /* set conf */
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_REMAP_COLOR_DEPTH, SSD1351_CMD))        /* set color sequence */
    {
        handle->debug_print("ssd1351: wrire color sequence failed.\n");                     /* write color sequence failed */
        
        return 1;                                                                           /* return error */
    }
    if (_ssd1351_write_byte(handle, handle->conf_1, SSD1351_DATA))                          /* set conf */
    {
        handle->debug_print("ssd1351: write color sequence failed.\n");                     /* write color sequence failed */
        
        return 1;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     set the scan mode
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] mode is the scan mode
 * @return    status code
 *            - 0 success
 *            - 1 set scan mode failed
 * @note      none
 */
uint8_t ssd1351_set_scan_mode(ssd1351_handle_t *handle, ssd1351_scan_mode_t mode)
{
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    handle->conf_1 &= ~ (1 << 4);                                                           /* clear conf */
    handle->conf_1 |= mode << 4;                                                            /* set conf */
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_REMAP_COLOR_DEPTH, SSD1351_CMD))        /* set scan mode */
    {
        handle->debug_print("ssd1351: wrire scan mode failed.\n");                          /* write scan mode failed */
        
        return 1;                                                                           /* return error */
    }
    if (_ssd1351_write_byte(handle, handle->conf_1, SSD1351_DATA))                          /* set conf */
    {
        handle->debug_print("ssd1351: write scan mode failed.\n");                          /* write scan mode failed */
        
        return 1;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     set the com split odd or even
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set com split odd even failed
 * @note      none
 */
uint8_t ssd1351_set_com_split_odd_even(ssd1351_handle_t *handle, ssd1351_bool_t enable)
{
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    handle->conf_1 &= ~ (1 << 5);                                                           /* clear conf */
    handle->conf_1 |= enable << 5;                                                          /* set conf */
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_REMAP_COLOR_DEPTH, SSD1351_CMD))        /* set com split odd even */
    {
        handle->debug_print("ssd1351: wrire com split odd even failed.\n");                 /* write com split odd even failed */
        
        return 1;                                                                           /* return error */
    }
    if (_ssd1351_write_byte(handle, handle->conf_1, SSD1351_DATA))                          /* set conf */
    {
        handle->debug_print("ssd1351: write com split odd even failed.\n");                 /* write com split odd even failed */
        
        return 1;                                                                           /* return error */
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     set the display start line
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] line is the start line
 * @return    status code
 *            - 0 success
 *            - 1 set display start line failed
 * @note      line <= 127
 */
uint8_t ssd1351_set_display_start_line(ssd1351_handle_t *handle, uint8_t line)
{
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    if (line > 127)                                                                          /* check line */
    {
        handle->debug_print("ssd1351: line is over 127.\n");                                 /* line is over 127 */
        
        return 1;                                                                            /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_DISPLAY_START_LINE, SSD1351_CMD))        /* set display start line */
    {
        handle->debug_print("ssd1351: write start line failed.\n");                          /* write start line failed */
        
        return 1;                                                                            /* return error */
    }
    if (_ssd1351_write_byte(handle, line, SSD1351_DATA))                                     /* set display start line */
    {
        handle->debug_print("ssd1351: write data failed.\n");                                /* write data failed */
        
        return 1;                                                                            /* return error */
    }

    return 0;                                                                                /* success return 0 */
}

/**
 * @brief     set the display offset
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] offset is the display offset
 * @return    status code
 *            - 0 success
 *            - 1 set display offset failed
 * @note      offset <= 127
 */
uint8_t ssd1351_set_display_offset(ssd1351_handle_t *handle, uint8_t offset)
{
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    if (offset > 127)                                                                    /* check offset */
    {
        handle->debug_print("ssd1351: offset is over 127.\n");                           /* offset is over 127 */
        
        return 1;                                                                        /* return error */
    }
  
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_DISPLAY_OFFSET, SSD1351_CMD))        /* set display offset */ 
    {
        handle->debug_print("ssd1351: write display offset failed.\n");                  /* write display offset failed */
        
        return 1;                                                                        /* return error */
    }
    if (_ssd1351_write_byte(handle, offset, SSD1351_DATA))                               /* set display offset */
    {
        handle->debug_print("ssd1351: write data failed.\n");                            /* write data failed */
        
        return 1;                                                                        /* return error */
    }

    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     set the display mode
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] mode is the display mode
 * @return    status code
 *            - 0 success
 *            - 1 set display mode failed
 * @note      none
 */
uint8_t ssd1351_set_display_mode(ssd1351_handle_t *handle, ssd1351_display_mode_t mode)
{
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }

    if (_ssd1351_write_byte(handle, mode, SSD1351_CMD))                      /* set display mode */
    {
        handle->debug_print("ssd1351: write display mode failed.\n");        /* write display mode failed */
        
        return 1;                                                            /* return error */
    }
    
    return 0;                                                                /* success return 0 */
}

/**
 * @brief     set the select vdd
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] vdd is the chip vdd
 * @return    status code
 *            - 0 success
 *            - 1 set select vdd failed
 * @note      none
 */
uint8_t ssd1351_set_select_vdd(ssd1351_handle_t *handle, ssd1351_select_vdd_t vdd)
{
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }

    handle->conf_2 &= ~ (1 << 0);                                                        /* clear config */
    handle->conf_2 |= vdd << 0;                                                          /* set vdd */
    if (_ssd1351_write_byte(handle, SSD1351_CMD_FUNCTION_SELECTION, SSD1351_CMD))        /* set select vdd */
    {
        handle->debug_print("ssd1351: write function selection failed.\n");              /* write function selection failed */
        
        return 1;                                                                        /* return error */
    }
    if (_ssd1351_write_byte(handle, handle->conf_2, SSD1351_DATA))                       /* set select vdd */
    {
        handle->debug_print("ssd1351: write function selection failed.\n");              /* write function selection failed */
        
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     set the interface parallel bits
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] parallel_bits is the interface parallel bits
 * @return    status code
 *            - 0 success
 *            - 1 set parallel bits failed
 * @note      none
 */
uint8_t ssd1351_set_parallel_bits(ssd1351_handle_t *handle, ssd1351_select_parallel_bits_t parallel_bits)
{
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }

    handle->conf_2 &= ~ (3 << 6);                                                        /* clear config */
    handle->conf_2 |= parallel_bits << 6;                                                /* set bits */
    if (_ssd1351_write_byte(handle, SSD1351_CMD_FUNCTION_SELECTION, SSD1351_CMD))        /* set parallel bits */
    {
        handle->debug_print("ssd1351: write parallel bits failed.\n");                   /* write parallel bits failed */
        
        return 1;                                                                        /* return error */
    }
    if (_ssd1351_write_byte(handle, handle->conf_2, SSD1351_DATA))                       /* set sparallel bits */
    {
        handle->debug_print("ssd1351: write parallel bits failed.\n");                   /* write parallel bits failed */
        
        return 1;                                                                        /* return error */
    }
    
    return 0;                                                                            /* success return 0 */
}

/**
 * @brief     set the sleep mode
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set sleep mode failed
 * @note      none
 */
uint8_t ssd1351_set_sleep_mode(ssd1351_handle_t *handle, ssd1351_bool_t enable)
{
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    if (enable == SSD1351_BOOL_TRUE)                                                         /* if sleep mode on */
    {
        if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_SLEEP_MODE_ON, SSD1351_CMD))         /* set sleep mode on */
        {
            handle->debug_print("ssd1351: set sleep mode on failed.\n");                     /* set sleep mode on failed */
            
            return 1;                                                                        /* return error */
        }
        
        return 0;                                                                            /* success return 0 */
    }
    else if (enable == SSD1351_BOOL_FALSE)                                                   /* if sleep mode off */
    {
        if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_SLEEP_MODE_OFF, SSD1351_CMD))        /* set sleep mode off */ 
        {
            handle->debug_print("ssd1351: set sleep mode off failed.\n");                    /* set sleep mode off failed */
            
            return 1;                                                                        /* return error */
        }
        
        return 0;                                                                            /* success return 0 */
    }
    else
    {
        handle->debug_print("ssd1351: param is invalid.\n");                                 /* param is invalid */
        
        return 1;                                                                            /* return error */
    }
}

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
uint8_t ssd1351_set_phase_period(ssd1351_handle_t *handle, uint8_t phase1_period, uint8_t phase2_period)
{
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    if (phase1_period < 2)                                                                        /* check phase1 */
    {
        handle->debug_print("ssd1351: phase1_period is less than 2.\n");                          /* phase1_period is less than 2 */
        
        return 1;                                                                                 /* return error */
    }
    if (phase1_period > 15)                                                                       /* check phase1 */
    {
        handle->debug_print("ssd1351: phase1_period is over 15.\n");                              /* phase1_period is over 15 */
        
        return 1;                                                                                 /* return error */
    }
    if (phase2_period < 3)                                                                        /* check phase2 */
    {
        handle->debug_print("ssd1351: phase2_period is less than 3.\n");                          /* phase2_period is less than 3 */
        
        return 1;                                                                                 /* return error */
    }
    if (phase2_period > 15)                                                                       /* check phase2 */
    {
        handle->debug_print("ssd1351: phase2_period is over 15.\n");                              /* phase2_period is over 15 */
        
        return 1;                                                                                 /* return error */
    }

    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_RESET_PRE_CHARGE_PERIOD, SSD1351_CMD))        /* set reset pre charge period */
    {
        handle->debug_print("ssd1351: write reset pre charge period failed.\n");                  /* write reset pre charge period failed */
        
        return 1;                                                                                 /* return error */
    }
    if (_ssd1351_write_byte(handle, (phase2_period << 4) | phase1_period, SSD1351_DATA))          /* set phase period */
    {
        handle->debug_print("ssd1351: write reset pre charge period failed.\n");                  /* write reset pre charge period failed */
        
        return 1;                                                                                 /* return error */
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t ssd1351_set_front_clock_oscillator_frequency(ssd1351_handle_t *handle, uint8_t div, uint8_t frequency)
{
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    if (div >= 11)                                                                                 /* check div */
    {
        handle->debug_print("ssd1351: div is over 11.\n");                                         /* div is over 11 */
        
        return 1;                                                                                  /* return error */
    }
    if (frequency > 15)                                                                            /* check frequency */
    {
        handle->debug_print("ssd1351: frequency is over 15.\n");                                   /* frequency is over 15 */
        
        return 1;                                                                                  /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_FRONT_CLOCK_DIVIDER_OSC_FREQ, SSD1351_CMD))        /* set front clock divider osc freq */
    {
        handle->debug_print("ssd1351: write front clock divider osc freq failed.\n");              /* write front clock divider osc freq failed */
        
        return 1;                                                                                  /* return error */
    }
    if (_ssd1351_write_byte(handle, (frequency << 4) | div, SSD1351_DATA))                         /* set front clock divider osc freq */
    {
        handle->debug_print("ssd1351: write front clock divider osc freq failed.\n");              /* write front clock divider osc freq failed */
        
        return 1;                                                                                  /* return error */
    }
    
    return 0;                                                                                      /* success return 0 */
}

/**
 * @brief     set the segment low voltage
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] segment is the segment low voltage
 * @return    status code
 *            - 0 success
 *            - 1 set segment low voltage failed
 * @note      none
 */
uint8_t ssd1351_set_segment_low_voltage(ssd1351_handle_t *handle, ssd1351_segment_low_voltage_t segment)
{
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_SEGMENT_LOW_VOLTAGE, SSD1351_CMD))        /* set segment low voltage */
    {
        handle->debug_print("ssd1351: write segment low voltage failed.\n");                  /* write segment low voltage failed */
        
        return 1;                                                                             /* return error */
    }
    if (_ssd1351_write_byte(handle, 0xA0|segment, SSD1351_DATA))                              /* set segment low voltage */
    {
        handle->debug_print("ssd1351: write segment low voltage failed.\n");                  /* write segment low voltage failed */
        
        return 1;                                                                             /* return error */
    }
    if (_ssd1351_write_byte(handle, 0xB5, SSD1351_DATA))                                      /* set segment low voltage */
    {
        handle->debug_print("ssd1351: write segment low voltage failed.\n");                  /* write segment low voltage failed */
        
        return 1;                                                                             /* return error */
    }
    if (_ssd1351_write_byte(handle, 0x55, SSD1351_DATA))                                      /* set segment low voltage */
    {
        handle->debug_print("ssd1351: write segment low voltage failed.\n");                  /* write segment low voltage failed */
        
        return 1;                                                                             /* return error */
    }
        
    return 0;                                                                                 /* success return 0 */
}

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
uint8_t ssd1351_set_gpio(ssd1351_handle_t *handle, ssd1351_gpio_pin_t gpio0, ssd1351_gpio_pin_t gpio1)
{
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_GPIO, SSD1351_CMD))        /* set gpio */
    {
        handle->debug_print("ssd1351: write gpio failed.\n");                  /* write gpio failed */
        
        return 1;                                                              /* return error */
    }
    if (_ssd1351_write_byte(handle, (gpio1<<2)|gpio0, SSD1351_DATA))           /* set gpio */
    {
        handle->debug_print("ssd1351: write gpio failed.\n");                  /* write gpio failed */
        
        return 1;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief     set the second pre charge period
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] period is the pre charge period
 * @return    status code
 *            - 0 success
 *            - 1 set second pre charge period failed
 * @note      none
 */
uint8_t ssd1351_set_second_pre_charge_period(ssd1351_handle_t *handle, uint8_t period)
{
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    if (period > 15)                                                                               /* check period */
    {
        handle->debug_print("ssd1351: period is over 15.\n");                                      /* period is over 15 */
        
        return 1;                                                                                  /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_SECOND_PRE_CHARGE_PERIOD, SSD1351_CMD))        /* set second pre charge period */
    {
        handle->debug_print("ssd1351: write second pre charge period failed.\n");                  /* write second pre charge period failed */
        
        return 1;                                                                                  /* return error */
    }
    if (_ssd1351_write_byte(handle, period, SSD1351_DATA))                                         /* set second pre charge period */
    {
        handle->debug_print("ssd1351: write second pre charge period failed.\n");                  /* write second pre charge period failed */
        
        return 1;                                                                                  /* return error */
    }
    
    return 0;                                                                                      /* success return 0 */
}

/**
 * @brief     set the gray scale pulse width
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] *gamma points to a gamma table buffer
 * @return    status code
 *            - 0 success
 *            - 1 set gray scale pulse width failed
 * @note      none
 */
uint8_t ssd1351_set_gray_scale_pulse_width(ssd1351_handle_t *handle, uint8_t gamma[63])
{
    volatile uint8_t i;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_GRAY_SCALE_PULSE_WIDTH, SSD1351_CMD))        /* set gray scale pulse width */
    {
        handle->debug_print("ssd1351: write gray scale pulse width failed.\n");              /* write gray scale pulse width failed */
        
        return 1;                                                                            /* return error */
    }
    for (i = 0; i < 63; i++)                                                                 /* 64 times */
    {
        if (_ssd1351_write_byte(handle, gamma[i], SSD1351_DATA))                             /* set gamma */
        {
            handle->debug_print("ssd1351: write gamma failed.\n");                           /* write gamma failed */
            
            return 1;                                                                        /* return error */
        }
    }
    
    return 0;                                                                                /* success return 0 */
}

/**
 * @brief     use built in linear lut
 * @param[in] *handle points to a ssd1351 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set use built in linear lut failed
 * @note      none
 */
uint8_t ssd1351_set_use_built_in_linear_lut(ssd1351_handle_t *handle)
{
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_USE_BUILT_IN_LINEAR_LUT, SSD1351_CMD))        /* set use built in linear lut */
    {
        handle->debug_print("ssd1351: use built in linear lut failed.\n");                    /* write use built in linear lut failed */
        
        return 1;                                                                             /* return error */
    }
    
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     set the pre charge voltage
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] voltage_level is the pre charge voltage level
 * @return    status code
 *            - 0 success
 *            - 1 set pre charge voltage failed
 * @note      voltage_level <= 0x1F
 */
uint8_t ssd1351_set_pre_charge_voltage(ssd1351_handle_t *handle, uint8_t voltage_level)
{
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    if (voltage_level > 0x1F)                                                                /* check voltage level */
    {
        handle->debug_print("ssd1351: voltage level is over 0x1F.\n");                       /* voltage level is over 0x1F */
        
        return 1;                                                                            /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_PRE_CHARGE_VOLTAGE, SSD1351_CMD))        /* set pre charge voltage */
    {
        handle->debug_print("ssd1351: write pre charge voltage failed.\n");                  /* write pre charge voltage failed */
        
        return 1;                                                                            /* return error */
    }
    if (_ssd1351_write_byte(handle, voltage_level, SSD1351_DATA))                            /* set pre charge voltage */
    {
        handle->debug_print("ssd1351: write pre charge voltage failed.\n");                  /* write pre charge voltage failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

/**
 * @brief     set the vcomh voltage
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] voltage_level is the vcomh voltage level
 * @return    status code
 *            - 0 success
 *            - 1 set vcomh voltage failed
 * @note      voltage_level <= 0x07
 */
uint8_t ssd1351_set_vcomh_voltage(ssd1351_handle_t *handle, uint8_t voltage_level)
{
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (voltage_level > 0x07)                                                           /* check voltage level */
    {
        handle->debug_print("ssd1351: voltage level is over 0x07.\n");                  /* voltage level is over 0x07 */
        
        return 1;                                                                       /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_VCOMH_VOLTAGE, SSD1351_CMD))        /* set vcomh voltage */
    {
        handle->debug_print("ssd1351: write vcomh voltage failed.\n");                  /* write vcomh voltage failed */
        
        return 1;                                                                       /* return error */
    }
    if (_ssd1351_write_byte(handle, voltage_level, SSD1351_DATA))                       /* set vcomh voltage */
    {
        handle->debug_print("ssd1351: write vcomh voltage failed.\n");                  /* write vcomh voltage failed */
        
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

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
uint8_t ssd1351_set_contrast(ssd1351_handle_t *handle, uint8_t a, uint8_t b, uint8_t c)
{
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_CONTRAST, SSD1351_CMD))        /* set contrast */
    {
        handle->debug_print("ssd1351: write contrast failed.\n");                  /* write contrast failed */
        
        return 1;                                                                  /* return error */
    }
    if (_ssd1351_write_byte(handle, a, SSD1351_DATA))                              /* set a */
    {
        handle->debug_print("ssd1351: write a failed.\n");                         /* write a failed */
        
        return 1;                                                                  /* return error */
    }
    if (_ssd1351_write_byte(handle, b, SSD1351_DATA))                              /* set b */
    {
        handle->debug_print("ssd1351: write b failed.\n");                         /* write b failed */
        
        return 1;                                                                  /* return error */
    }
    if (_ssd1351_write_byte(handle, c, SSD1351_DATA))                              /* set c */
    {
        handle->debug_print("ssd1351: write c failed.\n");                         /* write c failed */
        
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief     set the master contrast current
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] current is the master contrast current
 * @return    status code
 *            - 0 success
 *            - 1 set master contrast current failed
 * @note      current <= 0x0F
 */
uint8_t ssd1351_set_master_contrast_current(ssd1351_handle_t *handle, uint8_t current)
{
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    if (current > 0x0F)                                                                       /* check master contrast current */
    {
        handle->debug_print("ssd1351: master contrast current is over 0x0F.\n");              /* master contrast current is over 0x0F */
        
        return 1;                                                                             /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_MASTER_CONTRAST_CONTROL, SSD1351_CMD))        /* set master contrast current */
    {
        handle->debug_print("ssd1351: write master contrast current failed.\n");              /* write master contrast current failed */
        
        return 1;                                                                             /* return error */
    }
    if (_ssd1351_write_byte(handle, current, SSD1351_DATA))                                   /* set master contrast current */
    {
        handle->debug_print("ssd1351: write master contrast current failed.\n");              /* write master contrast current failed */
        
        return 1;                                                                             /* return error */
    }
        
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     set the mux ratio
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] ratio is the mux ratio
 * @return    status code
 *            - 0 success
 *            - 1 set mux ratio failed
 * @note      15 <= ratio <= 127
 */
uint8_t ssd1351_set_mux_ratio(ssd1351_handle_t *handle, uint8_t ratio)
{
    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }
    if (ratio < 15)                                                                           /* check ratio */
    {
        handle->debug_print("ssd1351: ratio < 15.\n");                                        /* ratio < 15 */
        
        return 1;                                                                             /* return error */
    }
    if (ratio > 127)                                                                          /* check ratio */
    {
        handle->debug_print("ssd1351: ratio > 127.\n");                                       /* ratio > 127 */
        
        return 1;                                                                             /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_MUX_RATIO, SSD1351_CMD))                  /* set mux ratio */
    {
        handle->debug_print("ssd1351: write mux ratio failed.\n");                            /* write mux ratio failed */
        
        return 1;                                                                             /* return error */
    }
    if (_ssd1351_write_byte(handle, ratio, SSD1351_DATA))                                     /* set mux ratio */
    {
        handle->debug_print("ssd1351: write mux ratio failed.\n");                            /* write mux ratio failed */
        
        return 1;                                                                             /* return error */
    }
        
    return 0;                                                                                 /* success return 0 */
}

/**
 * @brief     set the command
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] command is the send command
 * @return    status code
 *            - 0 success
 *            - 1 set command failed
 * @note      none
 */
uint8_t ssd1351_set_command(ssd1351_handle_t *handle, ssd1351_command_t command)
{
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_COMMAND_LOCK, SSD1351_CMD))        /* set command lock */
    {
        handle->debug_print("ssd1351: write command lock failed.\n");                  /* write command lock failed */
        
        return 1;                                                                      /* return error */
    }
    if (_ssd1351_write_byte(handle, command, SSD1351_DATA))                            /* set command lock */
    {
        handle->debug_print("ssd1351: write command lock failed.\n");                  /* write command lock failed */
        
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t ssd1351_set_scroll(ssd1351_handle_t *handle, int8_t scroll, uint8_t start_row, uint8_t row_len, ssd1351_scroll_mode_t mode)
{
    if (handle == NULL)                                                                 /* check handle */
    {
        return 2;                                                                       /* return error */
    }
    if (handle->inited != 1)                                                            /* check handle initialization */
    {
        return 3;                                                                       /* return error */
    }
    if (start_row > 127)                                                                /* check start row */
    {
        handle->debug_print("ssd1351: start row is over 127.\n");                       /* start row is over 127 */
        
        return 1;                                                                       /* return error */
    }
    if ((start_row + row_len) > 128)                                                    /* check row len */
    {
        handle->debug_print("ssd1351: start_row+row_len is over 128.\n");               /* start_row + row_len is over 128 */
        
        return 1;                                                                       /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_STOP_MOVING, SSD1351_CMD))              /* set stop moving */
    {
        handle->debug_print("ssd1351: write stop moving failed.\n");                    /* write stop moving failed */
        
        return 1;                                                                       /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_HORIZONTAL_SCROLL, SSD1351_CMD))        /* set horizontal scroll */
    {
        handle->debug_print("ssd1351: write horizontal scroll failed.\n");              /* write horizontal scroll failed */
        
        return 1;                                                                       /* return error */
    }
    if (_ssd1351_write_byte(handle, scroll, SSD1351_DATA))                              /* set scroll */
    {
        handle->debug_print("ssd1351: write scroll failed.\n");                         /* write scroll failed */
        
        return 1;                                                                       /* return error */
    }
    if (_ssd1351_write_byte(handle, start_row, SSD1351_DATA))                           /* set start row */
    {
        handle->debug_print("ssd1351: write start row failed.\n");                      /* write start row failed */
        
        return 1;                                                                       /* return error */
    }
    if (_ssd1351_write_byte(handle, row_len, SSD1351_DATA))                             /* start row len */
    {
        handle->debug_print("ssd1351: write row len failed.\n");                        /* write row len failed */
        
        return 1;                                                                       /* return error */
    }
    if (_ssd1351_write_byte(handle, 0x00, SSD1351_DATA))                                /* set 0x00 */
    {
        handle->debug_print("ssd1351: write data failed.\n");                           /* write data failed */
        
        return 1;                                                                       /* return error */
    }
    if (_ssd1351_write_byte(handle, mode, SSD1351_DATA))                                /* set mode */
    {
        handle->debug_print("ssd1351: write mode failed.\n");                           /* write mode failed */
        
        return 1;                                                                       /* return error */
    }
    
    return 0;                                                                           /* success return 0 */
}

/**
 * @brief     clear the display
 * @param[in] *handle points to a ssd1351 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear failed
 * @note      none
 */
uint8_t ssd1351_clear(ssd1351_handle_t *handle)
{
    volatile uint8_t i, j;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_ROW_ADDRESS, SSD1351_CMD))              /* set row address */
    {
        handle->debug_print("ssd1351: write command failed.\n");                            /* write command failed */
        
        return 1;                                                                           /* return error */
    }
    if (_ssd1351_write_byte(handle, 0, SSD1351_DATA))                                       /* set start address */
    {
        handle->debug_print("ssd1351: write start address failed.\n");                      /* write start address failed */
        
        return 1;                                                                           /* return error */
    }
    if (_ssd1351_write_byte(handle, 127, SSD1351_DATA))                                     /* set end address */
    {
        handle->debug_print("ssd1351: write end address failed.\n");                        /* write end address failed */
        
        return 1;                                                                           /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_COLUMN_ADDRESS, SSD1351_CMD))           /* set column address */
    {
        handle->debug_print("ssd1351: write command failed.\n");                            /* write command failed */
        
        return 1;                                                                           /* return error */
    }
    if (_ssd1351_write_byte(handle, 0, SSD1351_DATA))                                       /* set start address */
    {
        handle->debug_print("ssd1351: write start address failed.\n");                      /* write start address failed */
        
        return 1;                                                                           /* return error */
    }
    if (_ssd1351_write_byte(handle, 127, SSD1351_DATA))                                     /* set end address */
    {
        handle->debug_print("ssd1351: write end address failed.\n");                        /* write end address failed */
        
        return 1;                                                                           /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_WRITE_RAM, SSD1351_CMD))                    /* set write ram */
    {
        handle->debug_print("ssd1351: write ram failed.\n");                                /* write ram failed */
        
        return 1;                                                                           /* return error */
    }
    for (i = 0; i < 128; i++)                                                               /* set row */
    {
        for (j = 0; j < 128; j++)                                                           /* set column */
        {
            if ((handle->conf_1 & 0xC0) == 0)                                               /* if 256 */
            {
                if (_ssd1351_write_byte(handle, 0x00, SSD1351_DATA))                        /* set data */
                {
                    handle->debug_print("ssd1351: write data failed.\n");                   /* write data failed */
                    
                    return 1;                                                               /* return error */
                }
            }
            else if ((handle->conf_1 & 0xC0) == 0x40)                                       /* if 65K */
            {
                if (_ssd1351_write_byte(handle, 0x00, SSD1351_DATA))                        /* set data */
                {
                    handle->debug_print("ssd1351: write data failed.\n");                   /* write data failed */
                    
                    return 1;                                                               /* return error */
                }
                if (_ssd1351_write_byte(handle, 0x00, SSD1351_DATA))                        /* set data */
                {
                    handle->debug_print("ssd1351: write data failed.\n");                   /* write data failed */
                    
                    return 1;                                                               /* return error */
                }
            }
            else                                                                            /* if 262K */
            {
                if (_ssd1351_write_byte(handle, 0x00, SSD1351_DATA))                        /* set data */
                {
                    handle->debug_print("ssd1351: write data failed.\n");                   /* write data failed */
                    
                    return 1;                                                               /* return error */
                }
                if (_ssd1351_write_byte(handle, 0x00, SSD1351_DATA))                        /* set data */
                {
                    handle->debug_print("ssd1351: write data failed.\n");                   /* write data failed */
                    
                    return 1;                                                               /* return error */
                }
                if (_ssd1351_write_byte(handle, 0x00, SSD1351_DATA))                        /* set data */
                {
                    handle->debug_print("ssd1351: write data failed.\n");                   /* write data failed */
                    
                    return 1;                                                               /* return error */
                }
            }
        }
    }
    
    return 0;                                                                               /* success return 0 */
}

/**
 * @brief     draw a point in the display
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] color is the point color
 * @return    status code
 *            - 0 success
 *            - 1 draw point failed
 * @note      none
 */
static uint8_t _ssd1351_draw_point(ssd1351_handle_t *handle, uint8_t x, uint8_t y, uint32_t color)
{
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_COLUMN_ADDRESS, SSD1351_CMD))        /* write column address command */
    {
        handle->debug_print("ssd1351: write command failed.\n");                         /* write command failed */
        
        return 1;                                                                        /* return error */
    }
    if (_ssd1351_write_byte(handle, x, SSD1351_DATA))                                    /* set start address */
    {
        handle->debug_print("ssd1351: write start address failed.\n");                   /* write start address failed */
        
        return 1;                                                                        /* return error */
    }
    if (_ssd1351_write_byte(handle, x, SSD1351_DATA))                                    /* set end address */
    {
        handle->debug_print("ssd1351: write end address failed.\n");                     /* write end address failed */
        
        return 1;                                                                        /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_ROW_ADDRESS, SSD1351_CMD))           /* write column address command */
    {
        handle->debug_print("ssd1351: write command failed.\n");                         /* write command failed */
        
        return 1;                                                                        /* return error */
    }
    if (_ssd1351_write_byte(handle, y, SSD1351_DATA))                                    /* set start address */
    {
        handle->debug_print("ssd1351: write start address failed.\n");                   /* write start address failed */
        
        return 1;                                                                        /* return error */
    }
    if (_ssd1351_write_byte(handle, y, SSD1351_DATA))                                    /* set end address */
    {
        handle->debug_print("ssd1351: write end address failed.\n");                     /* write end address failed */
        
        return 1;                                                                        /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_WRITE_RAM, SSD1351_CMD))                 /* set write ram */
    {
        handle->debug_print("ssd1351: write ram failed.\n");                             /* write ram failed */
        
        return 1;                                                                        /* return error */
    }
    if ((handle->conf_1 & 0xC0) == 0)                                                    /* if 256 */
    {
        if (_ssd1351_write_byte(handle, color & 0xFF, SSD1351_DATA))                     /* set data */
        {
            handle->debug_print("ssd1351: write data failed.\n");                        /* write data failed */
            
            return 1;                                                                    /* return error */
        }
        
        return 0;                                                                        /* success return 0 */
    }
    else if ((handle->conf_1 & 0xC0) == 0x40)                                            /* if 65K */
    {
        if (handle->conf_1 & 0x04)                                                       /* CBA */
        {
            color &= 0x00FFFF;
            if (_ssd1351_write_byte(handle, (color>>8)&0xFF, SSD1351_DATA))              /* set 1st */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }
            if (_ssd1351_write_byte(handle, (color>>0)&0xFF, SSD1351_DATA))              /* set 2nd */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }
            
            return 0;                                                                    /* success return 0 */
        }
        else                                                                             /* ABC */
        {
            color &= 0x00FFFF;                                                           /* get color */
            #if (SSD1351_AUTO_COLOR_CONVERT == 1)                                        /* if auto color convert */
            color = ((color&0xF800)>>11) | (color&0x07E0) | ((color&0x001F)<<11);        /* blue green red */
            #endif
            if (_ssd1351_write_byte(handle, (color>>8)&0xFF, SSD1351_DATA))              /* set 1st */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }
            if (_ssd1351_write_byte(handle, (color>>0)&0xFF, SSD1351_DATA))              /* set 2nd */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }
            
            return 0;                                                                    /* success return 0 */
        }
    }
    else
    {
        if (handle->conf_1 & 0x04)                                                       /* CBA */
        {
            if (_ssd1351_write_byte(handle, (color>>16)&0x3F, SSD1351_DATA))             /* set red */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }
            if (_ssd1351_write_byte(handle, (color>>8)&0x3F, SSD1351_DATA))              /* set green */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }
            if (_ssd1351_write_byte(handle, (color>>0)&0x3F, SSD1351_DATA))              /* set blue */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }
            
            return 0;                                                                    /* success return 0 */
        }
        else                                                                             /* ABC */
        {
            #if (SSD1351_AUTO_COLOR_CONVERT == 1)                                        /* if auto color convert */
            if (_ssd1351_write_byte(handle, (color>>0)&0x3F, SSD1351_DATA))              /* set blue */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }
            if (_ssd1351_write_byte(handle, (color>>8)&0x3F, SSD1351_DATA))              /* set green */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }
            if (_ssd1351_write_byte(handle, (color>>16)&0x3F, SSD1351_DATA))             /* set red */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }
            #else
            if (_ssd1351_write_byte(handle, (color>>16)&0x3F, SSD1351_DATA))             /* set blue */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }
            if (_ssd1351_write_byte(handle, (color>>8)&0x3F, SSD1351_DATA))              /* set green */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }
            if (_ssd1351_write_byte(handle, (color>>0)&0x3F, SSD1351_DATA))              /* set red */
            {
                handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                
                return 1;                                                                /* return error */
            }            
            #endif
            
            return 0;                                                                    /* success return 0 */
        }
    }
}

/**
 * @brief     draw a char in the display
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] chr is the display char
 * @param[in] size is the display size
 * @param[in] color is the display color
 * @return    status code
 *            - 0 success
 *            - 1 show char failed
 * @note      none
 */
static uint8_t _ssd1351_show_char(ssd1351_handle_t *handle, uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint32_t color)
{
    volatile uint8_t temp, t, t1;
    volatile uint8_t y0 = y;
    volatile uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);        /* get size */
    
    chr = chr - ' ';                                                                /* get index */
    for (t = 0; t < csize; t++)                                                     /* write size */
    {   
        if (size == 12)                                                             /* if size 12 */
        {
            temp = gsc_ssd1351_ascii_1206[chr][t];                                  /* get ascii 1206 */
        }
        else if (size == 16)                                                        /* if size 16 */
        {
            temp = gsc_ssd1351_ascii_1608[chr][t];                                  /* get ascii 1608 */
        }
        else if(size == 24)                                                         /* if size 24 */
        {
            temp = gsc_ssd1351_ascii_2412[chr][t];                                  /* get ascii 2412 */
        }
        else
        {
            return 1;                                                               /* return error */
        }
        for (t1 = 0; t1 < 8; t1++)                                                  /* write one line */
        {
            if (temp & 0x80)                                                        /* if 1 */
            {
                _ssd1351_draw_point(handle, x, y, color);                           /* draw point */
            }
            temp <<= 1;                                                             /* left shift 1 */
            y++;
            if ((y - y0) == size)                                                   /* reset size */
            {
                y = y0;                                                             /* set y */
                x++;                                                                /* x++ */
                
                break;                                                              /* break */
            }
        }
    }
  
    return 0;                                                                       /* success return 0 */
}

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
 * @note      none
 */
static uint8_t _ssd1351_fill_rect(ssd1351_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint32_t color)
{
    volatile uint8_t i, j;  
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_COLUMN_ADDRESS, SSD1351_CMD))                /* write column address command */
    {
        handle->debug_print("ssd1351: write command failed.\n");                                 /* write command failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, left, SSD1351_DATA))                                         /* set start address */
    {
        handle->debug_print("ssd1351: write start address failed.\n");                           /* write start address failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, right, SSD1351_DATA))                                        /* set end address */
    {
        handle->debug_print("ssd1351: write end address failed.\n");                             /* write end address failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_ROW_ADDRESS, SSD1351_CMD))                   /* write column address command */
    {
        handle->debug_print("ssd1351: write command failed.\n");                                 /* write command failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, top, SSD1351_DATA))                                          /* set start address */
    {
        handle->debug_print("ssd1351: write start address failed.\n");                           /* write start address failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, bottom, SSD1351_DATA))                                       /* set end address */
    {
        handle->debug_print("ssd1351: write end address failed.\n");                             /* write end address failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_WRITE_RAM, SSD1351_CMD))                         /* set write ram */
    {
        handle->debug_print("ssd1351: write ram failed.\n");                                     /* write ram failed */
        
        return 1;                                                                                /* return error */
    }
    if ((handle->conf_1&0xC0) == 0)                                                              /* if 256 */
    {
        for (i = 0; i < (right - left + 1); i++)                                                 /* x */
        {
            for (j = 0; j < (bottom - top + 1); j++)                                             /* y */
            {
                if (_ssd1351_write_byte(handle, color&0xFF, SSD1351_DATA))                       /* set data */
                {
                    handle->debug_print("ssd1351: write data failed.\n");                        /* write data failed */
                    
                    return 1;                                                                    /* return error */
                }
        
            }
        }
        
        return 0;                                                                                /* success return 0 */
    }
    else if ((handle->conf_1&0xC0) == 0x40)                                                      /* if 65K */
    {
        for (i = 0; i < (right - left + 1); i++)                                                 /* x */
        {
            for (j = 0; j < (bottom - top + 1); j++)                                             /* y */
            {
                if (handle->conf_1 & 0x04)                                                       /* CBA */
                {
                    color &= 0x00FFFF;
                    if (_ssd1351_write_byte(handle, (color>>8)&0xFF, SSD1351_DATA))              /* set 1st */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    if (_ssd1351_write_byte(handle, (color>>0)&0xFF, SSD1351_DATA))              /* set 2nd */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                }
                else                                                                             /* ABC */
                {
                    color &= 0x00FFFF;                                                           /* get color */
                    #if (SSD1351_AUTO_COLOR_CONVERT == 1)                                        /* if auto color convert */
                    color = ((color&0xF800)>>11) | (color&0x07E0) | ((color&0x001F)<<11);        /* blue green red */
                    #endif
                    if (_ssd1351_write_byte(handle, (color>>8)&0xFF, SSD1351_DATA))              /* set 1st */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    if (_ssd1351_write_byte(handle, (color>>0)&0xFF, SSD1351_DATA))              /* set 2nd */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                }
            }
        }
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        for (i = 0; i < (right - left + 1); i++)                                                 /* x */
        {
            for (j = 0; j < (bottom - top + 1); j++)                                             /* y */
            {
                if (handle->conf_1 & 0x04)                                                       /* CBA */
                {
                    if (_ssd1351_write_byte(handle, (color>>16)&0x3F, SSD1351_DATA))             /* set red */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    if (_ssd1351_write_byte(handle, (color>>8)&0x3F, SSD1351_DATA))              /* set green */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    if (_ssd1351_write_byte(handle, (color>>0)&0x3F, SSD1351_DATA))              /* set blue */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                }
                else                                                                             /* ABC */
                {
                    #if (SSD1351_AUTO_COLOR_CONVERT == 1)                                        /* if auto color convert */
                    if (_ssd1351_write_byte(handle, (color>>0)&0x3F, SSD1351_DATA))              /* set blue */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    if (_ssd1351_write_byte(handle, (color>>8)&0x3F, SSD1351_DATA))              /* set green */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    if (_ssd1351_write_byte(handle, (color>>16)&0x3F, SSD1351_DATA))             /* set red */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    #else
                    if (_ssd1351_write_byte(handle, (color>>16)&0x3F, SSD1351_DATA))             /* set blue */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    if (_ssd1351_write_byte(handle, (color>>8)&0x3F, SSD1351_DATA))              /* set green */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    if (_ssd1351_write_byte(handle, (color>>0)&0x3F, SSD1351_DATA))              /* set red */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }                    
                    #endif
                }
            }
        }

        return 0;                                                                                /* success return 0 */
    }
}

/**
 * @brief     draw the picture
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] left is the left coordinate x
 * @param[in] top is the left coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the right coordinate y
 * @param[in] *color points to a image buffer
 * @return    status code
 *            - 0 success
 *            - 1 draw picture failed
 * @note      none
 */
static uint8_t _ssd1351_draw_picture(ssd1351_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint32_t *color)
{
    volatile uint8_t i, j;  
    volatile uint32_t p;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_COLUMN_ADDRESS, SSD1351_CMD))                /* write column address command */
    {
        handle->debug_print("ssd1351: write command failed.\n");                                 /* write command failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, left, SSD1351_DATA))                                         /* set start address */
    {
        handle->debug_print("ssd1351: write start address failed.\n");                           /* write start address failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, right, SSD1351_DATA))                                        /* set end address */
    {
        handle->debug_print("ssd1351: write end address failed.\n");                             /* write end address failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_ROW_ADDRESS, SSD1351_CMD))                   /* write column address command */
    {
        handle->debug_print("ssd1351: write command failed.\n");                                 /* write command failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, top, SSD1351_DATA))                                          /* set start address */
    {
        handle->debug_print("ssd1351: write start address failed.\n");                           /* write start address failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, bottom, SSD1351_DATA))                                       /* set end address */
    {
        handle->debug_print("ssd1351: write end address failed.\n");                             /* write end address failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_WRITE_RAM, SSD1351_CMD))                         /* set write ram */
    {
        handle->debug_print("ssd1351: write ram failed.\n");                                     /* write ram failed */
        
        return 1;                                                                                /* return error */
    }
    if ((handle->conf_1&0xC0) == 0)                                                              /* if 256 */
    {
        if (handle->conf_1 & 0x01)                                                               /* if vertical */
        {
            p = 0;                                                                               /* set zero */
            for (j = 0; j < (bottom - top + 1); j++)                                             /* x */
            {
                for (i = 0; i < (right - left + 1); i++)                                         /* y */
                {
                    if (_ssd1351_write_byte(handle, color[p]&0xFF, SSD1351_DATA))                /* set data */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    p++;                                                                         /* p++ */
                }
            }
        }
        else                                                                                     /* if horizontal */
        {
            p = 0;                                                                               /* set zero */
            for (i = 0; i < (right - left + 1); i++)                                             /* x */
            {
                for (j = 0; j < (bottom - top + 1); j++)                                         /* y */
                {
                    if (_ssd1351_write_byte(handle, color[p]&0xFF, SSD1351_DATA))                /* set data */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    p++;                                                                         /* p++ */
                }
            }
        }
        
        return 0;                                                                                /* success return 0 */
    }
    else if ((handle->conf_1&0xC0) == 0x40)                                                      /* if 65K */
    {
        if (handle->conf_1 & 0x01)                                                               /* if vertical */
        {
            p = 0;
            for (j = 0; j < (bottom - top + 1); j++)                                             /* x */
            {
                for (i = 0; i < (right - left + 1); i++)                                         /* y */
                {
                    if (handle->conf_1 & 0x04)                                                   /* CBA */
                    {
                        color[p] &= 0x00FFFF;
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0xFF, SSD1351_DATA))       /* set 1st */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0xFF, SSD1351_DATA))       /* set 2nd */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                    }
                    else                                                                         /* ABC */
                    {
                        color[p] &= 0x00FFFF;                                                    /* get color */
                        #if (SSD1351_AUTO_COLOR_CONVERT == 1)                                    /* if auto convert */
                        color[p] = ((color[p]&0xF800)>>11) | (color[p]&0x07E0) |                 /* convert color */
                                   ((color[p]&0x001F)<<11);                                      /* blue green red */
                        #endif
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0xFF, SSD1351_DATA))       /* set 1st */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0xFF, SSD1351_DATA))       /* set 2nd */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                    }
                    p++;                                                                         /* p++ */
                }
            }
        }
        else                                                                                     /* if horizontal */
        {
            p = 0;
            for (i = 0; i < (right - left + 1); i++)                                             /* x */
            {
                for (j = 0; j < (bottom - top + 1); j++)                                         /* y */
                {
                    if (handle->conf_1 & 0x04)                                                   /* CBA */
                    {
                        color[p] &= 0x00FFFF;
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0xFF, SSD1351_DATA))       /* set 1st */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0xFF, SSD1351_DATA))       /* set 2nd */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                    }
                    else                                                                         /* ABC */
                    {
                        color[p] &= 0x00FFFF;                                                    /* get color */
                        #if (SSD1351_AUTO_COLOR_CONVERT == 1)                                    /* if auto convert */
                        color[p] = ((color[p]&0xF800)>>11) | (color[p]&0x07E0) |                 /* convert color */
                                   ((color[p]&0x001F)<<11);                                      /* blue green red */
                        #endif
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0xFF, SSD1351_DATA))       /* set 1st */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0xFF, SSD1351_DATA))       /* set 2nd */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                    }
                    p++;                                                                         /* p++ */
                }
            }
        }
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        if (handle->conf_1 & 0x01)                                                               /* if vertical */
        {
            p = 0;                                                                               /* set zero */
            for (j = 0; j < (bottom - top + 1); j++)                                             /* x */
            {
                for (i = 0; i < (right - left + 1); i++)                                         /* y */
                {
                    if (handle->conf_1 & 0x04)                                                   /* CBA */
                    {
                        if (_ssd1351_write_byte(handle, (color[p]>>16)&0x3F, SSD1351_DATA))      /* set red */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0x3F, SSD1351_DATA))       /* set green */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0x3F, SSD1351_DATA))       /* set blue */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                    }
                    else                                                                         /* ABC */
                    {
                        #if (SSD1351_AUTO_COLOR_CONVERT == 1)                                    /* if auto convert */
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0x3F, SSD1351_DATA))       /* set blue */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0x3F, SSD1351_DATA))       /* set green */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>16)&0x3F, SSD1351_DATA))      /* set red */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        #else
                        if (_ssd1351_write_byte(handle, (color[p]>>16)&0x3F, SSD1351_DATA))      /* set blue */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0x3F, SSD1351_DATA))       /* set green */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0x3F, SSD1351_DATA))       /* set red */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        #endif
                    }
                    p++;                                                                         /* p++ */
                }
            }
        }
        else                                                                                     /* if horizontal */
        {
            p = 0;                                                                               /* set zero */
            for (i = 0; i < (right - left + 1); i++)                                             /* x */
            {
                for (j = 0; j < (bottom - top + 1); j++)                                         /* y */
                {
                    if (handle->conf_1 & 0x04)                                                   /* CBA */
                    {
                        if (_ssd1351_write_byte(handle, (color[p]>>16)&0x3F, SSD1351_DATA))      /* set red */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0x3F, SSD1351_DATA))       /* set green */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0x3F, SSD1351_DATA))       /* set blue */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                    }
                    else                                                                         /* ABC */
                    {
                        #if (SSD1351_AUTO_COLOR_CONVERT == 1)                                    /* if auto convert */
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0x3F, SSD1351_DATA))       /* set blue */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0x3F, SSD1351_DATA))       /* set green */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>16)&0x3F, SSD1351_DATA))      /* set red */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        #else
                        if (_ssd1351_write_byte(handle, (color[p]>>16)&0x3F, SSD1351_DATA))      /* set blue */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0x3F, SSD1351_DATA))       /* set green */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0x3F, SSD1351_DATA))       /* set red */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        #endif
                    }
                    p++;                                                                         /* p++ */
                }
            }
        }

        return 0;                                                                                /* success return 0 */
    }
}

/**
 * @brief     draw the 16 bits picture
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] left is the left coordinate x
 * @param[in] top is the left coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the right coordinate y
 * @param[in] *color points to a image buffer
 * @return    status code
 *            - 0 success
 *            - 1 draw picture failed
 * @note      none
 */
static uint8_t _ssd1351_draw_picture_16_bits(ssd1351_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint16_t *color)
{
    volatile uint8_t i, j;  
    volatile uint32_t p;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_COLUMN_ADDRESS, SSD1351_CMD))                /* write column address command */
    {
        handle->debug_print("ssd1351: write command failed.\n");                                 /* write command failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, left, SSD1351_DATA))                                         /* set start address */
    {
        handle->debug_print("ssd1351: write start address failed.\n");                           /* write start address failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, right, SSD1351_DATA))                                        /* set end address */
    {
        handle->debug_print("ssd1351: write end address failed.\n");                             /* write end address failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_ROW_ADDRESS, SSD1351_CMD))                   /* write column address command */
    {
        handle->debug_print("ssd1351: write command failed.\n");                                 /* write command failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, top, SSD1351_DATA))                                          /* set start address */
    {
        handle->debug_print("ssd1351: write start address failed.\n");                           /* write start address failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, bottom, SSD1351_DATA))                                       /* set end address */
    {
        handle->debug_print("ssd1351: write end address failed.\n");                             /* write end address failed */
        
        return 1;                                                                                /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_WRITE_RAM, SSD1351_CMD))                         /* set write ram */
    {
        handle->debug_print("ssd1351: write ram failed.\n");                                     /* write ram failed */
        
        return 1;                                                                                /* return error */
    }
    if ((handle->conf_1&0xC0) == 0)                                                              /* if 256 */
    {
        if (handle->conf_1 & 0x01)                                                               /* if vertical */
        {
            p = 0;                                                                               /* set zero */
            for (j = 0; j < (bottom - top + 1); j++)                                             /* x */
            {
                for (i = 0; i < (right - left + 1); i++)                                         /* y */
                {
                    if (_ssd1351_write_byte(handle, color[p]&0xFF, SSD1351_DATA))                /* set data */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    p++;                                                                         /* p++ */
                }
            }
        }
        else                                                                                     /* if horizontal */
        {
            p = 0;                                                                               /* set zero */
            for (i = 0; i < (right - left + 1); i++)                                             /* x */
            {
                for (j = 0; j < (bottom - top + 1); j++)                                         /* y */
                {
                    if (_ssd1351_write_byte(handle, color[p]&0xFF, SSD1351_DATA))                /* set data */
                    {
                        handle->debug_print("ssd1351: write data failed.\n");                    /* write data failed */
                        
                        return 1;                                                                /* return error */
                    }
                    p++;                                                                         /* p++ */
                }
            }
        }
        
        return 0;                                                                                /* success return 0 */
    }
    else if ((handle->conf_1&0xC0) == 0x40)                                                      /* if 65K */
    {
        if (handle->conf_1 & 0x01)                                                               /* if vertical */
        {
            p = 0;
            for (j = 0; j < (bottom - top + 1); j++)                                             /* x */
            {
                for (i = 0; i < (right - left + 1); i++)                                         /* y */
                {
                    if (handle->conf_1 & 0x04)                                                   /* CBA */
                    {
                        color[p] &= 0x00FFFF;
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0xFF, SSD1351_DATA))       /* set 1st */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0xFF, SSD1351_DATA))       /* set 2nd */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                    }
                    else                                                                         /* ABC */
                    {
                        color[p] &= 0x00FFFF;                                                    /* get color */
                        #if (SSD1351_AUTO_COLOR_CONVERT == 1)                                    /* if auto convert */
                        color[p] = ((color[p]&0xF800)>>11) | (color[p]&0x07E0) |                 /* convert color */
                                   ((color[p]&0x001F)<<11);                                      /* blue green red */
                        #endif
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0xFF, SSD1351_DATA))       /* set 1st */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0xFF, SSD1351_DATA))       /* set 2nd */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                    }
                    p++;                                                                         /* p++ */
                }
            }
        }
        else                                                                                     /* if horizontal */
        {
            p = 0;
            for (i = 0; i < (right - left + 1); i++)                                             /* x */
            {
                for (j = 0; j < (bottom - top + 1); j++)                                         /* y */
                {
                    if (handle->conf_1 & 0x04)                                                   /* CBA */
                    {
                        color[p] &= 0x00FFFF;                                                    /* get color */
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0xFF, SSD1351_DATA))       /* set 1st */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0xFF, SSD1351_DATA))       /* set 2nd */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                    }
                    else                                                                         /* ABC */
                    {
                        color[p] &= 0x00FFFF;                                                    /* get color */
                        #if (SSD1351_AUTO_COLOR_CONVERT == 1)                                    /* if auto convert */
                        color[p] = ((color[p]&0xF800)>>11) | (color[p]&0x07E0) |                 /* convert color */
                                   ((color[p]&0x001F)<<11);                                      /* blue green red */
                        #endif
                        if (_ssd1351_write_byte(handle, (color[p]>>8)&0xFF, SSD1351_DATA))       /* set 1st */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                        if (_ssd1351_write_byte(handle, (color[p]>>0)&0xFF, SSD1351_DATA))       /* set 2nd */
                        {
                            handle->debug_print("ssd1351: write data failed.\n");                /* write data failed */
                            
                            return 1;                                                            /* return error */
                        }
                    }
                    p++;                                                                         /* p++ */
                }
            }
        }
        
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("ssd1351: 262K color format can't use this function.\n");            /* write data failed */
        
        return 1;                                                                                /* success return 0 */
    }
}

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
uint8_t ssd1351_draw_point(ssd1351_handle_t *handle, uint8_t x, uint8_t y, uint32_t color)
{
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    }
    if (x > 127)                                                 /* check x */
    {
        handle->debug_print("ssd1351: x is over 127.\n");        /* x is over 127 */
        
        return 1;                                                /* return error */
    }
    if (y > 127)                                                 /* check y */
    {
        handle->debug_print("ssd1351: y is over 127.\n");        /* y is over 127 */
        
        return 1;                                                /* return error */
    }
    
    return _ssd1351_draw_point(handle, x, y, color);             /* draw point */
}

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
uint8_t ssd1351_write_string(ssd1351_handle_t *handle, uint8_t x, uint8_t y, char *str, uint16_t len, uint32_t color, ssd1351_font_t font)
{    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    if((x > 127) || (y > 127))                                               /* check x, y */
    {
        handle->debug_print("ssd1351: x or y is invalid.\n");                /* x or y is invalid */
        
        return 1;                                                            /* return error */
    }
    while (len && (*str <= '~') && (*str >= ' '))                            /* write all string */
    {       
        if (x > (127 - (font / 2)))                                          /* check x point */
        {
            x = 0;                                                           /* set x */
            y += font;                                                       /* set next row */
        }
        if (y > (127 - font))                                                /* check y pont */
        {
            y = x = 0;                                                       /* reset to 0 */
        }
        if (_ssd1351_show_char(handle, x, y, *str, font, color))             /* show a char */
        {
            return 1;                                                        /* return error */
        }
        x += font/2;                                                         /* x + font/2 */
        str++;                                                               /* str address++ */
        len--;                                                               /* str length-- */
    }
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t ssd1351_fill_rect(ssd1351_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint32_t color)
{
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (left > 127)                                                            /* check left */
    {
        handle->debug_print("ssd1351: left is over 127.\n");                   /* left is over 127 */
        
        return 1;                                                              /* return error */
    }
    if (right > 127)                                                           /* check right */
    {
        handle->debug_print("ssd1351: right is over 127.\n");                  /* right is over 127 */
        
        return 1;                                                              /* return error */
    }
    if (left >= right)                                                         /* check left and right */
    {
        handle->debug_print("ssd1351: left >= right.\n");                      /* left >= right */
        
        return 1;                                                              /* return error */
    }
    if (top > 127)                                                             /* check top */
    {
        handle->debug_print("ssd1351: top is over 127.\n");                    /* top is over 127 */
        
        return 1;                                                              /* return error */
    }
    if (bottom > 127)                                                          /* check bottom */
    {
        handle->debug_print("ssd1351: bottom is over 127.\n");                 /* bottom is over 127 */
        
        return 1;                                                              /* return error */
    }
    if (top >= bottom)                                                         /* check top and bottom */
    {
        handle->debug_print("ssd1351: top >= bottom.\n");                      /* top >= bottom */
        
        return 1;                                                              /* return error */
    }
    
    return _ssd1351_fill_rect(handle, left, top, right, bottom, color);        /* fill rect */
}

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
uint8_t ssd1351_draw_picture(ssd1351_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint32_t *image)
{
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    if (left > 127)                                                            /* check left */
    {
        handle->debug_print("ssd1351: left is over 127.\n");                   /* left is over 127 */
        
        return 1;                                                              /* return error */
    }
    if (right > 127)                                                           /* check right */
    {
        handle->debug_print("ssd1351: right is over 127.\n");                  /* right is over 127 */
        
        return 1;                                                              /* return error  */
    }
    if (left >= right)                                                         /* check left and right */
    {
        handle->debug_print("ssd1351: left >= right.\n");                      /* left >= right */
        
        return 1;                                                              /* return error */
    }
    if (top > 127)                                                             /* check top */
    {
        handle->debug_print("ssd1351: top is over 127.\n");                    /* top is over 127 */
        
        return 1;                                                              /* return error */
    }
    if (bottom > 127)                                                          /* check bottom */
    {
        handle->debug_print("ssd1351: bottom is over 127.\n");                 /* bottom is over 127 */
        
        return 1;                                                              /* return error */
    }
    if (top >= bottom)                                                         /* check top and bottom */
    {
        handle->debug_print("ssd1351: top >= bottom.\n");                      /* top >= bottom */
        
        return 1;                                                              /* return error */
    }
    
    return _ssd1351_draw_picture(handle, left, top, right, bottom, image);     /* draw picture */
}

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
uint8_t ssd1351_draw_picture_16bits(ssd1351_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint16_t *image)
{
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    if (left > 127)                                                                       /* check left */
    {
        handle->debug_print("ssd1351: left is over 127.\n");                              /* left is over 127 */
        
        return 1;                                                                         /* return error */
    }
    if (right > 127)                                                                      /* check right */
    {
        handle->debug_print("ssd1351: right is over 127.\n");                             /* right is over 127 */
        
        return 1;                                                                         /* return error */
    }
    if (left >= right)                                                                    /* check left and right */
    {
        handle->debug_print("ssd1351: left >= right.\n");                                 /* left >= right */
        
        return 1;                                                                         /* return error */
    }
    if (top > 127)                                                                        /* check top */
    {
        handle->debug_print("ssd1351: top is over 127.\n");                               /* top is over 127 */
        
        return 1;                                                                         /* return error */
    }
    if (bottom > 127)                                                                     /* check bottom */
    {
        handle->debug_print("ssd1351: bottom is over 127.\n");                            /* bottom is over 127 */
        
        return 1;                                                                         /* return error */
    }
    if (top >= bottom)                                                                    /* check top and bottom */
    {
        handle->debug_print("ssd1351: top >= bottom.\n");                                 /* top >= bottom */
        
        return 1;                                                                         /* return error */
    }
    
    return _ssd1351_draw_picture_16_bits(handle, left, top, right, bottom, image);        /* draw picture */
}

/**
 * @brief  start the display moving
 * @return status code
 *         - 0 success
 *         - 1 start moving failed
 * @note   none
 */
uint8_t ssd1351_start_moving(ssd1351_handle_t *handle)
{
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_START_MOVING, SSD1351_CMD))        /* set start moving */
    {
        handle->debug_print("ssd1351: write start moving failed.\n");              /* write start moving failed */
        
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

/**
 * @brief  stop the display moving
 * @return status code
 *         - 0 success
 *         - 1 stop moving failed
 * @note   none
 */
uint8_t ssd1351_stop_moving(ssd1351_handle_t *handle)
{
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    
    if (_ssd1351_write_byte(handle, SSD1351_CMD_STOP_MOVING, SSD1351_CMD))        /* set stop moving */
    {
        handle->debug_print("ssd1351: write stop moving failed.\n");              /* write stop moving failed */
        
        return 1;                                                                 /* return error */
    }
    
    return 0;                                                                     /* success return 0 */
}

/**
 * @brief     write the command
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] cmd is the send command
 * @return    status code
 *            - 0 success
 *            - 1 write command failed
 * @note      none
 */
uint8_t ssd1351_write_cmd(ssd1351_handle_t *handle, uint8_t cmd)
{
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    }
    
    return _ssd1351_write_byte(handle, cmd, SSD1351_CMD);        /* write command */
}

/**
 * @brief     write the data
 * @param[in] *handle points to a ssd1351 handle structure
 * @param[in] data is the send data
 * @return    status code
 *            - 0 success
 *            - 1 write data failed
 * @note      none
 */
uint8_t ssd1351_write_data(ssd1351_handle_t *handle, uint8_t data)
{
    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }
  
    return _ssd1351_write_byte(handle, data, SSD1351_DATA);        /* write data */
}

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
uint8_t ssd1351_init(ssd1351_handle_t *handle)
{
    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->debug_print == NULL)                                            /* check debug_print */
    {
        return 3;                                                               /* return error */
    }
    if (handle->spi_init == NULL)                                               /* check spi_init */
    {
        handle->debug_print("ssd1351: spi_init is null.\n");                    /* spi_init is null */
        
        return 3;                                                               /* return error */
    }
    if (handle->spi_deinit == NULL)                                             /* check spi_deinit */
    {
        handle->debug_print("ssd1351: spi_deinit is null.\n");                  /* spi_deinit is null */
        
        return 3;                                                               /* return error */
    }
    if (handle->spi_write_cmd == NULL)                                          /* check spi_write_cmd */
    {
        handle->debug_print("ssd1351: spi_write_cmd is null.\n");               /* spi_init is null */
        
        return 3;                                                               /* return error */
    }
    if (handle->cmd_data_gpio_init == NULL)                                     /* check cmd_data_gpio_init */
    {
        handle->debug_print("ssd1351: cmd_data_gpio_init is null.\n");          /* cmd_data_gpio_init is null */
        
        return 3;                                                               /* return error */
    }
    if (handle->cmd_data_gpio_deinit == NULL)                                   /* check cmd_data_gpio_deinit */
    {
        handle->debug_print("ssd1351: cmd_data_gpio_deinit is null.\n");        /* cmd_data_gpio_deinit is null */
        
        return 3;                                                               /* return error */
    }
    if (handle->cmd_data_gpio_write == NULL)                                    /* check cmd_data_gpio_write */
    {
        handle->debug_print("ssd1351: cmd_data_gpio_write is null.\n");         /* cmd_data_gpio_write is null */
        
        return 3;                                                               /* return error */
    }
    if (handle->reset_gpio_init == NULL)                                        /* check reset_gpio_init */
    {
        handle->debug_print("ssd1351: reset_gpio_init is null.\n");             /* reset_gpio_init is null */
        
        return 3;                                                               /* return error */
    }
    if (handle->reset_gpio_deinit == NULL)                                      /* check reset_gpio_deinit */
    {
        handle->debug_print("ssd1351: reset_gpio_deinit is null.\n");           /* reset_gpio_deinit is null */
        
        return 3;                                                               /* return error */
    }
    if (handle->reset_gpio_write == NULL)                                       /* check reset_gpio_write */
    {
        handle->debug_print("ssd1351: reset_gpio_write is null.\n");            /* reset_gpio_write is null */
        
        return 3;                                                               /* return error */
    }
    if (handle->delay_ms == NULL)                                               /* check delay_ms */
    {
        handle->debug_print("ssd1351: delay_ms is null.\n");                    /* delay_ms is null */
        
        return 3;                                                               /* return error */
    }
    
    if (handle->cmd_data_gpio_init())                                           /* check cmd_data_gpio_init */
    {
        handle->debug_print("ssd1351: cmd data gpio init failed.\n");           /* cmd data gpio init failed */
        
        return 5;                                                               /* return error */
    }
    if (handle->reset_gpio_init())                                              /* reset gpio init */
    {
        handle->debug_print("ssd1351: reset gpio init failed.\n");              /* reset gpio init failed */
        handle->cmd_data_gpio_deinit();                                         /* cmd_data_gpio_deinit */
        
        return 4;                                                               /* return error */
    }
    if (handle->reset_gpio_write(0))                                            /* write 0 */
    {
        handle->debug_print("ssd1351: reset gpio write failed.\n");             /* reset gpio write failed */
        handle->cmd_data_gpio_deinit();                                         /* cmd_data_gpio_deinit */
        handle->reset_gpio_deinit();                                            /* reset_gpio_deinit */
        
        return 4;                                                               /* return error */
    }
    handle->delay_ms(100);                                                      /* delay 100 ms */
    if (handle->reset_gpio_write(1))                                            /* write 1 */
    {
        handle->debug_print("ssd1351: reset gpio write failed.\n");             /* reset gpio write failed */
        handle->cmd_data_gpio_deinit();                                         /* cmd_data_gpio_deinit */
        handle->reset_gpio_deinit();                                            /* reset_gpio_deinit */
        
        return 4;                                                               /* return error */
    }
    if (handle->spi_init())                                                     /* spi init */
    {
        handle->debug_print("ssd1351: spi init failed.\n");                     /* spi init failed */
        handle->cmd_data_gpio_deinit();                                         /* cmd_data_gpio_deinit */
        handle->reset_gpio_deinit();                                            /* reset_gpio_deinit */
        
        return 1;                                                               /* return error */
    }
    handle->inited = 1;                                                         /* flag inited */
    
    return 0;                                                                   /* success return 0 */
}

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
uint8_t ssd1351_deinit(ssd1351_handle_t *handle)
{
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }

    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_SLEEP_MODE_ON, SSD1351_CMD))          /* set sleep mode on */
    {
        handle->debug_print("ssd1351: write sleep mode on failed.\n");                    /* write sleep mode on failed */
        
        return 4;                                                                         /* return error */
    }
    if (_ssd1351_write_byte(handle, SSD1351_CMD_SET_DISPLAY_ALL_OFF, SSD1351_CMD))        /* set display all off */
    {
        handle->debug_print("ssd1351: write display all off failed.\n");                  /* write display all off failed */
        
        return 4;                                                                         /* return error */
    }
    if (handle->reset_gpio_deinit())                                                      /* reset gpio deinit */
    {
        handle->debug_print("ssd1351: reset gpio deinit failed.\n");                      /* reset gpio deinit failed */
            
        return 5;                                                                         /* return error */
    }
    if (handle->cmd_data_gpio_deinit())                                                   /* cmd data gpio deinit */
    {
        handle->debug_print("ssd1351: cmd data gpio deinit failed.\n");                   /* cmd data gpio deinit failed */
            
        return 6;                                                                         /* return error */
    }
    if (handle->spi_deinit())                                                             /* spi deinit */
    {
        handle->debug_print("ssd1351: spi deinit failed.\n");                             /* spi deinit failed */
        
        return 1;                                                                         /* return error */
    }
    handle->inited = 0;                                                                   /* flag close */
    
    return 0;                                                                             /* success return 0 */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to a ssd1351 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ssd1351_info(ssd1351_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(ssd1351_info_t));                        /* initialize ssd1351 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "SPI", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver verison */
    
    return 0;                                                       /* success return 0 */
}
