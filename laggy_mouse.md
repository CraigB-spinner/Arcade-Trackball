**On your computer,**  
use program Putty and SSH to your PI's ip address.  
`login: pi`  
`Password: raspberry` *(unless updated by you)*  

**Add:**  
*usbhid.mousepoll=0*  
to *cmdline.txt*

`sudo nano /boot/cmdline.txt`  
on the same line separated from other stuff by spaces  
This parameter enforces a mouse polling rate of ? 62.5Hz ? and reduces any quick movement lag.  
**0** or **1** - 1000Hz, **2** - 500Hz, **4** - 250Hz, **8** - 125Hz, **16** - 62.5Hz  
(Micro-controller is updating mouse as fast as main loop() and any axes interrupt(s) being serviced.)  

&nbsp;&nbsp;&nbsp;(using `lsusb` command {after installing} lists - ID *####*:*####* or Vender and Product-ID  
&nbsp;&nbsp;&nbsp;for your custom Trackball controller;  using grep as root to find Iv1=polling interval, to be  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**1ms or 1000Hz**  
&nbsp;&nbsp;&nbsp;`grep -B3 -A15 ####.*#### /sys/kernel/debug/usb/devices`  
&nbsp;&nbsp;&nbsp;device info starts with T: line, and ends with E: line  
&nbsp;&nbsp;&nbsp;find I: line with driver=usbhid or hid)  

`sudo reboot`

Under Arcade or MAME select your favourite trackball game.  
TAB to bring up the Game Menu  
Select *Analog Controls*  
Adjust the *X Sensitivity* & *Y Sensitivity* values 
