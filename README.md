# User Manual

## Introduction
This user manual provides detailed guidance on installing, using, and troubleshooting a multi-sensor embedded system developed for monitoring voltage, temperature, and brightness (light intensity) levels.

The system, which uses an AVR microcontroller, reads environmental data using three sensors and provides real-time feedback via an LCD, as well as visual and audible alerts via status-indicating LEDs and a buzzer. Sensors are user-selectable using three push buttons.

## System Overview
The embedded system is based on the **ATmega328P** microcontroller and is designed to read data from:
* A **Potentiometer** (acting as a voltage regulator)
* An **LM35** Temperature Sensor
* A light-sensitive **Photo Resistor (LDR)**

The system primarily uses external interrupt service routines (ISRs) to allow reliable sensor selection and timer-based alert handling for warnings and critical alerts.

## Requirements

### Hardware Requirements
To build and run the system, the following hardware components are required:
* ATmega328P Microcontroller (or Arduino Uno equivalent)
* LDR and resistor (for brightness sensing)
* Potentiometer (Voltage Regulation)
* LM35 Temperature Sensor
* 16x2 LCD display (with parallel interface)
* Active buzzer
* 3x Push buttons (for sensor selection)
* 3x LEDs (Green, Yellow, Red)
* Current-limiting resistors (220 Ohm for LEDs and LCD)
* Breadboard
* Jumper wires

### Software Requirements
* **IDE:** Microchip Studio IDE (or equivalent IDE)
* **AVR Libraries:**
  * AVR interrupt library (`<avr/interrupt.h>`)
  * AVR I/O library (`<avr/io.h>`)
  * Delay utilities (`<util/delay.h>`)
* **Custom Handling Libraries:**
  * LCD handling (`lcd.h`, `lcd.c`)
  * ADC handling (`ADC_HeaderFile.h`, `ADC.c`)

## Installation

### Step 1: Hardware Setup
1. Connect the **16x2 LCD** to `PORTB` of the ATmega328P.
2. Connect the **push buttons** to digital pins triggering `PCINT` and `INT0`/`INT1` (1, 2, 3).
3. Connect the **sensors** to analog pin channels (`A1`, `A2`, `A4`).
4. Connect the **Green, Yellow, and Red LEDs** to `PIND5`, `PIND6`, and `PIND7` respectively.
5. Connect the **active buzzer** to `PIND4`.
6. Connect a regulated **5V power source** to the microcontroller.

### Step 2: Software Upload
1. Ensure all required library files are included in the project directory.
2. Compile the project using **AVR-GCC**.
3. Upload the compiled program to the MCU.

## Usage
1. **Startup:** By default, the LCD will display a welcome message upon powering on.
2. **Sensor Selection:** Press one of the three buttons to select a sensor:
   * **Button 1** → Voltage
   * **Button 2** → Temperature
   * **Button 3** → Brightness
3. **Monitoring**: The LCD will display the real-time value of the selected sensor.
4. **Status Alerts**: Based on the sensor readings, the system indicates status via LEDs and the buzzer:
   * **Green LED (Solid)**: Normal range
   * **Yellow LED (0.5Hz blinking)**: Warning state
   * **Red LED (2Hz blinking)**: Critical state

> **Note:** The system continuously updates sensor values and responds to real-time user input via interrupts. Both warning and critical alerts will also trigger the active buzzer to sound at the corresponding frequencies (0.5Hz and 2Hz).
