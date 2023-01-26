### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

SPI Pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8.

GPIO Pin: RESET/CMD_DATA GPIO27/GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
# open another terminal
./ssd1351_server 

# use the CMake terminal
make test
```

Find the compiled library in CMake. 

```cmake
find_package(ssd1351 REQUIRED)
```
### 3. SSD1351

#### 3.1 Command Instruction

1. Show ssd1351 chip and driver information.

   ```shell
   ssd1351 (-i | --information)
   ```

2. Show ssd1351  help.

   ```shell
   ssd1351 (-h | --help)
   ```

3. Show ssd1351  pin connections of the current board.

   ```shell
   ssd1351 (-p | --port)
   ```

4. Run ssd1351 display test.

   ```shell
   ssd1351 (-t display | --test=display)
   ```

5. Run ssd1351 basic init function.

   ```shell
   ssd1351 (-e basic-init | --example=basic-init)
   ```

6. Run ssd1351 basic deinit function.

   ```shell
   ssd1351 (-e basic-deinit | --example=basic-deinit)
   ```

7. Run ssd1351 show string function, string means the shown string. 

   ```shell
   ssd1351 (-e basic-str | --example=basic-str) [--str=<string>]
   ```

8. Run ssd1351 display on function.

   ```shell
   ssd1351 (-e basic-display-on | --example=basic-display-on)
   ```

9. Run ssd1351 display off function.

   ```shell
   ssd1351 (-e basic-display-off | --example=basic-display-off)
   ```

10. Run ssd1351 clear screen function.

    ```shell
    ssd1351 (-e basic-clear | --example=basic-clear)
    ```

11. Run ssd1351 write pont function, x0 is the x of the point and y0 is the y of the point, rgb565 means the filled color.

    ```shell
    ssd1351 (-e basic-point | --example=basic-point) --x0=<x0> --y0=<y0> [--color=<rgb565>]
    ```

12. Run ssd1351 draw rectangle function, x1 is the top left x of the rect, y1 is the top left y of the rect, x2 is the bottom right x of the rect and y2 is the bottom right y of the rect, rgb565 means the filled color.

    ```shell
    ssd1351 (-e basic-rect | --example=basic-rect) --x1=<x1> --y1=<y1> --x2=<x2> --y2=<y2> [--color=<rgb565>]
    ```

13. Run ssd1351 advance init function. 

    ```shell
    ssd1351 (-e advance-init | --example=advance-init)
    ```

14. Run ssd1351 advance deinit function. 

    ```shell
    ssd1351 (-e advance-deinit | --example=advance-deinit)
    ```

15. Run ssd1351 show string function, string means the shown string. 

    ```shell
    ssd1351 (-e advance-str | --example=advance-str) [--str=<string>]
    ```

16. Run ssd1351 display on function.

    ```shell
    ssd1351 (-e advance-display-on | --example=advance-display-on)
    ```

17. Run ssd1351 display off function.

    ```shell
    ssd1351 (-e advance-display-off | --example=advance-display-off)
    ```

18. Run ssd1351 clear screen function.

    ```shell
    ssd1351 (-e advance-clear | --example=advance-clear)
    ```

19. Run ssd1351 write pont function, x0 is the x of the point and y0 is the y of the point, rgb565 means the filled color.

    ```shell
    ssd1351 (-e advance-point | --example=advance-point) --x0=<x0> --y0=<y0> [--color=<rgb565>]
    ```

20. Run ssd1351 draw rectangle function, x1 is the top left x of the rect, y1 is the top left y of the rect, x2 is the bottom right x of the rect and y2 is the bottom right y of the rect, rgb565 means the filled color.

    ```shell
    ssd1351 (-e advance-rect | --example=advance-rect) --x1=<x1> --y1=<y1> --x2=<x2> --y2=<y2> [--color=<rgb565>]
    ```

21. Run ssd1351 disable scroll function.

    ```shell
    ssd1351 (-e advance-disable-scroll | --example=advance-disable-scroll)
    ```

22. Run ssd1351 scroll function, num means the scroll number, if > 0 means left scroll and < 0 means right scroll, srow means the start row, len means the row length and srow + len max is 128.

    ```shell
    ssd1351 (-e advance-scroll | --example=advance-scroll) [--scroll=<num>] [--start=<srow>] [--length=<len>] [--mode=<TEST | NORMAL | SLOW | SLOWEST>]
    ```

#### 3.2 How to run

```shell
# open one terminal and run the server
./ssd1351_server 
```
```shell
# open another terminal and sent the command
./ssd1351 xxx xxx xxx ...
```

#### 3.3 Command Example

```shell
./ssd1351 -i

ssd1351: chip is Solomon Systech SSD1351.
ssd1351: manufacturer is Solomon Systech.
ssd1351: interface is SPI.
ssd1351: driver version is 2.0.
ssd1351: min supply voltage is 2.4V.
ssd1351: max supply voltage is 2.6V.
ssd1351: max current is 0.70mA.
ssd1351: max temperature is 85.0C.
ssd1351: min temperature is -40.0C.
```

```shell
./ssd1351 -p

ssd1351: SCK connected to GPIO11(BCM).
ssd1351: MISO connected to GPIO9(BCM).
ssd1351: MOSI connected to GPIO10(BCM).
ssd1351: CS connected to GPIO8(BCM).
ssd1351: cmd data gpio GPIO connected to GPIO17(BCM).
ssd1351: reset GPIO connected to GPIO27(BCM).
```

```shell
./ssd1351 -t display

ssd1351: chip is Solomon Systech SSD1351.
ssd1351: manufacturer is Solomon Systech.
ssd1351: interface is SPI.
ssd1351: driver version is 2.0.
ssd1351: min supply voltage is 2.4V.
ssd1351: max supply voltage is 2.6V.
ssd1351: max current is 0.70mA.
ssd1351: max temperature is 85.0C.
ssd1351: min temperature is -40.0C.
ssd1351: start display test.
ssd1351: font 12 test.
ssd1351: font 16 test.
ssd1351: font 24 test.
ssd1351: rectangle test.
ssd1351: rand point test.
ssd1351: picture test.
ssd1351: graphic acceleration test.
ssd1351: left horizontal scroll.
ssd1351: right horizontal scroll.
ssd1351: finish display test.
```

```shell
./ssd1351 -e basic-str --str=12345abcdEFGH

ssd1351: 12345abcdEFGH.
```

```shell
./ssd1351 -e basic-init

ssd1351: init success.
```

```shell
./ssd1351 -e basic-display-off

ssd1351: display off.
```

```shell
./ssd1351 -e basic-display-on

ssd1351: display on.
```

```shell
./ssd1351 -e basic-clear

ssd1351: clear screen.
```

```shell
./ssd1351 -e basic-deinit

ssd1351: deinit ssd1351.
```

```shell
./ssd1351 -e basic-point --x0=16 --y0=16 --color=65535

ssd1351: write point 16 16 65535.
```

```shell
./ssd1351 -e basic-rect --x1=0 --y1=0 --x2=72 --y2=72 --color=65535

ssd1351: draw rect 0 0 72 72.
```

```shell
./ssd1351 -e advance-init

ssd1351: init success.
```

```shell
./ssd1351 -e advance-str --str=12345abcdEFGH

ssd1351: 12345abcdEFGH.
```

```shell
./ssd1351 -e advance-display-off

ssd1351: display off.
```

```shell
./ssd1351 -e advance-display-on

ssd1351: display on.
```

```shell
./ssd1351 -e advance-clear

ssd1351: clear screen.
```

```shell
./ssd1351 -e advance-point --x0=16 --y0=16 --color=65535

ssd1351: write point 16 16 65535.
```

```shell
./ssd1351 -e advance-rect --x1=0 --y1=0 --x2=72 --y2=72 --color=65535

ssd1351: draw rect 0 0 72 72.
```

```shell
./ssd1351 -e advance-deinit

ssd1351: deinit ssd1351.
```

```shell
./ssd1351 -e advance-disable-scroll

ssd1351: disable scroll.
```

```shell
./ssd1351 -e advance-scroll --scroll=1 --start=0 --length=128 --mode=NORMAL

ssd1351: set scroll 1 0 128.
```

```shell
./ssd1351 -h

Usage:
  ssd1351 (-i | --information)
  ssd1351 (-h | --help)
  ssd1351 (-p | --port)
  ssd1351 (-t display | --test=display)
  ssd1351 (-e basic-init | --example=basic-init)
  ssd1351 (-e basic-deinit | --example=basic-deinit)
  ssd1351 (-e basic-str | --example=basic-str) [--str=<string>]
  ssd1351 (-e basic-display-on | --example=basic-display-on)
  ssd1351 (-e basic-display-off | --example=basic-display-off)
  ssd1351 (-e basic-clear | --example=basic-clear)
  ssd1351 (-e basic-point | --example=basic-point) --x0=<x0> --y0=<y0> [--color=<rgb565>]
  ssd1351 (-e basic-rect | --example=basic-rect) --x1=<x1> --y1=<y1> --x2=<x2> --y2=<y2> [--color=<rgb565>]
  ssd1351 (-e advance-init | --example=advance-init)
  ssd1351 (-e advance-deinit | --example=advance-deinit)
  ssd1351 (-e advance-str | --example=advance-str) [--str=<string>]
  ssd1351 (-e advance-display-on | --example=advance-display-on)
  ssd1351 (-e advance-display-off | --example=advance-display-off)
  ssd1351 (-e advance-clear | --example=advance-clear)
  ssd1351 (-e advance-point | --example=advance-point) --x0=<x0> --y0=<y0> [--color=<rgb565>]
  ssd1351 (-e advance-rect | --example=advance-rect) --x1=<x1> --y1=<y1> --x2=<x2> --y2=<y2> [--color=<rgb565>]
  ssd1351 (-e advance-disable-scroll | --example=advance-disable-scroll)
  ssd1351 (-e advance-scroll | --example=advance-scroll) [--scroll=<num>] [--start=<srow>] [--length=<len>]
          [--mode=<TEST | NORMAL | SLOW | SLOWEST>]

Options:
     --color=<rgb565>     Set the chip color.([default: 65535])
  -e <basic-init | basic-deinit | basic-str | basic-display-on | basic-display-off | basic-clear
     | basic-point | basic-rect>, --example=<advance-init | advance-deinit | advance-str | advance-display-on
     | advance-display-off | advance-clear | advance-point | advance-rect | advance-disable-scroll | advance-scroll>
                          Run the driver example.
  -h, --help              Show the help.
  -i, --information       Show the chip information.
      --mode=<TEST | NORMAL | SLOW | SLOWEST>
                          Set the scrolling mode.([default: NORMAL])
      --length=<len>      Set the scrolling row length and len + srow <= 128.([default: 128])
  -p, --port              Display the pin connections of the current board.
      --scroll            Set the scrolling number, if > 0 left scroll and < 0 right scroll.([default: 1])
      --start=<srow>      Set the scrolling start row and len + srow <= 128.([default: 0])
      --str=<string>      Set the display string.([default: libdriver])
  -t <display>, --test=<display>
                          Run the driver test.
      --x0=<x0>           Set the x0 and it is the x of the point.
      --x1=<x1>           Set the x1 and it is the top left x of the rect.
      --x2=<x2>           Set the x2 and it is the bottom right x of the rect.
      --y0=<y0>           Set the y0 and it is the y of the point.
      --y1=<y1>           Set the y1 and it is the top left y of the rect.
      --y2=<y2>           Set the y2 and it is the bottom right y of the rect.
```

