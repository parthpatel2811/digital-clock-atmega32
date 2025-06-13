# 🕒 Digital Clock using ATmega32

## 📌 Overview
This project implements a real-time **digital clock** system using the **ATmega32** microcontroller. The system displays time and date on a 16x2 LCD and supports **manual adjustment** of hours, minutes, date, and month. An **LED-based mode toggle** allows the user to switch between **increment** and **decrement** mode for all values.

---

## 🛠️ Components Used

| Component             | Description                           |
|----------------------|---------------------------------------|
| 🖥️ Microcontroller    | ATmega32                              |
| 📟 Display            | 16x2 LCD                              |
| 🔘 Push Buttons       | For setting hours, minutes, date, month |
| 💡 LED                | Mode indicator (Increment/Decrement) |
| ⚡ Power Supply       | +5V regulated                         |
| 🔌 Programmer         | USBasp / Arduino ISP                  |
| 🔩 Other Components   | Resistors, Wires, Breadboard or PCB   |

---

## 🧠 Features

- Real-time clock display (HH:MM:SS)
- Real-time date display (DD/MM/YYYY)
- Push-button controlled time and date setting
- Mode switch using LED (ON = Increment, OFF = Decrement)
- Accurate 1-second tracking using Timer1 in CTC mode
- Leap year handling for February 29
- Modular and easy-to-modify C code (AVR-GCC)

---

## 🖼️ Project Diagram

![images/digital_clock.png](https://github.com/parthpatel2811/digital-clock-atmega32/blob/04834b7293ad9117a2de959f660ba914df1ef0c7/Schematic%20Diagram)

## ⚙️ Pin Configuration

| Component           | ATmega32 Pin    |
|---------------------|------------------|
| RS (LCD)            | PD0              |
| EN (LCD)            | PD1              |
| D4-D7 (LCD Data)    | PB4–PB7          |
| Push Button: Hours  | PC0              |
| Push Button: Minutes| PC1              |
| Push Button: Date   | PC2              |
| Push Button: Month  | PC3              |
| LED (Mode Toggle)   | PD7              |

---

## 🏗️ Working Principle

- **Timer1** is configured in **CTC mode** to generate an interrupt every 1 second.
- Time is incremented or decremented based on the state of a mode-select **LED**.
- Manual adjustments are done via push buttons.
- Date rolls over correctly across months, including leap years.

---

## 💡 How the Mode Works

- 🔴 **LED ON** → Increment Mode: Time and date values increase.
- ⚫ **LED OFF** → Decrement Mode: Time and date values decrease.
- A single push button toggles LED and hence the mode.

---

## 🖥️ Code

The project is written in embedded C using **AVR-GCC** and can be compiled via **Atmel Studio**, **PlatformIO**, or the **AVR-GCC toolchain**.

All code files are inside the `/code` folder.

## ▶️ How to Run
1.**🔌 Upload the Program**
Burn the compiled .hex file to the ATmega32 microcontroller using a programmer such as USBasp, AVR Dragon, or Arduino as ISP via tools like AVRDUDE, Atmel Studio, or ProgISP.

2.**⚡ Power the Circuit**
Connect the system to a 5V regulated power supply (from an adapter, USB, or a regulated battery pack).

3.**🔘 Use Buttons to Adjust Time/Date**
  - Use the four push buttons to increment/decrement hours, minutes, date, and month.
  - A separate toggle button switches the LED ON/OFF to choose between increment and decrement modes.

4.**👀 Observe the Clock**
  - The 16x2 LCD displays the current time and date in real-time.
  - Values change accurately with button inputs and timer ticks every 1 second.
