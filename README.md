# STM32F103C8 Digital Thermometer - Bare Metal Implementation

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![STM32](https://img.shields.io/badge/STM32-F103C8T6-blue)
![Keil](https://img.shields.io/badge/IDE-Keil--uVision5-darkgreen)

## Project Overview
This project implements a digital thermometer based on the **STM32F103C8T6** (Blue Pill) microcontroller, programmed entirely at the register level without HAL or standard peripheral libraries. Temperature data is acquired from an **LM35 analog temperature sensor** and displayed on a **4-digit 7-segment display**.

The display is controlled using two daisy-chained **74HC595 shift registers** over **SPI**, minimizing GPIO usage. The ADC operates in **single-conversion, software-triggered mode**, with conversion results handled via **end-of-conversion interrupts**. A timer provides non-blocking delays and display multiplexing timing.

This project focuses on **bare-metal embedded firmware design**, demonstrating complete control over the microcontroller's peripherals through direct register manipulation.

For a HAL/library-based version of this project, including detailed pin configuration and schematic:<br>
🔗 [STM32_Digital_Thermometer](https://github.com/rubin-khadka/STM32_Digital_Thermometer)

### Key Features
- 100% Register-Level Programming - No HAL, no external libraries
- Non-blocking Timer Architecture - Timer1 provides both delays and periodic interrupts
- Interrupt-Driven ADC - Temperature conversion with zero polling overhead
- SPI Communication - 4MHz data transfer to daisy-chained 74HC595 shift registers
- Memory Efficient - Minimal RAM/Flash footprint (see comparison below)
- Deterministic Timing - Precise control over every microsecond

### Hardware Components
- MCU: STM32F103C8T6 (Blue Pill) running at 8MHz (HSI)
- Temperature Sensor: LM35 (10 mV/°C)
- Display: 4-digit common-cathode 7-segment display
- Display Driver: 2 × 74HC595 shift registers (daisy-chained)
- Interface: SPI1 (data) + GPIO (latch control)

### Quick Pin Reference
| STM32 Pin | Function |
|-----------|----------|
| PA0 | LM35 Output |
| PA4 | 74HC595 Latch (ST_CP) |
| PA5 | SPI1 SCK |
| PA7 | SPI1 MOSI |

*For complete schematic and detailed connections, see the [original project](https://github.com/rubin-khadka/STM32_Digital_Thermometer)*

## System Configuration
The system is configured for deterministic timing and minimal resource usage.
- **System Clock**: 8 MHz (HSI internal oscillator)
    - No PLL, no external crystal required
    - Faster startup, lower power consumption

- **Timer1**:
    - 100µs per tick (PSC = 799 at 8MHz)
    - Dual-mode operation: one-shot delays + periodic interrupts
    - Display refresh every 5ms

- **SPI1**:
    - Baud Rate: 4 MHz (APB2/2)
    - Mode: Master, CPOL=0, CPHA=0 (first edge capture)
    - 16-bit transfers for daisy-chained shift registers

- **ADC1**:
    - Software-triggered, single conversion mode
    - Sampling Time: 28.5 cycles
    - End-of-conversion interrupt
    - Channel: PA0 (ADC1_IN0)

## Memory Footprint Comparison
| Memory Type | HAL Version | Bare Metal Version | Reduction |
|-------------|-------------|--------------------|-----------|
|Code | 6806 bytes | 3822 bytes | 44% smaller |
|RO-data	|326 bytes	|278 bytes	|15% smaller|
|RW-data	|12 bytes	|0 bytes	|100% smaller|
|ZI-data	|6396 bytes	|1648 bytes	|74% smaller|
|Total	|13.5 KB	|5.7 KB	|58% smaller|

This efficient implementation leaves **94% of Flash** and **92% of RAM** available for additional features.

## Video Demonstration

You can see the HAL-based version demo in the [original project](https://github.com/rubin-khadka/STM32_Digital_Thermometer#video-demonstrations).

## Quick Start
### Prerequisites
- STM32F103C8T6 (Blue Pill)
- Keil MDK-ARM 5 with ARM Compiler V6.18
- ST-Link programmer
- Proteus 8 (for simulation)

### Building the Project

1. Clone the repository
```bash
git clone https://github.com/rubin-khadka/STM32_Digital_Thermometer_BareMetal
```
2. Open in Keil uVision5
    - Double-click the project file (.uvprojx)
    - Or `Project` → `Open Project` → select file

3. Configure Project Settings
    - Ensure device is set to STM32F103C8
    - `Project` → `Options for Target` → `C/C++` tab
    - Add `STM32F10X_MD` to Preprocessor Symbols `Define`
    - Also add `Include Paths`:
        - ~\Keil\Packs\ARM\CMSIS\5.9.0\CMSIS\Core\Include

4. Build & Flash
    - Build: `F7`
    - Flash: `F8` (if using ST-Link)
    - Or `Debug` → `Start/Stop Debug Session`

## Resources
- [STM32F103 Datasheet](https://www.st.com/resource/en/datasheet/stm32f103c8.pdf)
- [74HC595 Shift Register Datasheet](https://www.ti.com/lit/ds/symlink/sn74hc595.pdf?spm=a2ty_o01.29997173.0.0.16bf5171zm4QVo&file=sn74hc595.pdf)
- [LM35 Temperature Sensor Datasheet](https://www.ti.com/lit/ds/symlink/lm35.pdf?spm=a2ty_o01.29997173.0.0.16bf5171zm4QVo&file=lm35.pdf)

## What This Project Demonstrates
- Complete register-level STM32 programming without HAL dependencies
- Interrupt-driven architecture for real-time responsiveness
- Efficient resource utilization (58% smaller than HAL equivalent)
- SPI communication with daisy-chained shift registers

## Project Status
- Status: Complete
- Version: v1.0
- Last Updated: February 2026

## Contact
Rubin Khadka Chhetri<br>
📧 rubin.robotic@gmail.com<br>
🐙 GitHub: https://github.com/rubin-khadka
