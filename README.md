# cpp-arduino-tm1637

## Control LCD display tm1637 from a arduino

Original work from [https://github.com/Seeed-Studio/Grove_4Digital_Display](https://github.com/Seeed-Studio/Grove_4Digital_Display)

You can control a LCD tm1637 with facultative semi-colon separator

Allowed characters : [aA-zZ],[0-9], ,°,-,=,[,],(,)

Usage:
```
    #include "TM1637.h"
    int CLKPIN=2;
    int DIOPIN=3;
    TM1637 tm1637(CLKPIN,DIOPIN);
    tm1637.set();
    tm1637.init();
    tm1637.show("HELO");
```
If you specify less than 4 characters the alignement is automatically set on the right (all characters beyond the 4th will be ignored)
```
    tm1637.show("A");   //Will output "   A" on the lcd
```
You can set a character at a specified position (0 to 3)
```
    tm1637.show(0,'H');
    tm1637.show(1,'E');
    tm1637.show(2,'L');
    tm1637.show(3,'O');
```
Set the semi-colon separator that way
```
    tm1637.point(true);
    tm1637.show("1035");    //Will output "10:35" on the lcd
```
