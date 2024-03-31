# ar-due-pi-mega
just controlling my chips

I am starting to get the hang of this electronic business.
This is a C program to control an arduino due via i2c.

The due is controlling an atmega328p chip on a bread board.

The C program is being written on a raspberry pi 4, which serves as the Master.

Since the logic level of the pins on an Rpi 4 is 3v3, it works well with the due
which is a 3v3 logic level micro controller. The Rpi is a good computer for running on
5V but is not super great for peripherals. Good, just not great. The Arduinos are
only good for peripherals. Not just good but great.

The due, being 3v3, is triggering a relay which turns on the power for the atmega-chip.

The atmega chip is set up for a little surprise thing I am working on.

The main purpose of this repo is to be able to move this code over to the living room
to make my other Rpi in there the master of my other due. :D 

I have a few more, ehhm, involving projects coming up soon. somewhere, some time.
