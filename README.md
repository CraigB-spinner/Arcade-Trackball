## **Arcade-Trackball** 
Arcade Trackball - based off Spinner version 2 <br/><br/>
[![Trackball](Pics/IMG_3843.jpg)](Pics/IMG_3843a.jpg)  
**Using an Arduino Pro Micro - DIY Trackball Controller**  
At sub $10 USD, it is a substitute to Ultimarc's U-HID Nano @ $35 + shipping - You do the HID device coding...<br/><br/> 
**Code to follow:** New additional y-axis interrupt & movement processing for both x/y axes handling.<br/><br/> 
**Oct. 12/20:** Code now availble, see extra pictures of finished Trackball box.  
**Dec. 16/20:** Added code for x-axis disable (Golf game driver - no hook/slice cheat).  
**Dec. 16/20:** Uploaded new code for override Joystick directions mod (using buttons for menu).  
**Jan. 15/21:** Uploaded new code for Trackball six colour rotation using a RGB 5050 LED Strip.  
<sup>(Use of programmer is required to unprogram fuse bit, JTAGEN = 1)</sup>  
**Jan. 26/21:** Uploaded new code for Mouse only Trackball operation. Note: all buttons can be inactivated for Trackball-Only operation.  
**Feb. 19/21:** Updated code for Trackball six colour RGB 5050 LED Strip.  
**Dec. 30/22:** Uploaded new code for Mouse vari-SpinSpeed Trackball (can be used in MiSTer FPGA Cores where device too sensitive)
<br/><br/>
This project uses an Arduino Pro Micro clone, arcade trackball(Atari/Happ style), with 8 buttons (up to 10) for use with MAME or any other emulator which can use x/y axes of mouse device as a trackball controller. Code should work on any board using an ATmega32U4 as long as port pins are mapped to same "digital pins" as [Pro Micro](Pics/Arduino%20Pro%20Micro%20Atmega32u4.jpg).  
To construct, you will need an older non-PS/2 style trackball which can operate at 5v along with your momentary switches affixed to an Arduino micro controller. 
This trackball, [RA-TRACKBALL-2](Pics/Retro%20Arcade%203in%20Trackball%20and%20Harness.jpg), uses [red Opto boards](Pics/Red_Board_Pinout.jpg), A052-1011-00, and not a green PS/2 Opto board pair. The green board pair can suffer from backspin. USB+PS2 Mouse MCU controller, HT82M98A, runs at 6 MHz and historically experience dropped signals of Quadrature transistions during rapid movement. Coding your own HID controller with ATmega32U4 is one reason why I used RA-TRACKBALL-2 Trackball, plus it is cheaper. It is unknown if a Jamma 60-in-1 (6-pin header) style trackball uses quadrature rotary encoder pulses for x/y axes like the Game Elf Jamma 138/276/352/485/619/1162-in-1, etc. (10-pin header/dual player) boards; or the board outputs PS/2 (DIR/CLK) protocol. AliExpress does show a Jamma 60-in-1 board with trackball using 6-pin header & 10-pin header harness, but would require a closer look at encoder boards.<br/> 
[**TrackBall**](https://www.ebay.ca/itm/230867558802?_trkparms=amclksrc%3DITM%26aid%3D1110006%26algo%3DHOMESPLICE.SIM%26ao%3D1%26asc%3D20220405142716%26meid%3D5e692133e19341d9aa7a86913013652e%26pid%3D101506%26rk%3D6%26rkt%3D10%26sd%3D233831655157%26itm%3D230867558802%26pmt%3D0%26noa%3D1%26pg%3D4481478%26algv%3DDefaultOrganicWebWithV11WebTrimmedV3VisualRankerWithKnnV3AndUltBRecall&_trksid=p4481478.c101506.m1851)(https://www.amazon.ca/gp/product/B00F1YQH6G/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1) used (clone of SuzoHapp style trackball by RetroArcade.us)<br/>
[RA-TRACKBALL No Harness](Pics/RA-TRACK-BALL-No-Harness.jpg), 
[RA-TRACKBALL-1 Jamma 60-in-1 Harness](Pics/RA-TRACKBALL-1-Jamma-60-in-1.jpg), 
[RA-TRACKBALL-2 Game Elf 412-in-1 Harness](Pics/RA-TRACKBALL-2-GameElf-412-in-1.jpg)<br/>
[**ATmega32U4**](https://www.amazon.ca/gp/product/B01N4TVIQX/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1) used (clone of SparkFun Pro Micro)<br/><br/> 

You will need Arduino joystick library at: [https://github.com/MHeironimus/ArduinoJoystickLibrary](https://github.com/MHeironimus/ArduinoJoystickLibrary) <br/>
Matthew Heironimus joystick code: [Joystick.cpp](https://github.com/MHeironimus/ArduinoJoystickLibrary/blob/master/src/Joystick.cpp) & 
[Joystick.h](https://github.com/MHeironimus/ArduinoJoystickLibrary/blob/master/src/Joystick.h) or 
Download: [master.zip](https://github.com/MHeironimus/ArduinoJoystickLibrary/archive/master.zip)  
In Arduino IDE:
- Select Sketch 
- Include Library 
- Add .ZIP Library... 
- Browse to downloaded ZIP file 
- Click Open

The Joystick library's examples will now appear under File > Examples > Joystick. <br/><br/>

This device will be detected as both mouse and gamepad/joystick. RetroPie requires joystick X/Y axes to be declared in library call for controller buttons to be detected.  Just center X/Y axes in setup() and forget, later code versions use Button/Joystick override for RetroArch menu navigation. The trackball controls X & y axes which by default is mapped to the mouse in MAME (don't forget to enable mouse in MAME under advanced options!). Buttons will work as regular gamepad/joystick buttons. Arcade 3" trackball rotates a 24 slot wheel with roller, gear ratio of about x7 (3.000"/0.425") = 169.412 slots/rev or 677.65 pulses/rev. Hand movement of 9.42"/rev. Comparing movement to my spinner (600p/r or 2400 pulses/rev), movement is effectively half, but can be adjusted in software for micro controller.  
Code uses Atmega32u4 ports directly because that's faster (over x30) than using digitalRead/Write. No button debouncing is setup, but can be added depending on your hardware issues.<br/><br/>

**Extra info:**<br/>
Wiring: [See diagram](Pics/Trackball%20Sketch_fix_bb.jpg), [2x5 pinout](Pics/Trackball%202x5%20pin%20connector.jpg), 
[2x5 conn](Finished%20pictures/ATmega32U4%20wired%20inside.jpg) image (lower left Red-Purple-Yellow)<br/>
Vcc (+5V) and Gnd output wires red and black<br/>
Trackball A(X<sub>B</sub>), B(X<sub>A</sub>), C(Y<sub>A</sub>) & D(Y<sub>B</sub>) output wires blue, purple, yellow and green.  
<sup>Colours match generic Game Elf 10 pin wire harness, Suzo/Happ 6 pin wire harness uses same colours but flip-flop X & Y axes.</sup><br/>
Note: resistor used in series to limit current around 10mA (390Ω	- 470Ω).  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Red Opto boards have LMC662CM (Dual Op Amp) acting as output buffer with 21-22mA limits<br/>
Buttons 1-6, plus 9-select(coin), and 10-start(player)<br/>

Pin |Data |Button/wire  |Code |2x5 Conn
--- |---- |------------ |---- |--------
0   |D1   |Y<sub>A</sub> trackball - Yellow |PD3/PinC|Ay- (5)
1   |D0   |Y<sub>B</sub> trackball - Green |PD2/PinD|Ay+ (4)
2   |D2   |X<sub>B</sub> trackball - Blue |PD1/PinA|Ax+ (2)
3   |D3   |X<sub>A</sub> trackball - Purple |PD0/PinB|Ax- (3)
4   |D4   |Button 1 - X
5   |D5   |Button 2 - A
6   |D6   |Button 3 - B
7   |D7   |Button 4 - Y
8   |D8   |Button 5 - L
9   |D9   |Button 6 - R
10  |D10  |Button 9 - Select
15  |D15  |Button 10 - Start
Gnd |     |Gnd - Black| |Gnd (10)
Vcc |     |+5v - Red| |+5v (1)
16  |D16  |Switch: X-Axis Opt.
14  |D14  |Switch: Joystick Opt.
A0  |F7/D23 |- - -
A1  |F6/D22 |RGB Blue<sub>Out</sub> Opt.
A2  |F5/D21 |RGB Green<sub>Out</sub> Opt.
A3  |F4/D20 |RGB Red<sub>Out</sub> Opt. <br/>


I believe the trackball encoder A/B output is reversed as you move the ball and not the roller directly. The code has been tested and trackball moves in the proper direction, so, the code was left as is. This Opto encoder board is also used for steerwheel devices.<br/>I am using Xbox A/B, X/Y configuration, Nintendo protocol is B/A, Y/X reversed controls (Japanese cultural differences). 
You can assign buttons to any assignment in RetroArch but it is best to pick a consistent pattern across all controllers to avoid confusion.  <br/>

Some [trouble shooting](trouble_shoot.md) with your controller  
   
**The Code:**<br/>
There are five versions of code: Pick your code to use
- Standard Mouse and Joystick buttons version  
  - [Trackball_8-Button.ino](Trackball_8-Button.ino)
- Special Mouse only version with & without Mouse buttons - no Joystick buttons
  - [Trackball_L-C-R-Button_Special.ino](Trackball_L-C-R-Button_Special.ino)
- Standard Mouse and Joystick buttons version - mod Joystick directions (use buttons for menu)
  - [Trackball_8-Button-mod.ino](Trackball_8-Button-mod.ino) <sub> 1.</sub><br/>
- Standard Mouse and Joystick buttons version - mod RGB Trackball and Joystick directions
  - [Trackball_8-Button-mod-rgb.ino](Trackball_8-Button-mod-rgb.ino) <sub> 1. 2.</sub> Update is available... parts arrived & tested.<br/>  
- Special Mouse vari--SpinSpeed version - 6 +2 +1 buttons  
  - [Trackball_6-Button-MiSTer.ino](Trackball_6-Button-MiSTer.ino)<br/>

<sub>1. Used two TTP223 Capacitive Touch switches to control special functions: the 
"[Touch Switches](Pics/Double%20Touch.jpg)" and 
"[Touch in Box](Pics/Special%20Touch%20Buttons.jpg)"  
Note: Invert the Touch I/O state by shorting [contact A to Vcc](Pics/TTP223B_chip_side.jpg), and/or contact B to activate on/off toggle mode.</sub>  
<sub>2. Trackball colour rotation using a [RGB 5050 LED Strip](Pics/RGB5050_board.jpg) and  
PORTF bits 4-6 set to output (requires fuse bit JTAGEN=1 be disabled) to control each colour LED (Red, Green Blue).  
**Proceed at own risk.**  
You may not require an ISP Programmer, if fuse bit JTAGEN=1 has been factory set. You will need to validate with LED and resistor anyway. 
Use caution when reprograming Fuse bits (L-FF, H-D8, E-C8) & Bootloader as incorrect settings may brick your microcontroller.  
Extra circuitry likely required to handle RGB LED current, microcontroller has 40mA current limits per bit without damaging Port output.  
[Inverting MOSFET circuit (Common Anode)](Pics/MOSFET-3-LED-Strip(red)-ca.jpg), 
[Inverting MOSFET circuit (Common Cathode)](Pics/MOSFET-3-LED-Strip(red)-cc.jpg) & 
[USBtinyISP Programmer & DIY Harness](Pics/USBtinyISP-Programmer+DIY-Harness.jpg) 
Note: [using MOSFETs](http://electronicsadventures.blogspot.com/2017/03/mosfet-switches-experiments.html)</sub>

**[How I Changed my Pro Micro Fuse bits](Changing_Fuse_Bits.md)**  

**My LED driver board:**  
required multiple redesigns due to 5050RGB LED strip board positive anode side switches, requiring use of P-Channel MOSFET. I also had issues sourcing some components locally.  
Here is my circuit x3 and required MOSFET driver board to switch each Red, Green, Blue LED segment (66mA @ 5V is too much for direct micro-controller driving). <br/>
[5050RGB circuit & pre-board](5050RGB_Pics/5050RGB%20circuit%20%26%20pre-board.jpg), 
[5050RGB wired LED strip](5050RGB_Pics/5050RGB%20strip%20wired.jpg), 
[5050RGB finished driver board](5050RGB_Pics/Finished%205050RGB%20driver.jpg) <br/> 
<sub>starting RGB White then dims from camera's auto-exposure (colours may differ in-person)</sub>  
<sup>➙White to Red, Yellow, Green, Cyan, Blue, Magenta...</sup>  
   <video autoplay loop muted>
      <source src="5050RGB_Pics/Trackball_RGB-120x90-1810.mp4" type="video/mp4">
   </video>
Here is the circuit design I used for 5050RGB strip: 
[My circuit](5050RGB_Pics/MOSFET-3-LED-Strip(red)-cc-3.jpg) and, 
[DIY circuit layout](5050RGB_Pics/RGB5050%20circuit%20Sketch_bb.jpg) <br/>
Components were sourced locally, P-MOSFET is overkill at 3.0A.  
<sup>(Note: Layout displays solder side & components located on other side; 
Brown and Purple links correspond to passive components) </sup>  

Other issues related to excessive current pull from added new Trackball RGB LEDs, required a separate USB Port/cable to be used to power close to 200mA current pull along with original USB cable to supply 100mA for micro-controller/switches/buttons and dual quadrature encoder boards from trackball. Trackball voltages at encoder boards were floating around 4.5V and operating without issues; however, when RGB LEDS were added voltage dropped to low 3V range triggering false signal pulses to the micro-controlller. A heavy duty USB cable capable of carrying current 3A or over (24AWG) may have solved the problem, but I just used a separate USB cable to supply the new needs of the RGB LEDs. Note: you must tie grounds(negative) together for equal Gnd levels; DO NOT tie +5v together.  

**Extra expenses:**  
Dupont connectors 2.54mm pitch JST SM kit (requires special crimper)  
or female Dupont pre-wired bundles with spare wire 24-28AWG (stranded wire for flexiblity),  
Sanwa - 2.8mm(0.110") or Suzo-Happ - 4.8mm(0.187") female spade crimp connectors for arcade button ends,  
Special Trackball lighting with [RGB 5050 LED Strip](https://www.amazon.ca/gp/product/B08FMB85WD/ref=ppx_yo_dt_b_asin_title_o03_s00?ie=UTF8&psc=1), and  
Special function switches with [TTP223 Capacitive Touch Switch](https://www.amazon.ca/gp/product/B07JDH7KHN/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1)  
<br/>
**For the more observant** in pictures 
"[Inside Spaghetti](Special%20Testing%20Pics/Inside%20spaghetti.jpg)" and 
"[Arduino Beauty Shot](Special%20Testing%20Pics/Arduino%20beauty%20shot.jpg)" 
there's an extra wire harness and aligator clip hooking to Gnd. TX0 & RX1 are wired to the y-axis of a proto-type [Trackball](https://www.amazon.ca/gp/product/B00F1YQH6G/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1) setup. Arduino code has been modified, tested and confirmed to work. No stutter or lag as using only USB protocol to RetroPie. No PS/2 Protocol overhead. I will be working on enclosure to finish new controller for games like **Centipede**, **Marble Madness**, **Crystal Castles**, and others.  

**Video:**  
[Trackball in Action](https://www.youtube.com/watch?v=E44fWpxooT4) playing **Centipede**<br/>
[Trackball in Action](https://youtu.be/Nwyvn5b7OW4) playing **Crystal Castles**<br/>

![RetroPie Logo](Pics/RetroPie%20Logo.png) RetroPie [laggy trackball](laggy_mouse.md) issues  

**More pictures:**   
The assembled pictures before adding clear finish to antique oak wood box 
"[See-thru-Top](Finished%20pictures/See-thru-Top.jpg)", 
"[Bottom-side](Finished%20pictures/Bottom-side.jpg)", 
"[ATmega32U4 wired inside](Finished%20pictures/ATmega32U4%20wired%20inside.jpg)", 
"[Buttons](Finished%20pictures/Buttons.jpg)", 
"[Trackball](Finished%20pictures/Trackball.jpg)".
The top, I used tinted Polycarbonate sheet (0.120" thick) found at a local surplus store and bottom from wood sitting in my parents basement from early 60's sourced from Stelco for handling steel. I later found out the wood was very dense, hard oak 100 to 120 years old from sapling. I snapped a few #4 screws while assembling which staled progress. Bottom panel is <sup>3</sup>/<sub>16</sub>" - <sup>1</sup>/<sub>4</sub>" plywood insert.  

**The Box:**  
The wood box measures 17" x 9" x 3⅞" in size, with a 19" x 11" Lexan top - [Trackball box](Pics/TrackBox%20Layout%20dimensions.png) dimensions;  
wood sides have <sup>7</sup>/<sub>16</sub>" rabbet butt joints glued, and <sup>7</sup>/<sub>16</sub>" rabbet along inside bottom edge to allow 15⅞" x 7⅞" plywood panel to recess in flush. The outside bottom edge has a ¼ round routered edge.  
Physical dimensions of [Trackball](Pics/RA-Trackball-2%20dimensions.jpg) are shimmed by four wooden blocks on right side, bottom side, and two on the box bottom. Block dimensions will vary with your own inside/outside box dimensions; leave space under ball to allow lighting and "Dust Bunny" particles to fall through. Please note: that if you compare idealized dimensions to what can be seen in finished pictures; it deviates somewhat to avoid an extended masochistic hand planing session leaving ones arms as chipper as a snail on a sloth.
