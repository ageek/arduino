Org code credit: http://www.instructables.com/id/ATTiny85-connects-to-I2C-OLED-display-Great-Things/

Tried flashing on digispark ATTiny85 kit, but it throws this error:
-------
Sketch uses 6700 bytes (111%) of program storage space. Maximum is 6012 bytes.

Global variables use 143 bytes of dynamic memory.
Sketch too big; see http://www.arduino.cc/en/Guide/Troubleshooting#size for tips on reducing it.
Error compiling for board Digispark (Default - 16.5mhz).
------
Reason being, 2k space is reserved in digispark kit for micronuclues bootloader, to make your life easier while flashing from IDE etc. You can reclaim the 2k space and flash digispark kit directly using ISP, similar to flashing other arduinos (google HV programming for arduino attiny85 etc, check it out: http://www.instructables.com/id/How-to-unlock-Digispark-ATtiny85-and-convert-it-to/). 
Yet to try, will update later with further details.

======
Update:
Switched to bare bones ATTiny85 chip, flashed using handmade ATTin85 flasher HAT for Uno. Works like a charm.

=====

Note: The hearbeat on OLED SSD1306 looks awesome !

