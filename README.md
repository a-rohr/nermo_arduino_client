# Nermo Arduino Client
The arduino client is designed for Nermo-Lite. The arduino client running on the hardware model of Nermo that enables communication and control with the ROS backend of the mouse controller. The client does not handle the main controller computations. These are computed on a central computer using the [mouse_controller](https://github.com/a-rohr/nermo_compliant_robot_mouse) ROS modules.

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

## Hardware Changes Nermo-Lite
Next to the electrical design changes, Nermo-Lite also has several CAD-model based updates. The most important updates can be summarized as follows:

1. Simpler tendon routing in the spine
2. Removal of second spine motor (and therefore closing the spine)
3. Elimination of vertical spine axis
4. Adding more cable routing options in the spine
5. Simplification of various detail components throughout the mouse
6. Slightly larger components to accommodate the un-modified servo motors (these require more space than the modified servo motors).
7. Change of the leg mount flanges from hexagon to square for better compensation of tolerances.

The parts are designed to be printed using a standard desktop 3d printer (FDM based).

## Bill of Material
All prices below are based on EU prices. 
|       Item                               | Quantity | Cost/Item (EUR) | Total (EUR) |
| -------------                            | -------- | --------------  | ----------- |
| DSM 44 Digital Servo                     |    11    |        5.00     |    55.00    | 
| Adafruit 12-channel 16-bit PWM Driver    |     1    |        5.99     |     5.99    |
| ESP-01s                                  |     1    |        2.50     |     2.50    |
| LiPo Buck Converter 6V/5A                |     1    |        9.99     |     9.99    |
| PETG Filament for skeleton               |     1    |       23.99     |    23.99    |
| LiPo 7.4V 500mAh                         |     1    |        6.99     |     6.99    |
| Micro-circuit components                 |     1    |        4.00     |     4.00    |
| Auxiliay components (screws, wires, ...) |     1    |       10.00     |    10.00    |
| --------------------------------------   | -------- | --------------- | ----------- |
|                                          |          |   TOTAL         |   118.46    |
