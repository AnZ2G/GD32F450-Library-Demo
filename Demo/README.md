# GD32F450 Demo Collection

---

## Overview

This folder contains **15 demo projects** and one **main project** designed for the **GD32F450ZIT6** microcontroller.  
All demos are built and tested using **Keil uVision5 (Keil MDK v5)**.  
These examples demonstrate core peripheral functions, IO operations, and sensor interfaces for the GD32F450 development board.

Each demo includes full source code, interrupt handlers, and system clock configuration, and can be built directly after installing the GD32F450 add-on for Keil.

---

## Board Information

- **MCU:** GD32F450ZIT6  
- **IDE:** Keil 5 (uVision MDK v5)  
- **Dependencies:** GD32F450 Keil Add-on  
- **Programming Method:** USB ISP / ST-Link  
- **Libraries:**  
  - `Firmware/` — Official GigaDevice firmware library  
  - `Utilities/` — Board support and helper modules  
  - `Utilities/Third_Party/wh/` — Custom support library for demos  
  - `Utilities/Third_Party/GD32F450_Main_Board_Camera_V1.0/` — Library for the main project demo  

---

## Demo List

| No. | Folder | Name (EN) | Description |
|-----|---------|------------|-------------|
| 00 | Rolling_LED (board resource) | Rolling LED (board resource) | Demonstrates using onboard LED resources. |
| 01 | Rolling_LED (GPIO config) | Rolling LED (GPIO config) | Demonstrates how to configure GPIO for LED control. |
| 02 | Key_and_Dial_switch | Key and Dial Switch | Demonstrates button and DIP switch control of LEDs. |
| 03 | Key_EXTI | Key External Interrupt | Demonstrates using button external interrupts. |
| 04 | Wired_Communication_CH340C | Wired Communication (CH340C) | Demonstrates serial communication via CH340C. |
| 05 | Wireless_Communication | Wireless Communication | Demonstrates communication using a wireless serial module. |
| 06 | Reed_Switch | Reed Switch | Demonstrates magnetic field detection using a reed sensor. |
| 07 | ADC_Inductance | ADC Inductance | Demonstrates magnetic strength measurement using an inductor and ADC. |
| 08 | Encoder | Encoder | Demonstrates counting rotational pulses with an encoder. |
| 09 | Ultrasonic_Sensor | Ultrasonic Sensor | Demonstrates distance measurement using an ultrasonic sensor. |
| 10 | Steering_Engine_A3 | Servo A3 | Demonstrates controlling a servo motor via PWM output. |
| 11 | Motor_Drive | Motor Drive | Demonstrates PWM-based motor control. |
| 12 | OLED_Screen_Hardware_SPI | OLED (Hardware SPI) | Demonstrates using an OLED display over hardware SPI. |
| 13 | Gyroscope_Hardware_SPI | Gyroscope (Hardware SPI) | Demonstrates using a gyroscope sensor via SPI. |
| 14 | DCI_Camera | DCI Camera | Demonstrates image capture via DCI camera module. |
| 15 | Timer_Interrupt | Timer Interrupt | Demonstrates using timer interrupts for periodic tasks. |

---

## Main Project Demo

The main project demonstrates an **Autonomous ModelCar** running on the GD32F450 development board.  
It uses several peripherals (DC motors, encoders and camera module) integrated together for motion control.

---

## Project Structure

Demo/
├── 00_Rolling_LED(board resource)/
│ ├── main.c
│ ├── systick.c / systick.h
│ ├── gd32f4xx_it.c / gd32f4xx_it.h
│ ├── gd32f4xx_libopt.h
│ └── Keil_project/
│ └── Project.uvprojx
├── 01_Rolling_LED(GPIO config)/
│ └── ...
│
├── Libraries/
│ ├── Firmware/ # GigaDevice official firmware
│ └── Utilities/
│ ├── Board/
│ ├── Third_Party/
│ │ ├── wh/ # Custom helper libs
│ │ └── GD32F450_Main_Board_Camera_V1.0/
│
└── _Main_Project/

---

## Usage

1. **Install Environment**
   - Install **Keil MDK 5**  
   - Install **GD32F450 add-on package**  
   - Connect the development board via **ST-Link** or **USB ISP**

2. **Open and Build Demo**
   - Navigate to any demo folder (e.g. `Demo/01_Rolling_LED(GPIO config)/Keil_project/`)
   - Open the `Project.uvprojx` file in Keil
   - Build and download the firmware to the GD32F450 MCU

3. **Run**
   - Reset the board  
   - Observe the demo behavior (LED flashing, sensor response, etc.)

---

## Notes

- Each demo is **independent**, with its own `main.c` and peripheral configuration.
- The **main project demo** integrates multiple modules and libraries for a complete system demonstration.
- `Libraries` should remain at the same directory level as `Demo` for include paths to work correctly.

---

## License

This demo set is provided under the **MIT License** (see root directory for details).


