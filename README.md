Engineering materials
====

This repository contains engineering materials of a self-driven vehicle's model participating in the WRO Future Engineers competition in the season 2024 made by team RoboChampions.

## Content

* `Team Pictures` contains 1 photo of the team 
* `Vehicle Pictures` contains 6 photos of the vehicle 
* `Bot Videos` contains the video.md file with the link to a video where driving demonstration exists
* `Electronics` contains one schematic diagram in form of a PDF and a the bill_of_materials.md file with the BOM (bill of materials) of the electromechanical components illustrating all the elements (electronic components and motors) used in the vehicle and how they connect to each other.
* `Codes` contains code of control software for all components which were programmed to participate in the competition
* `CAD` is for the files for models used by 3D printers and laser cutting machines to produce the vehicle elements. It also contains the bot itterations

## Introduction
The vehicle uses an Arduino UNO R3 as a controler and a pixy for image processing which is conected to the arduino as shown in the circuit diagram

### Arduino
The code can be made, compiled and uploaded to the arduino using the Arduino IDE software and a USB A to USB B cable.

### Pixy
To upload the pixy parameters file (.prm) the pixy2 software should be used and the pixy can be conected with the laptop using a micro USB cable.

### Libraries 
The following libraries are used in the code and need to bes installed in arduino IDE:
* Pixy2 (pixy cominication library)
* Wire.h (i2c library)
* Servo (servo library)
* Adafruit_BNO055 (BNO gyro library)
* utility/imumaths (used for IMU calculations)
* Adafruit_NeoPixel (used to control neo pixel LED headlights)
