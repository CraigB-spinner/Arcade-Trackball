# Arcade-Trackball
Arcade Trackball - based off Spinner version 2


This is an Arduino Pro Micro clone, arcade trackball, with 8 buttons (up to 10) for use with MAME or any other emulator which can use the X axis and y-axis of the mouse as a trackball controller. This code should also work on any board which uses the ATmega32U4 as long as the port pins are mapped to the same "digital pins" as the Micro.  
To construct this you will need an older style non-PS/2 trackball which can operate at 5v and some momentary switch buttons plus Arduino micro controller. 
This trackball uses the red Opto boards A052-1011-00, and not the green PS/2 Opto board pair.<br/> 
[**TrackBall** used](https://www.amazon.ca/gp/product/B00F1YQH6G/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)<br/>
[**ATmega32U4** used](https://www.amazon.ca/gp/product/B01N4TVIQX/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)<br/>
<br/>

You will also need the Arduino joystick library available at: https://github.com/MHeironimus/ArduinoJoystickLibrary<br/><br/>

This device will be detected as both mouse and joystick/gamepad. The joystick only has X and Y axes which we don't use. At least one axis is needed by the library to declare the joystick and it was found that 2 were required for the controller to be detected on some systems (RetroPi). We just center the X and Y axes in setup() and forget about them. The trackball controls the mouse X & y axes which is by default mapped to the mouse in MAME (don't forget to enable the mouse in MAME under advanced options!). The buttons will work as regular gamepad/joystick buttons. The 3" trackball is attached to a 24 slot wheel with gear(roller) ratio of about x7 (3.000"/0.425") = 169.412 slots/rev and  677.65 pulses/rev This less than a spinner 600p/r or 2400 pulses or half effective movement in the micro controller software. Hand movement of 9.42"/rev

different positions (transitions) that can be detected on the 600ppr(pulse per revolution) encoder I'm using are way too many for our purposes so they are halved in the code to 1200. The code uses the Atmega32u4 ports directly because that's faster than using digitalRead/Write. I'm not doing any debouncing of the buttons or encoder as it seems to work great for me as is, but you might want to add debouncing depending on your hardware.<br/><br/>

**Extra info:**<br/>
Wiring: <br/>
Vcc (+5V) and Gnd<br/>
Spinner A & B rotary output wires red and green. see picture <br/>
Note: resistor used in series to limit current around 10mA (390Ω	- 470Ω)  
I advise you double check Vcc(5-24v power - Wht) and Gnd(Blk) wire colours as some chinese factories are colour blind.<br/>
Buttons 1-6, plus 9-select(coin), and 10-start(player)<br/>
Pin |Button/wire
--- |-----------
2   |A spinner
3   |B spinner
4   |Button 1 - X
5   |Button 2 - A
6   |Button 3 - B
7   |Button 4 - Y
8   |Button 5 - L
9   |Button 6 - R
10  |Button 9 - Select
15  |Button 10 - Start
Gnd |Gnd
Vcc |+5v  <br/>

I am using Xbox A/B, X/Y configuration, Nintendo protocol is B/A, Y/X reversed controls (Japanese cultural differences)  
You can assign buttons to any assignment in RetroArch but it is best to pick a consistent pattern across all controllers to avoid confusion.  <br/>
 
**Extra expenses:**  
Dupont connectors 2.54mm pitch JST SM kit (requires special crimper)  
or female Dupont pre-wired bundles with spare wire 24-28AWG (stranded wire for flexiblity)  
Sanwa - 2.8mm(0.110") or Suzo-Happ - 4.8mm(0.187") female spade crimp connectors for arcade button ends 
<br/><br/>
**For the more observant** in pictures "Inside Spaghetti" and "Arduino Beauty Shot" there's an extra wire harness and aligator clip hooking to Gnd. TX0 & RX1 are wired to the y-axis of a proto-type [Trackball](https://www.amazon.ca/gp/product/B00F1YQH6G/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1) setup. Arduino code has been modified, tested and confirmed to work. No stutter or lag as using only USB protocol to RetroPi. No PS/2 Protocol overhead. I will be working on enclosure to finish new controller for games like **Centipede**, **Marble Madness**, **Crystal Castles**, and others.  
