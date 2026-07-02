<div align="center">

# CHARGÉ — Embedded Systems Robot

**A line-following robot with symbol recognition, adaptive behaviors, and serial reporting, built on an ATmega324PA**

![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=white)
![Embedded](https://img.shields.io/badge/embedded-ATmega324PA-informational)
![Status](https://img.shields.io/badge/status-completed-brightgreen)

*Project completed for course INF1900 — Embedded Systems*

</div>

---

## Table of Contents

- [Project at a Glance](#project-at-a-glance)
- [Robot Behavior](#robot-behavior)
- [Symbol Responses](#symbol-responses)
- [End-of-Path Reporting](#end-of-path-reporting)
- [Hardware Used](#hardware-used)
- [Code Architecture](#code-architecture)
- [What I Learned](#what-i-learned)
- [Team](#team)

## Project at a Glance

CHARGÉ is an autonomous robot built around a finite state machine. It follows a line on a table, recognizes physical symbols placed along its path, reacts differently depending on the symbol encountered, and transmits a trip report over a serial link once it reaches the end. The robot supports three distinct paths, each with its own possible starting points.

This project required combining, on a single microcontroller, real-time control logic (motors, sensors, interrupts) with a communication and persistent memory layer — a solid look at what embedded systems development involves under real constraints.

## Robot Behavior

The robot starts on a yellow or red point, facing into the path. As it advances, it detects posts placed after each symbol using a distance sensor, at either 10 or 4 inches depending on the case. Each symbol triggers a specific behavior — stopping, going around, sounding an alert, or executing a rotation sequence — before the robot resumes its route.

## Symbol Responses

| Symbol | Behavior |
|---|---|
| Diode (△) | Stops at 10", green LED on, 2 s pause, then advances to 4" with a color change; LED turns off once the post is removed. |
| Motor (○) | Circles around the post without contact, then returns to the line before the next symbol. |
| Piezo (□) | Stops at 4", buzzer loops continuously, resumes with a melody once the post is removed. |
| Switch (_/) | Stops at 4", executes a rotation sequence, confirms the post has been removed before continuing. |
| End of path | Red/green LED blinks, then turns steady green after the interrupt button is pressed, triggering transmission of the final report. |

## End-of-Path Reporting

At the end of the run, the robot transmits a text representation of the detected path over USART to the console: the starting position, the location of every detected post, and the associated symbol type (M for motor, D for diode, P for piezo, I for switch).

## Hardware Used

- **Microcontroller:** ATmega324PA
- **Actuators:** DC motors, LEDs (red, green, amber), piezo buzzer
- **Sensors:** infrared sensors for line following, distance sensor (range up to 80 cm), switches for post detection
- **Memory:** external I²C EEPROM (24CXXX family)
- **Communication:** RS-232 serial link via USART

## Code Architecture

The code is organized into independent C++ classes, each responsible for one robot subsystem, which made testing and debugging across the team easier.

| Class | Responsibility |
|---|---|
| `DEL` | LED color and blinking control |
| `Interruptions` | External interrupt and Timer1 handling |
| `Timer` | Timer configuration |
| `USART` | RS-232 serial communication |
| `Memoire24CXXX` | External EEPROM read/write |
| `CAN` | Analog-to-digital conversion |
| `PWM` | Motor speed and LED intensity control |
| `RobotMotricite` | Robot movement (forward, backward, turn, stop) |
| `Debug` | Diagnostic and state-tracking tools |

The project builds through Makefiles linked against `librairie1900.a`, with an optional debug mode (`-DDEBUG -g`) and automatic cleanup of generated files.

## What I Learned

- Designing a finite state machine capable of handling several concurrent behaviors on a resource-constrained microcontroller
- Integrating sensors, actuators, external memory, and serial communication into one coherent system
- Working as a team on an embedded C++ codebase structured into reusable classes
- Debugging real-time behavior with limited tools (LEDs, serial output) instead of a conventional debugger

## Team

Project completed by **Ranya Anis**, **Noé Julien**, and **Rym Zidi**, for course INF1900.
