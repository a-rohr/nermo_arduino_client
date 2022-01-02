# Nermo Arduino Client
The arduino clinet is designed for Nermo-Lite. The arduino client running on the hardware model of Nermo that enables communication and control with the ROS backend of the mouse controller. 

## Nermo-Lite
Nermo-Lite is a simpler, scaled-down version of Nermo that makes use of primarily off-the-shelf, unmodified components. The skeleton of Nermo has also been redesigned for Nermo-Lite to be printable on a regular desktop FDM printer, rather than the more complex and expensive SLS printers. 
![nermo-lite](https://user-images.githubusercontent.com/46085397/147863006-0376eea8-847f-4a26-bc2b-d0237f4897b4.png)

Unlike Nermo, Nermo-Lite only uses 11 motors (vs. 13 on Nermo). The second spine flex motor and the head tilt motor have been omitted. Nermo-Lite also uses a simple ESP8266 as the hardware controller vs. the Raspberry Pi Zero of Nermo. Therefore, the local compute power of Nermo-Lite is fairly limited. 

## Electrical Setup
FDM Nermo uses as many off-the-shelf components as possible and tries to reduce the amount of custom work. Nevertheless, a little bit of soldering & custom board making is required. The schematic of the main electrical components is shown in the diagram below. The 11 servo motors are then connected to the 3-pin pinouts of the PCA9685 driver board. 

![Schematic_Nermo Embedded ESP01s_2021-09-02 (1)](https://user-images.githubusercontent.com/46085397/147862978-67376865-afab-4087-813f-676dcc587800.png)

This here is a simplified schematic setup of the electrical components. A more updated version of the electrical scheme including a custom connection board can be found here: [https://oshwlab.com/alexrohregger96/connector_board](https://oshwlab.com/alexrohregger96/connector_board). Click on "open in editor" to see the latest version of the schematics and boards.

## Important Client Setup

The arduino client code is uploaded to the ESP-01s using a USB writer. Communication occurs with a local Wifi-Hotspot. Therefore, the SSID, password and ROS-Master IP needs to be updated. The ESP-01s connects to the ROS Master server running on the main computer (set with the server IPAddress) using the rosserial library from [here](http://wiki.ros.org/rosserial). The master computer then sends the desired servo commands via rosserial to the connected ESP-01s, which are then sent via I2C to the PWM Servo Driver. 
