/*   Arcade Trackball/Spinner v2.02
*    (new trackball/mouse dual x and y axis - added pinC & pinD - RX1/PD2, TX0/PD3 data pins)
*    Special Mouse version with Left/Centre/Right Buttons or
*            Mouse version without buttons - JustSpin directive.
*
*    Craig B  - (craigbspinner@gmail.com)
*    Copyright 2020 Craig B - based on Spinner code from early June 2019 
*                   Craig B - Updated 70% of code, 
*                             Added spinner movement ACCM/DROP, 
*                             Optimized code speed by changing if logic to case statement logic for Button validation of port bits.
*                   Craig B - Added code for spinner movement NORM/ACCS/ACCX - End of June 2019         
*                   Craig B - Added code for mouse y-axis and interupt processing from late Aug 2020
*                   Craig B - Added ACCSX code for mouse larger movement for both axes late Nov 2020 (issue with mouse sensitivity in MAME)
*                   Craig B - Added code for x-axis disable (golf drive - no hook/slice cheat) from mid Dec 2020   
*                   Craig B - Special mouse only code - Left, Centre, Right and x-axis disable buttons from late Jan 2021   
*                           - No button mode, JustSpin 
*    
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Mouse.h"
#include <Joystick.h>

/*   RetroPie mods - Arduino IDE -> Tools, Get Board Info
*    VID: 1B4F
*    PID: 9206
*    Add the following to /boot/cmdline.txt -- just add a space to the end of the existing entry, so it is all on one line):
*    usbhid.quirks=0x16c0:0x05e1:0x040
*    The first two numbers are the vendor ID 16c0 and product ID 05e1.
*/

// Port Bit/Pin layout   
//      Bit - 76543210 - Silk screen ## - Micro-Controller
#define xPB3 0b00001000 //Digital Pin 0  - Micro/PRO Micro - RX,  INT2
#define xPB2 0b00000100 //Digital Pin 1  - Micro/PRO Micro - TX,  INT3
#define xPB1 0b00000010 //Digital Pin 2  - Micro/PRO Micro - SDA, INT0
#define xPB0 0b00000001 //Digital Pin 3  - Micro/PRO Micro - SCL, INT1
#define xPD4 0b00010000 //Digital Pin 4  - Micro/PRO Micro
#define xPC6 0b01000000 //Digital Pin 5  - Micro/PRO Micro
#define xPD7 0b10000000 //Digital Pin 6  - Micro/PRO Micro
#define xPE6 0b01000000 //Digital Pin 7  - Micro/PRO Micro
#define xPB4 0b00010000 //Digital Pin 8  - Micro/PRO Micro
#define xPB5 0b00100000 //Digital Pin 9  - Micro/PRO Micro
#define xPB6 0b01000000 //Digital Pin 10 - Micro/PRO Micro
#define xPB7 0b10000000 //Digital Pin 11 - Micro
#define xPD6 0b01000000 //Digital Pin 12 - Micro
#define xPC7 0b10000000 //Digital Pin 13 - Micro
#define xPB3 0b00001000 //Digital Pin 14 - PRO Micro
#define xPB1 0b00000010 //Digital Pin 15 - PRO Micro
#define xPB2 0b00000100 //Digital Pin 16 - PRO Micro

//   Arcade Trackball/Mouse requires ACCS movement.
//   axis wheel is 24T with 3" ball, 0.425" roller -> 24 x (3000/425) = 169.412  holes/revs
//   4 edge transistions Hi/Low and Low/Hi x 2 sensors per axis = 169.412 x 4 = 677.647 pulses/revs 
//   Double sensor required to indicate rotation direction in single axis 
//
//   Arcade Spinner/Mouse requires ACCM movement.
//   2400 pulses/revs divide by two
    
//Mouse movement - pick one to compile specific code inside loop()
//#define NORM   //Mouse movement Normal; every interrupt processed; maybe lag in fast movement, 2 pulse = 2 move
//#define DROP   //Mouse movement Drop; drop extra interrupts processed; smooth movement, no lag, 2 - 2n+1 pulse = 1 move
#define ACCS   //Mouse movement Accelerated; every interrupt processed but accelerated movement, no lag x1, 1 pulse = 1 move 
#define ACCSX 1   //Mouse movement Accelerated; every interrupt processed but 2x accelerated movement, no lag x1, 1 pulse = 2 move (use with ACCS)
//#define ACCSX 2   //Mouse movement Accelerated; every interrupt processed but 4x accelerated movement, no lag x1, 1 pulse = 4 move (use with ACCS)
//#define ACCM   //Mouse movement Accelerated; every interrupt processed but accelerated movement, no lag x2, 2 pulse = 1 move
//#define ACCX   //Mouse movement Accelerated; every interrupt processed but accelerated movement, no lag x4, 4 pulse = 1 move

//#define JustSpin //Mouse movement only logic - NO BUTTON CODE - Note: Must disable axisEnable or code will not compile properly.

#define pinA 2    //The pins that the trackball/mouse x-axis encoder's A and B terminals are connected to.
#define pinB 3
#define pinC 0    //The pins that the trackball/mouse y-axis encoder's C and D terminals are connected to.
#define pinD 1
#define maxBut 4        //Update lastButtonState array below when changing number of elements.
#define axisEnable 3  //Special x-axis disable button (button offset: 0 thru 9) - comment out if 'x-axis disable' feature not required by you.
#define butPrs 0      //Button pulled low when pressed
#define butOff 1      //Button pullup high when not pressed

//The previous state of the AB pins
volatile int prevQuadratureX = 0;
//The previous state of the CD pins
volatile int prevQuadratureY = 0;

//Keeps track of how much the encoder has been moved
volatile int rotPositionX = 0;
volatile int rotMultiX = 0;
volatile int rotPositionY = 0;
volatile int rotMultiY = 0;

#ifndef JustSpin
#ifdef axisEnable
volatile int xAxis = 1;  //enable/disable interrupt counter of rotPositionX x-axis

// Last state of 4 buttons (update array for your maxBut buttons)
// Since Button Port Bits are set to 1 to activate(pull-hi input resistors), the press state is low, and not pressed is high.
  int lastButtonState[maxBut] = {1,1,1,1};
#else
  int lastButtonState[maxBut] = {1,1,1,0};
#endif
#endif


//---------------------------------------------------------------------
void setup() {
  //No need to set the pin modes with DDRx = DDRx | 0b00000000 as we're using all input and that's the initial state of the pins
  //Setup for three mouse buttons Left, Centre, Right, plus special axisFlip control.
  //Use internal input resistors for all the pins we're using - pull high resistor, when button pressed (short to ground) input on pin is pulled low.
  //Digital pin D# corresponds to silk screen on micro-controller board, (#) corresponds to port bit internal use MSD(7)-to-LSD(0) - you assign button # in code 
#ifdef JustSpin
  PORTD = 0b10010011; //Digital pins D2(1), D3(0)
#endif
#ifndef JustSpin
  PORTD = 0b10010011; //Digital pins D2(1), D3(0), D4(4), and D6(7). (Button Left and Right)
  PORTC = 0b01000000; //Digital pin D5(6)                            (Button Centre)
//PORTB = 0b00000101; //Digital pin D16(2) requires PB0 set high(master)(Button Special)
  PORTE = 0b01000000; //Digital pin D7(6)                            (Button Special)
//
//PORTB = 0b00010000; //Digital pin D8(4). D9(5), D10(6), D15(1), D14(3), D16(2) requires PB0 set high(master)
//PORTB = 0b01110010; //Digital pins D8(4), D9(5), D10(6), and D15(1). D14(3), D16(2) requires PB0 set high(master)
//PORTB = 0b01110111; //Digital pins D8(4), D9(5), D10(6), and D15(1). D14(3), D16(2) requires PB0 set high(master)
//PORTF = 0b11000000; //Digital pin A0(7) & A1(6). A2(5), A3(4) 
#endif

  //Set up the interrupt handler for the encoder's A and B terminals on digital pins 2 and 3 respectively. Both interrupts use the same handler.
  attachInterrupt(digitalPinToInterrupt(pinA), pinChangeX, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(pinB), pinChangeX, CHANGE);
  //Set up the interrupt handler for the encoder's C and D terminals on digital pins 0 and 1 respectively. Both interrupts use the same handler.
  attachInterrupt(digitalPinToInterrupt(pinC), pinChangeY, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(pinD), pinChangeY, CHANGE);

  //Start the mouse
  Mouse.begin();
}

//may want to add mouse left, mid, right button press functionality instead joystick buttons at code bottom
//char button MOUSE_LEFT(default), MOUSE_RIGHT, MOUSE_MIDDLE
//Mouse.press(button)   
//Mouse.release(button) 
//Mouse.click(button)  
//Mouse.isPressed(button)  returns bool 

//---------------------------------------------------------------------
//Interrupt handler x-axis
void pinChangeX() {

  //Set currQuadratureX to state of rotary encoder terminals A & B from input of PORTD bits 0 & 1 (digital pins 2 and 3)
  //You could do int currQuadratureX = (digitalRead(pinA) << 1) | digitalRead(pinB); to get the same thing, but it would be much slower.
  //Read current state 00AB.
  int currQuadratureX = PIND & 0b00000011;

  //Store comboQuadratureX with previous and current quadrature rotary encoder states together. 
  //Combined previous/current states form two groups of four unique bit patterns indicating direction of movement.
  //Shift left 2 bits: 00AB <- AB00 to store current state as previous state.
  int comboQuadratureX  = (prevQuadratureX << 2) | currQuadratureX; 

  //Rotate to the right, Clockwise
  //State 00AB, A leads B. A0 -> AB -> 0B -> 00 0b0010 0b1011 0b1101 0b0100
  if(comboQuadratureX == 0b0010 || comboQuadratureX == 0b1011 ||
     comboQuadratureX == 0b1101 || comboQuadratureX == 0b0100) {
  #ifdef axisEnable
     rotPositionX += (xAxis);          //update the position of the encoder
  #else
     rotPositionX++;                   //update the position of the encoder
  #endif
  }

  //Rotate to the left, Counter Clockwise
  //State 00AB, B leads A. 0B -> AB -> A0 -> 00 0b0001 0b0111 0b1110 0b1000
  if(comboQuadratureX == 0b0001 || comboQuadratureX == 0b0111 ||
     comboQuadratureX == 0b1110 || comboQuadratureX == 0b1000) {
  #ifdef axisEnable
     rotPositionX -= (xAxis);          //update the position of the encoder
  #else
     rotPositionX--;                   //update the position of the encoder
  #endif
  }

  //Save the previous state of the A and B terminals for next time
  prevQuadratureX = currQuadratureX;
}


//---------------------------------------------------------------------
//Interrupt handler y-axis
void pinChangeY() {

  //Set currQuadratureY to state of rotary encoder terminals A & B from input of PORTD bits 2 & 3 (digital pins 0 and 1) 
  //You could do int currQuadratureY = (digitalRead(pinC) << 1) | digitalRead(pinD); to get the same thing, but it would be much slower.
  //Read current state CD00.
  int currQuadratureY = PIND & 0b00001100;

  //Store comboQuadratureY with current and previous quadrature rotary encoder states together. 
  //Combined current/previous states form two groups of four unique bit patterns indicating direction of movement.
  //Shift right 2 bits: CD00 -> 00CD to store current state as previous state.
  int comboQuadratureY  =  currQuadratureY | (prevQuadratureY >> 2); 

  //Rotate down, Counter Clockwise
  //State CD00, C leads D. C0 -> CD -> 0D -> 00 0b1000 0b1110 0b0111 0b0001
  if(comboQuadratureY == 0b1000 || comboQuadratureY == 0b1110 || 
     comboQuadratureY == 0b0111 || comboQuadratureY == 0b0001) { 
     rotPositionY++;                   //update the position of the encoder
  }

  //Rotate up,  Clockwise
  //State CD00, D leads C. 0D -> CD -> C0 -> 00 0b0100 0b1101 0b1011 0b0010
  if(comboQuadratureY == 0b0100 || comboQuadratureY == 0b1101 ||
     comboQuadratureY == 0b1011 || comboQuadratureY == 0b0010) {
     rotPositionY--;                   //update the position of the encoder
  }

  //Save the previous state of the C and D terminals for next time
  prevQuadratureY = currQuadratureY;
}


//---------------------------------------------------------------------
//Process the x/y axis mouse movement using one criteria (NORM, DROP, ACCS, ACCM, ACCX)
//most criteria process two edge transistions before moving the mouse/trackball pointer.
//Then cycle thru four button states 1, 2, 3, 4, to trigger button press/release on mouse
void loop(){ 
  int currentButtonState;

#ifdef NORM
  //If the encoder has moved 2 or more transitions move the mouse in the appropriate direction 
  //and update the position variable to reflect that we moved the mouse. Smooth movement but lags.
  if(rotPositionX >= 2) {
    Mouse.move(1,0,0);
    rotPositionX -= 2;
  } else if(rotPositionX <= -2) {
    Mouse.move(-1,0,0);
    rotPositionX += 2;
  } 
  if(rotPositionY >= 2) {
    Mouse.move(0,1,0);
    rotPositionY -= 2;
  } else if(rotPositionY <= -2) {
    Mouse.move(0,-1,0);
    rotPositionY += 2;
  } 
#endif 

#ifdef DROP
  //If the encoder has moved 2 or more transitions move the mouse in the appropriate direction 
  //and update the position variable to reflect that we moved the mouse. Smooth drop extra moves.
  if(rotPositionX >= 2) {
    Mouse.move(1,0,0);
    rotPositionX = 0;
  } else if(rotPositionX <= -2) {
    Mouse.move(-1,0,0);
    rotPositionX = 0;
  } 
  if(rotPositionY >= 2) {
    Mouse.move(0,1,0);
    rotPositionY = 0;
  } else if(rotPositionY <= -2) {
    Mouse.move(0,-1,0);
    rotPositionY = 0;
  } 
#endif 

#ifdef ACCS
  //If the encoder has moved 1 or more transitions move the mouse in the appropriate direction 
  //and update the position variable to reflect that we moved the mouse. Accelerated move.
  if(rotPositionX != 0 || rotPositionY != 0) {
#ifdef ACCSX
    rotMultiX = rotPositionX << ACCSX;
    rotMultiY = rotPositionY << ACCSX;
    Mouse.move(rotMultiX,rotMultiY,0);
#else
    Mouse.move(rotPositionX,rotPositionY,0);
#endif 
    rotPositionX = 0;
    rotPositionY = 0;
  }
#endif 

#ifdef ACCM
  //If the encoder has moved 2 or more transitions move the mouse in the appropriate direction 
  //and update the position variable to reflect that we moved the mouse. Accelerated move.
  if(rotPositionX >= 2 || rotPositionX <= -2) {
    rotMultiX = rotPositionX >> 1;
    Mouse.move(rotMultiX,0,0);
    rotPositionX -= (rotMultiX << 1);
  }
  if(rotPositionY >= 2 || rotPositionY <= -2) {
    rotMultiY = rotPositionY >> 1;
    Mouse.move(0,rotMultiY,0);
    rotPositionY -= (rotMultiY << 1);
  }
#endif 

#ifdef ACCX
  //If the encoder has moved 4 or more transitions move the mouse in the appropriate direction 
  //and update the position variable to reflect that we moved the mouse. Accelerated move.
  if(rotPositionX >= 4 || rotPositionX <= -4) {
    rotMultiX = rotPositionX >> 2;
    Mouse.move(rotMultiX,0,0);
    rotPositionX -= (rotMultiX << 2);
  }
  if(rotPositionY >= 4 || rotPositionY <= -4) {
    rotMultiY = rotPositionY >> 2;
    Mouse.move(0,rotMultiY,0);
    rotPositionY -= (rotMultiY << 2);
  }
#endif 

#ifndef JustSpin
  //Iterate through the 4 buttons (0-3) assigning the current digital state of pin for each button, 
  //HIGH(0b00000001) or LOW(0b00000000), to currentState variable. 
  //Note: Pro Micro boards use various ports to access different digital pins. 
  //Buttons 1-4 used; buttons 5 - 10 unused
  //Using digital pins 4, 5, 6, and 7 on Pro Micro board. See silk screening.
  //Pins 8, 9, 10, 14, 15 and 16 are free to use.
  //Pins 0, 1, 2 and 3 are used for trackball rotary output as they are interrupt driven.
  int button = 0;
  do {
    switch ( button ) {
      case 0:  //on digital pin 4, PD4 - Arcade Button 1
        currentButtonState = (PIND & xPD4) >> 4;
        break;
      case 1:  //on digital pin 5, PC6 - Arcade Button 2
        currentButtonState = (PINC & xPC6) >> 6;
        break;
      case 2:  //on digital pin 6, PD7 - Arcade Button 3
        currentButtonState = (PIND & xPD7) >> 7;
        break;
    #ifdef axisEnable
      case 3:  //on digital pin 7, PE6 - Arcade Button 4
        currentButtonState = (PINE & xPE6) >> 6;
//      case 3:  //on digital pin 16, PB2 - Special Axis Button (internal function) - requires PB0 set to master or high
//        currentButtonState = (PINB & xPB2) >> 2;
        break;
    #endif
//      case 4:  //on digital pin 8, PB4 - Arcade Button 5
//        currentButtonState = (PINB & xPB4) >> 4;
//        break;
//      case 5:  //on digital pin 9, PB5 - Arcade Button 6
//        currentButtonState = (PINB & xPB5) >> 5;
//        break;
//#ifdef axisEnable
//      case 6:  //on digital pin 16, PB2 - Special Axis Button (internal function) - requires PB0 set to master or high
//        currentButtonState = (PINB & xPB2) >> 2;
//        break; 
//#endif
//      case 7:  //on digital pin 14, PB3 - 2nd Special Button 
//        currentButtonState = (PINB & xPB3) >> 3;
//        break; 
//      case 8:  //on digital pin 10, PB6 - COIN/Select Button 9
//        currentButtonState = (PINB & xPB6) >> 6;
//        break;
//      case 9:  //on digital pin 15, PB1 - PLAYER/Start Button 10
//        currentButtonState = (PINB & xPB1) >> 1;
//        break; 
      default: //Extra digital pins 16, PB2 + PB0(master=hi) and 14, PB3
        currentButtonState = 0b00000000;
        break;
    }
    //If the current state of the pin for each button is different than last time, update the joystick button state
    if(currentButtonState != lastButtonState[button]) {

//  button press state is low, and not pressed is high
//  if button and state logic to control Mouse.press() or Mouse.release() activity
      switch ( button ) {
        case 0:  //Left Mouse Button 
          if (currentButtonState == butOff) 
            Mouse.release(MOUSE_LEFT);
          else 
            Mouse.press(MOUSE_LEFT);
          break;
        case 1:  //Centre Mouse Button
          if (currentButtonState == butOff) 
            Mouse.release(MOUSE_MIDDLE);
          else 
            Mouse.press(MOUSE_MIDDLE);
          break;
        case 2:  //Right Mouse Button
          if (currentButtonState == butOff) 
            Mouse.release(MOUSE_RIGHT);
          else 
            Mouse.press(MOUSE_RIGHT);
          break;
      #ifdef axisEnable
        case 3:  //Special Axis Enable Button 
          if (button == axisEnable )  //change x-axis enable status
            xAxis = !xAxis;
          break;
      #endif
      }
    }
      
    //Save the last button state for each button for next time
    lastButtonState[button] = currentButtonState;

    ++button;
  } while (button < maxBut);
#endif
}
