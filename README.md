# GD32F450ZIT6 Demo and PCB Project

This repository contains demo projects, libraries, and PCB design files based on the **GD32F450ZIT6** microcontroller.  
It demonstrates the use of **GPIOs, peripherals, communication modules, sensors, and camera systems**, as well as the **main control system for an autonomous model car**.

---

## Directory Structure

```
GD32F450_Project/
│
├── Demo/                         # Functional demo programs (15 demos + main project)
│
├── PCB/                          # Mainboard and driver board PCB projects (Altium Designer)
│
├── Docs/                         # Documentation and demonstration files
│   ├── GD32F450_User_Manual.pdf  # Official user manual
│   ├── main_car_demo.gif          # Autonomous model car demo
│   └── camera_display.gif         # Camera + OLED binary display demo
│
├── .gitignore
├── LICENSE
└── README.md
```

---

## Demo Overview

**MCU:** GD32F450ZIT6  
**IDE:** Keil uVision 5  
**Libraries:** Official Firmware + custom secondary encapsulation (Third_Party/wh)

### Demo List

| No. | Folder | Demo Name | Description |
|:----:|:-----------------------------|:----------------------------|:----------------------------------------------|
| 00 | Rolling_LED (board resorse) | Rolling LED (board resource) | Demonstrates how to use onboard LEDs |
| 01 | Rolling_LED (GPIO config) | Rolling LED (GPIO config) | Demonstrates GPIO configuration for LED control |
| 02 | Key_and_Dial_switch | Key and Dial Switch | Demonstrates button and DIP switch control |
| 03 | Key_EXTI | Key External Interrupt | Demonstrates external interrupt handling |
| 04 | Wired_Communication_CH340C | Wired Communication (CH340C) | Uses CH340C for serial communication |
| 05 | Wireless_Communication | Wireless Communication | Uses wireless UART module for communication |
| 06 | Reed_Switch | Reed Switch | Demonstrates magnetic field detection |
| 07 | ADC_Inductance | ADC Inductance | Measures magnetic field strength via inductance |
| 08 | Encoder | Encoder | Measures rotational counts and direction |
| 09 | Ultrasonic_Sensor | Ultrasonic Sensor | Demonstrates distance measurement |
| 10 | Steering_Engine_A3 | Servo A3 | PWM control for servo motor |
| 11 | Motor_Drive | Motor Drive | PWM control for DC motor speed and direction |
| 12 | OLED_Screen_Hardware_SPI | OLED (Hardware SPI) | Drives OLED display via SPI |
| 13 | Gyroscope_Hardware_SPI | Gyroscope (Hardware SPI) | Reads gyroscope data through SPI |
| 14 | DCI_Camera | DCI Camera | Demonstrates camera image capture |
| 15 | Timer_Interrupt | Timer Interrupt | Demonstrates timer interrupt usage |

---

## Main Project Demo

**Demonstration Purpose:**  
The main project demonstrates the autonomous driving behavior of a model car based on this board.  
It also includes camera image acquisition and real-time binary visualization on the onboard OLED display.

- Autonomous Model Car Demo:  
  ![main_car_demo.gif](Docs/main_car_demo.gif)

- Camera Real-Time Display Demo:  
  ![camera_display.gif](Docs/camera_display.gif)

**Related Libraries:**
```
Libraries/
├── Firmware/                          # Official GD32 firmware library
├── Utilities/                         # Board support and third-party utilities
│   ├── Third_Party/
│   │   ├── wh/                        # Common encapsulation library for demos
│   │   └── GD32F450_Main_Board_Camera_V1.0/  # Library for main project demo
```

---

## Usage Instructions

1. Install **Keil uVision 5** and the **GD32F450 add-on package**.  
2. Open any demo project, for example:  
   ```
   Demo\01_Rolling_LED\Keil_project\Project.uvprojx
   ```
3. Build the project in Keil.  
4. Program the generated `.hex` or `.bin` file into the MCU.  
5. Power on the board and observe the demo behavior.

**File Structure of Each Demo:**
```
gd32f4xx_it.c/.h      # Interrupt handlers
main.c                # Main application
systick.c/.h          # System clock
gd32f4xx_libopt.h     # Hardware library configuration
```

---

## License

This project is developed for educational and research purposes based on the **GigaDevice GD32F450** platform.  
It includes portions of the official firmware library.  
Please refer to the [LICENSE](LICENSE) file for detailed terms.
