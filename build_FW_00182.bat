cd C:\ProgettiGitHub\firmware
del main.hex
make
avr-size -C --mcu=atmega328p main.elf
::https://www.avrfreaks.net/forum/easy-way-check-size-usedfree-ram
echo off
del main.d
del main.eep
del main.elf
del main.lss
del main.lst
del main.map
del main.o
del main.sym
del FW_IOTGEMINI.iot
ren main.hex FW_IOTGEMINI.iot
pause
