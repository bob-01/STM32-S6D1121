### STM32 LCD S6D1121 2.0"

https://www.youtube.com/watch?v=h1Iup8MnTXo

Подключение пинов в s6d1121.h

```c
#define LCD_CTRL_PORT GPIOB
#define LCD_DATA_PORT GPIOA

#define LCD_CS	GPIO_PIN_10
#define LCD_RS  GPIO_PIN_1
#define LCD_WR  GPIO_PIN_0
#define LCD_RST	GPIO_PIN_11
```

Пример(часть кода):

```c
#include "s6d1121.h"

extern unsigned char SmallFont[];
uint8_t position_x;

disp_x_size = 239;
disp_y_size = 319;
int buf[318];
int x, x2;
int y, y2;
int r;

InitLCD(1);
setFont(SmallFont);

clrScr();
setColor(255, 0, 0);
fillRect(0, 0, 319, 13);
setColor(64, 64, 64);
fillRect(0, 226, 319, 239);
setColor(255, 255, 255);
setBackColor(255, 0, 0);
print("* Universal Color TFT Display Library *", CENTER, 1, 0);
setBackColor(64, 64, 64);
setColor(255,255,0);
print("<http://electronics.henningkarlsen.com>", CENTER, 227, 0);
HAL_Delay(500);

setColor(0, 0, 255);
drawRect(0, 14, 319, 225);

// Draw crosshairs
setColor(0, 0, 255);
setBackColor(0, 0, 0);
drawLine(159, 15, 159, 224);
drawLine(1, 119, 318, 119);
for (int i=9; i<310; i+=10)
drawLine(i, 117, i, 121);
for (int i=19; i<220; i+=10)
drawLine(157, i, 161, i);

// Draw sin-, cos- and tan-lines
setColor(0,255,255);
print("Sin", 5, 15, 0);
for (int i=1; i<318; i++)
{
	drawPixel(i,119+(sin(((i*1.13)*3.14)/180)*95));
}

setColor(255,0,0);
print("Cos", 5, 27, 0);
for (int i=1; i<318; i++)
{
	drawPixel(i,119+(cos(((i*1.13)*3.14)/180)*95));
}

setColor(255,255,0);
print("Tan", 5, 39, 0);
for (int i=1; i<318; i++)
{
	drawPixel(i,119+(tan(((i*1.13)*3.14)/180)));
}

HAL_Delay(500);

setColor(0,0,0);
fillRect(1,15,318,224);
setColor(0, 0, 255);
setBackColor(0, 0, 0);
drawLine(159, 15, 159, 224);
drawLine(1, 119, 318, 119);

// Draw a moving sinewave
x=1;
for (int i=1; i<(318*20); i++)
{
x++;
if (x==319)
	x=1;
if (i>319)
{
	if ((x==159)||(buf[x-1]==119))
	setColor(0,0,255);
	else
	setColor(0,0,0);
	drawPixel(x,buf[x-1]);
}
setColor(0,255,255);
y=119+(sin(((i*1.1)*3.14)/180)*(90-(i / 100)));
drawPixel(x,y);
buf[x-1]=y;
}

HAL_Delay(500);

setColor(0,0,0);
fillRect(1,15,318,224);

// Draw some filled rectangles
for (int i=1; i<6; i++)
{
switch (i)
{
	case 1:
	setColor(255,0,255);
	break;
	case 2:
	setColor(255,0,0);
	break;
	case 3:
	setColor(0,255,0);
	break;
	case 4:
	setColor(0,0,255);
	break;
	case 5:
	setColor(255,255,0);
	break;
}
fillRect(70+(i*20), 30+(i*20), 130+(i*20), 90+(i*20));
}


HAL_Delay(500);

setColor(0,0,0);
fillRect(1,15,318,224);

// Draw some filled, rounded rectangles
for (int i=1; i<6; i++)
{
switch (i)
{
	case 1:
	setColor(255,0,255);
	break;
	case 2:
	setColor(255,0,0);
	break;
	case 3:
	setColor(0,255,0);
	break;
	case 4:
	setColor(0,0,255);
	break;
	case 5:
	setColor(255,255,0);
	break;
}
fillRoundRect(190-(i*20), 30+(i*20), 250-(i*20), 90+(i*20));
}


HAL_Delay(500);
setColor(0,0,0);
fillRect(1,15,318,224);

// Draw some filled circles
for (int i=1; i<6; i++)
{
switch (i)
{
	case 1:
	setColor(255,0,255);
	break;
	case 2:
	setColor(255,0,0);
	break;
	case 3:
	setColor(0,255,0);
	break;
	case 4:
	setColor(0,0,255);
	break;
	case 5:
	setColor(255,255,0);
	break;
}
fillCircle(100+(i*20),60+(i*20), 30);
}

HAL_Delay(500);

setColor(0,0,0);
fillRect(1,15,318,224);

// Draw some lines in a pattern
setColor (255,0,0);
for (int i=15; i<224; i+=5)
{
drawLine(1, i, (i*1.44)-10, 224);
}
setColor (255,0,0);
for (int i=224; i>15; i-=5)
{
drawLine(318, i, (i*1.44)-11, 15);
}
setColor (0,255,255);
for (int i=224; i>15; i-=5)
{
drawLine(1, i, 331-(i*1.44), 15);
}
setColor (0,255,255);
for (int i=15; i<224; i+=5)
{
drawLine(318, i, 330-(i*1.44), 224);
}

HAL_Delay(500);

setColor(0,0,0);
fillRect(1,15,318,224);

for (int i=0; i<10000; i++)
{
setColor(random(255), random(255), random(255));
drawPixel(2+random(316), 16+random(209));
}

HAL_Delay(500);

fillScr(0, 0, 255);
setColor(255, 0, 0);
fillRoundRect(80, 70, 239, 169);

setColor(255, 255, 255);
setBackColor(255, 0, 0);
print("That's it!", CENTER, 93, 0);
print("Restarting in a", CENTER, 119, 0);
print("few seconds...", CENTER, 132, 0);

setColor(0, 255, 0);
setBackColor(0, 0, 255);
print("Runtime: (secs)", CENTER, 210, 0);

printNumI(HAL_GetTick()/1000, CENTER, 225,1,' ');

HAL_Delay(5000);

```
