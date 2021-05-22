<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver SSD1351

[English](/README.md) | [ 简体中文](/README_CN.md)

The SSD1351 is a CMOS OLED/PLED driver with 384 segments and 128 commons output, supporting up to 128RGB x 128 dot matrix display. This chip is designed for Common Cathode type OLED/PLED panel.The SSD1351 has embedded Graphic Display Data RAM (GDDRAM). It supports with 8, 16, 18 bits 8080 / 6800 parallel interface, Serial Peripheral Interface. It has 256-step contrast and 262K color control, giving vivid color display on OLED panels.

LibDriver SSD1351 is the full function driver of SSD1351 launched by LibDriver.It provides the functions of drawing single point, filling block, image display, etc.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example advance](#example-advance)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver SSD1351 source files.

/interface includes LibDriver SSD1351 SPI platform independent template。

/test includes LibDriver SSD1351 driver test code and this code can test the chip necessary function simply。

/example includes LibDriver SSD1351 sample code.

/doc includes LibDriver SSD1351 offline document.

/datasheet includes SSD1351 datasheet。

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

### Install

Reference /interface SPI platform independent template and finish your platform SPI driver.

Add /src, /interface and /example to your project.

### Usage

#### example basic

```C
uint8_t res;

res = ssd1351_basic_init();
if (res)
{
    ssd1351_interface_debug_print("ssd1351: init failed.\n");

    return 1;
}

...

res = ssd1351_basic_clear();
if (res)
{
    ssd1351_interface_debug_print("ssd1351: clear screen failed.\n");
    ssd1351_basic_deinit();

    return 1;
}
res = ssd1351_basic_string(0, 0, "123", 3, 0xFFFF, SSD1351_FONT_16);
if (res)
{
    ssd1351_interface_debug_print("ssd1351: show string failed.\n");
    ssd1351_basic_deinit();

    return 1;
}

...

res = ssd1351_basic_rect(0, 32, 32, 32, 0xFFFF);
if (res)
{
    ssd1351_basic_deinit();

    return 1;
}

...

ssd1351_basic_deinit();

return 0;
```

#### example advance

```C
uint8_t res;

res = ssd1351_advance_init();
if (res)
{
    ssd1351_interface_debug_print("ssd1351: init failed.\n");

    return 1;
}

...

res = ssd1351_advance_clear();
if (res)
{
    ssd1351_interface_debug_print("ssd1351: clear screen failed.\n");
    ssd1351_advance_deinit();

    return 1;
}
res = ssd1351_advance_string(0, 0, "123", 3, 0xFFFF, SSD1351_FONT_16);
if (res)
{
    ssd1351_interface_debug_print("ssd1351: show string failed.\n");
    ssd1351_advance_deinit();

    return 1;
}

...

res = ssd1351_advance_write_point(56, 56, 0xFF00);
if (res)
{
    ssd1351_advance_deinit();

    return 1;
}

...

res = ssd1351_advance_rect(0, 32, 32, 32, 0xFFFF);
if (res)
{
    ssd1351_advance_deinit();

    return 1;
}

...

res = ssd1351_advance_set_scroll(1, 0, 128, SSD1351_SCROLL_MODE_NORMAL);
if (res)
{
    ssd1351_advance_deinit();

    return 1;
}
res = ssd1351_advance_enable_scroll();
if (res)
{
    ssd1351_advance_deinit();

    return 1;
}

...

ssd1351_advance_deinit();

return 0;
```

### Document

Online documents: https://www.libdriver.com/docs/ssd1351/index.html

Offline documents: /doc/html/index.html

### Contributing

Please sent an e-mail to lishifenging@outlook.com

### License

Copyright (C) LibDriver 2015-2021 All rights reserved 



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please sent an e-mail to lishifenging@outlook.com