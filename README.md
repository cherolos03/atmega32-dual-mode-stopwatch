##🕒 Stopwatch with Dual Mode (Increment & Countdown) using ATmega32

##📌 Project Overview

This project implements a digital stopwatch system using an ATmega32 microcontroller.
The stopwatch supports two operating modes:
 • ✅ Increment Mode (Stopwatch)
 • ⏳ Countdown Mode (Timer)

The system displays hours, minutes, and seconds using six multiplexed seven-segment displays controlled by a single 7447 BCD decoder.

⸻

##⚙️ Features
 • ⏱️ Real-time counting using Timer1 (CTC Mode)
 • 🔁 Dual operating modes (Count Up / Count Down)
 • ⏸️ Pause and Resume using external interrupts
 • 🔄 Reset using external interrupt
 • 🔔 Buzzer alarm when countdown reaches zero
 • 💡 LED indicators for mode status
 • 🎛️ Time adjustment buttons (hours / minutes / seconds)
 • 🖥️ Proteus simulation supported

⸻

##🧰 Hardware Components
 • ATmega32 Microcontroller (16 MHz)
 • 6 × Seven Segment Displays (Common Anode)
 • 7447 BCD Decoder
 • NPN Transistors for multiplexing
 • Push buttons (Reset, Pause, Resume, Mode, Adjustments)
 • Buzzer
 • LEDs
 • Proteus Simulation Environment

⸻

##🧠 Concepts Used
 • Embedded C Programming
 • Timers & Interrupts
 • External Interrupts (INT0, INT1, INT2)
 • Multiplexed Display Driving
 • Debouncing
 • Bitwise Register Configuration
 • Hardware Interfacing
 • State Machine Logic
