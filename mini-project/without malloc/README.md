# Scientific Graphing Tool Using Touch LCD

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [Setup](#setup)
- [Code Structure](#code-structure)
- [Authors](#authors)

## Introduction

In this Project, we have built a scientific graphing tool on a touchscreen module as an embedded project, which has an enhanced user experience, i.e., portable and with touchscreens offering a more intuitive interface compared to traditional button-based input systems. Like the Desmos graphing calculator website,  we have built a similar utility hardware tool. Here, we have tried to accommodate the maximum features of the calculator by integrating a touchscreen through a microcontroller.

<img src=".\Assets\main-panel.jpg" alt="Alt Text" width="500">
<img src=".\Assets\first.jpg" alt="Alt Text" width="500">

## Features

Several features are available in the graphing tool. We have implemented eight such features.

1. **Plotting graph**
2. **Derivative tool**
3. **Integral tool**
4. **Zoom in and out**
5. **Shift left and right**
6. **Area under the curve**
7. **Zeros of the function**
8. **Hide/Show coordinate data**

## Requirements

- **Hardware:** BOOSTXL-K350QVG-S1 Kentec QVGA Touch Screen Graphics Display, TIVA C Series Microcontroller (TM4C123GH6PM)
- **Software:** Code Composer Studio IDE

## Setup

1. **TIVA Hardware Setup**
    1. Configuring the interface (8-bit or 16-bit) depending on the host controller capabilities.
    2. Initializing the SSD2119 controller to set up the screen resolution, color depth, and other features like scrolling and partial display modes.
    3. Require to remove the resistor <R9> and <R10> on TIVA microcontroller for the correct functioning of SPI. <img src=".\Assets\tiva_with_R.png" alt="Alt Text" width="400"> <img src=".\Assets\tiva_without_R.png" alt="Alt Text" width="400">
    4. Connect through the pins in proper alignment pin configurations <J2,J4> and <J1,J3>. <img src=".\Assets\tiva_back.png" alt="Alt Text" width="350"> <img src=".\Assets\tiva_front.png" alt="Alt Text" width="400">
2. **Code Composer Studio Setup**
    1. Since we will be using the libraries specific to the Senshub (BOOSTXL-K350QVG-S1 Kentec QVGA) module, programming the code using the TI compiler is preferable as shown here. <img src=".\Assets\ccs1.jpg" alt="Alt Text" width="700">
    2. Add relevant Path variables to include the necessary files from the TIVA installation directory. Primarily include the following:
        1. <ORIGINAL_PROJECT_ROOT> - ..\TivaWare_C_Series-2.2.0.295\examples\boards\ek-tm4c123gxl-boostxl-kentec-s1\grlib_demo\ccs
        2. <SW_ROOT> - ..\TivaWare_C_Series-2.2.0.295
        3. <tivaware_install> - ..\TivaWare_C_Series-2.2.0.295 <img src=".\Assets\ccs2.jpg" alt="Alt Text" width="700">
    3. To enable SPI communication and touchscreen interfacing, the following  .c files must be linked. <img src=".\Assets\ccs3.jpg" alt="Alt Text" width="700">
    4. Add <SW_ROOT> as the build variable. <img src=".\Assets\ccs4.jpg" alt="Alt Text" width="700">
    5. Include appropriate search paths and directories. <img src=".\Assets\ccs5.jpg" alt="Alt Text" width="700">
    6. Include appropriate library search paths for the linker. <img src=".\Assets\ccs6.jpg" alt="Alt Text" width="700">

## Code Structure

The project is organized into directories and files with specific roles as outlined below:

<img src=".\Assets\codeTree.jpg" alt="Alt Text" width="600">

1. **<Initials.h>** - Contains initial setup functions for the tool.
2. **<Panels.h>** - Declarations related to different touchscreen panels.
3. **<Panel_6.h>** - Interface functions for the mathematical expression input panel.
4. **<Panel_6_2.h>** - Interface for the plotting panel.
5. **<calculator.c>** - The main entry point of the tool program.
6. **<OnButtonPress_6.c>** - Handles button presses in the expression input interface.
7. **<OnButtonPress_6_2.c>** - Manages the plotting interface functionality.
8. **<ButtonPress_Plot.c>** - Implements the features of zooming, shifting, differentiation, integration, and other plotting.
9. **<process_calculation.c>** - Processes and evaluates mathematical expressions.
10. **<evaluate.c>** - Converts expressions to pixel coordinate arrays for plotting.
11. **<startup_css>** - Startup file


## Authors

1. [ANANYA PAL](https://www.linkedin.com/in/ananya-pal-snuece2023)
2. [UJJWAL CHAUDHARY](https://www.linkedin.com/in/ujjwal-chaudhary-4436701aa/)
