### 1. chip

#### 1.1 chip info

chip name : Raspberry Pi 4B

spi pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8

gpio pin: RESET/CMD_DATA GPIO27/GPIO17

### 2. install

#### 2.1 install info

```shell
sudo apt-get install libgpiod-dev

make
```

#### 2.2 run server

```shell
./ssd1351_server 
```

### 3. ssd1351

#### 3.1 command Instruction

​          ssd1351 is a basic command which can test all ssd1351 driver function:

​           -i        show ssd1351 chip and driver information.

​           -h       show ssd1351  help.

​           -p       show ssd1351  pin connections of the current board.

​           -t display        run ssd1351 display test. 

​           -c (basic (-init | -deinit | -str <string> | -displayon | -displayoff | -clear | -writepoint <x> <y> <color> | -rect <x1> <y1> <x2> <y2> <color>) | advance (-init | -deinit | -str <string> | -displayon | -displayoff | -clear | -writepoint <x> <y> <color> | -rect <x1> <y1> <x2> <y2> <color> | -disable_scroll | -scroll <scroll> <startrow> <rowlen> (TSET | NORMAL | SLOW | SLOWEST)))

​           -c basic -init        run ssd1351 basic init function. 

​           -c basic -deinit        run ssd1351 basic deinit function. 

​           -c basic -str <string>        run ssd1351 show string function. string means the shown string. 

​           -c basic -displayon        run ssd1351 display on function. 

​           -c basic -displayoff        run ssd1351 display off function. 

​           -c basic -clear        run ssd1351 clear screen function. 

​           -c basic -writepoint  <x> <y> <color>        run ssd1351 write pont function. x and y mean coordinate in screen. color means the filled color.

​           -c basic -rect <x1> <y1> <x2> <y2> <color>        run ssd1351 draw rectangle function . x1 means x start. y1 means y start. x2 means x end. y2 means y end. color means the filled color.

​           -c advance -init        run ssd1351 advance init function. 

​           -c advance -deinit        run ssd1351 advance deinit function. 

​           -c advance -str <string>        run ssd1351 show string function. string means the shown string. 

​           -c advance -displayon        run ssd1351 display on function. 

​           -c advance -displayoff        run ssd1351 display off function. 

​           -c advance -clear        run ssd1351 clear screen function. 

​           -c advance -writepoint  <x> <y> <color>        run ssd1351 write pont function. x and y mean coordinate in screen. color means the filled color.

​           -c advance -rect <x1> <y1> <x2> <y2> <color>        run ssd1351 draw rectangle function . x1 means x start. y1 means y start. x2 means x end. y2 means y end. color means the filled color.

​           -c advance -disable_scroll        run ssd1351 disable scroll function. 

​           -c advance -scroll <scroll> <startrow> <rowlen> (TSET | NORMAL | SLOW | SLOWEST)        run ssd1351 scroll function. scroll means the scroll number,if >0 left scroll and <0 right scroll. startrow means the start row. rowlen means the row lenght and startrow+rowlen max is 128. 

#### 3.2 command example

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
./ssd1351 -c basic -str 12345abcdEFGH!@#$<>?{}[];<>

ssd1351: 12345abcdEFGH!@#$<>?{}[];<>.
```

```shell
./ssd1351 -c basic -init

ssd1351: init success.
```

```shell
./ssd1351 -c basic -displayoff

ssd1351: display off.
```

```shell
./ssd1351 -c basic -displayon

ssd1351: display on.
```

```shell
./ssd1351 -c basic -clear

ssd1351: clear screen.
```

```shell
./ssd1351 -c basic -deinit

ssd1351: deinit ssd1351.
```

```shell
./ssd1351 -c basic -writepoint 16 16 65535

ssd1351: write point 16 16 65535.
```

```shell
./ssd1351 -c basic -rect 0 0 72 72 31

ssd1351: draw rect 0 0 72 72.
```

```shell
./ssd1351 -c advance -init

ssd1351: init success.
```

```shell
./ssd1351 -c advance -str 12345abcdEFGH!@#$<>?{}[];<>

ssd1351: 12345abcdEFGH!@#$<>?{}[];<>.
```

```shell
./ssd1351 -c advance -displayoff

ssd1351: display off.
```

```shell
./ssd1351 -c advance -displayon

ssd1351: display on.
```

```shell
./ssd1351 -c advance -clear

ssd1351: clear screen.
```

```shell
./ssd1351 -c advance -writepoint 16 16 65535

ssd1351: write point 16 16 65535.
```

```shell
./ssd1351 -c advance -rect 0 0 72 72 31

ssd1351: draw rect 0 0 72 72.
```

```shell
./ssd1351 -c advance -deinit

ssd1351: deinit ssd1351.
```

```shell
./ssd1351 -c advance -disable_scroll

ssd1351: disable scroll.
```

```shell
./ssd1351 -c advance -scroll 1 0 128 NORMAL

ssd1351: set scroll 1 0 128.
```

```shell
./ssd1351 -h

ssd1351 -i
	show ssd1351 chip and driver information.
ssd1351 -h
	show ssd1351 help.
ssd1351 -p
	show ssd1351 pin connections of the current board.
ssd1351 -t display
	run ssd1351 display test.
ssd1351 -c basic -init
	run ssd1351 basic init function.
ssd1351 -c basic -deinit
	run ssd1351 basic deinit function.
ssd1351 -c basic -str <string>
	run ssd1351 show string function.string means the shown string.
ssd1351 -c basic -displayon
	run ssd1351 display on function.
ssd1351 -c basic -displayoff
	run ssd1351 display off function.
ssd1351 -c basic -clear
	run ssd1351 clear screen function.
ssd1351 -c basic -writepoint <x> <y> <color>
	run ssd1351 write pont function.x and y mean coordinate in screen.color means the filled color.
ssd1351 -c basic -rect <x1> <y1> <x2> <y2> <color>
	run ssd1351 draw rectangle function.x1 means x start.y1 means y start.x2 means x end.y2 means y end.color means the filled color.
ssd1351 -c advance -init
	run ssd1351 advance init function.
ssd1351 -c advance -deinit
	run ssd1351 advance deinit function.
ssd1351 -c advance -str <string>
	run ssd1351 show string function.string means the shown string.
ssd1351 -c advance -displayon
	run ssd1351 display on function.
ssd1351 -c advance -displayoff
	run ssd1351 display off function.
ssd1351 -c advance -clear
	run ssd1351 clear screen function.
ssd1351 -c advance -writepoint <x> <y> <color>
	run ssd1351 write pont function.x and y mean coordinate in screen.color means the filled color.
ssd1351 -c advance -rect <x1> <y1> <x2> <y2> <color>
	run ssd1351 draw rectangle function.x1 means x start.y1 means y start.x2 means x end.y2 means y end.color means the filled color.
ssd1351 -c advance -disable_scroll
	run ssd1351 disable scroll function.
ssd1351 -c advance -scroll <scroll> <startrow> <rowlen> (TSET | NORMAL | SLOW | SLOWEST)
	run ssd1351 scroll function.scroll means the scroll number,if >0 left scroll and <0 right scroll.startrow means the start row.rowlen means the row lenght and startrow + rowlen max is 128.
```

