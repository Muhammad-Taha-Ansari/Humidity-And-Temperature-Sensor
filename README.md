<div align="center">

<!-- Animated Banner -->
<img src="https://capsule-render.vercel.app/api?type=waving&color=0:0d1117,50:00d4ff,100:0d1117&height=200&section=header&text=Humidity%20%26%20Temperature%20Sensor&fontSize=36&fontColor=00d4ff&fontAlignY=38&desc=Smart%20Environmental%20Monitoring%20System%20%7C%20ATmega328P%20%2B%20Embedded%20C&descSize=14&descAlignY=58&descColor=7fdbff&animation=fadeIn" width="100%"/>

<!-- Animated Typing SVG -->
<a href="https://github.com/Muhammad-Taha-Ansari/Humidity-And-Temperature-Sensor">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=22&duration=3000&pause=1000&color=00D4FF&center=true&vCenter=true&multiline=true&repeat=true&width=700&height=100&lines=⚡+Real-Time+Environmental+Monitoring;🌡️+AHT20+%2B+SSD1306+OLED+%2B+ATmega328P;🔁+FSM-Based+Alert+System+%7C+PWM+Fan+Control;🎨+Graphical+UI+%7C+Graph+Plotting+%7C+Splash+Screen" alt="Typing SVG" />
</a>

<br/>

<!-- Badges Row 1 -->
<p>
  <img src="https://img.shields.io/badge/Language-Embedded%20C-00d4ff?style=for-the-badge&logo=c&logoColor=white&labelColor=0d1117"/>
  <img src="https://img.shields.io/badge/MCU-ATmega328P-ff6b35?style=for-the-badge&logo=arduino&logoColor=white&labelColor=0d1117"/>
  <img src="https://img.shields.io/badge/Toolchain-AVR--GCC-a855f7?style=for-the-badge&logo=gnu&logoColor=white&labelColor=0d1117"/>
  <img src="https://img.shields.io/badge/Simulation-Proteus-00ff88?style=for-the-badge&logo=data:image/svg+xml;base64,&logoColor=white&labelColor=0d1117"/>
</p>

<!-- Badges Row 2 -->
<p>
  <img src="https://img.shields.io/badge/Status-Active%20Development-brightgreen?style=for-the-badge&logo=statuspage&logoColor=white&labelColor=0d1117"/>
  <img src="https://img.shields.io/badge/Protocol-I2C-f59e0b?style=for-the-badge&logo=protocols.io&logoColor=white&labelColor=0d1117"/>
  <img src="https://img.shields.io/badge/Display-SSD1306%20OLED-e879f9?style=for-the-badge&logo=adafruit&logoColor=white&labelColor=0d1117"/>
  <img src="https://img.shields.io/badge/Sensor-AHT20-22d3ee?style=for-the-badge&logo=data:image/svg+xml;base64,&logoColor=white&labelColor=0d1117"/>
</p>

<!-- Badges Row 3 -->
<p>
  <img src="https://img.shields.io/github/stars/Muhammad-Taha-Ansari/Humidity-And-Temperature-Sensor?style=for-the-badge&logo=github&color=ffd700&labelColor=0d1117"/>
  <img src="https://img.shields.io/github/forks/Muhammad-Taha-Ansari/Humidity-And-Temperature-Sensor?style=for-the-badge&logo=github&color=00d4ff&labelColor=0d1117"/>
  <img src="https://img.shields.io/github/issues/Muhammad-Taha-Ansari/Humidity-And-Temperature-Sensor?style=for-the-badge&logo=github&color=ff4444&labelColor=0d1117"/>
  <img src="https://img.shields.io/github/license/Muhammad-Taha-Ansari/Humidity-And-Temperature-Sensor?style=for-the-badge&logo=github&color=a855f7&labelColor=0d1117"/>
</p>

</div>

---

<!-- Divider -->
<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## <img src="https://media2.giphy.com/media/QssGEmpkyEOhBCb7e1/giphy.gif?cid=ecf05e47a0n3gi1bfqntqmob8g9aid1oyj2wr3ds3mg700bl&rid=giphy.gif" width="28"> Table of Contents

<div align="center">

| # | Section |
|---|---------|
| 01 | [⚡ Project Overview](#-project-overview) |
| 02 | [🎯 Key Features](#-key-features) |
| 03 | [🛠️ Tech Stack](#%EF%B8%8F-tech-stack) |
| 04 | [🏗️ System Architecture](#%EF%B8%8F-system-architecture) |
| 05 | [🔁 FSM Diagram](#-fsm-diagram) |
| 06 | [📺 OLED Screen Descriptions](#-oled-screen-descriptions) |
| 07 | [⚙️ Pin Configuration](#%EF%B8%8F-pin-configuration) |
| 08 | [🗂️ Project Structure](#%EF%B8%8F-project-structure) |
| 09 | [🔄 System Workflow](#-system-workflow) |
| 10 | [📊 Project Status](#-project-status) |
| 11 | [🚀 Future Improvements](#-future-improvements) |
| 12 | [👨‍💻 Team Members](#-team-members) |

</div>

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## ⚡ Project Overview

<div align="center">
<img src="https://capsule-render.vercel.app/api?type=rect&color=0:0d1117,100:001a2e&height=3&section=header" width="100%"/>
</div>

> **A production-grade embedded system** for real-time environmental intelligence — engineered from silicon to UI.

The **Humidity And Temperature Sensor System** is a fully featured embedded application built on the **ATmega328P** microcontroller. Developed in pure **Embedded C** using **AVR-GCC**, it delivers real-time acquisition of environmental data via the **AHT20** sensor over **I2C**, rendered through a rich graphical interface on a **128×64 SSD1306 OLED** display.

The firmware implements a robust **Finite State Machine (FSM)** for intelligent alert management, a **cooperative task scheduler** for deterministic execution, **PWM-based variable fan control**, multi-indicator alerting (LEDs + buzzer), **live graph plotting**, animated splash screens, and multi-screen button navigation — all implemented without an RTOS, using optimized fixed-point arithmetic.

```
╔══════════════════════════════════════════════════════════════════╗
║  ENVIRONMENT → AHT20 → I2C Bus → ATmega328P → FSM Engine       ║
║                                        ↓                         ║
║              OLED UI ← Graph Engine ← Scheduler ← Sensor Data  ║
║                                        ↓                         ║
║         PWM Fan ← Alert System → LEDs + Buzzer + Display        ║
╚══════════════════════════════════════════════════════════════════╝
```

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## 🎯 Key Features

<div align="center">

```
┌─────────────────────────────────────────────────────────────────┐
│                    ★  FEATURE MATRIX  ★                         │
└─────────────────────────────────────────────────────────────────┘
```

</div>

<table align="center">
<tr>
<td width="50%">

### 🌡️ Sensing & Data
- ✅ Real-time temperature acquisition (AHT20)
- ✅ Real-time humidity acquisition (AHT20)
- ✅ I2C communication protocol (400 kHz)
- ✅ Fixed-point arithmetic (no FPU required)
- ✅ Calibrated sensor readings

</td>
<td width="50%">

### 🖥️ Display & UI
- ✅ 128×64 SSD1306 OLED rendering
- ✅ Animated splash screen on boot
- ✅ Live graph plotting (temp & humidity)
- ✅ Multi-screen navigation via buttons
- ✅ Custom graphical UI layout

</td>
</tr>
<tr>
<td width="50%">

### 🔁 Control & Logic
- ✅ FSM-based 4-state alert engine
- ✅ Cooperative embedded task scheduler
- ✅ PWM variable-speed fan control (D9)
- ✅ Dynamic duty cycle based on temperature
- ✅ Button debounce logic

</td>
<td width="50%">

### 🚨 Alerting & Indicators
- ✅ Green LED — Normal state
- ✅ Yellow LED — Warning state
- ✅ Red LED — Critical state
- ✅ Active buzzer (patterned alerts)
- ✅ Multi-level alert thresholds

</td>
</tr>
</table>

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## 🛠️ Tech Stack

<div align="center">

<img src="https://skillicons.dev/icons?i=c,git,github,vscode,linux&theme=dark&perline=5" />

<br/><br/>

| Component | Technology | Role |
|-----------|-----------|------|
| <img src="https://img.shields.io/badge/Language-Embedded%20C-00d4ff?style=flat-square&logo=c&logoColor=white"/> | **Embedded C (C99)** | Core firmware logic |
| <img src="https://img.shields.io/badge/MCU-ATmega328P-ff6b35?style=flat-square&logo=arduino&logoColor=white"/> | **ATmega328P** | 8-bit AVR microcontroller |
| <img src="https://img.shields.io/badge/Toolchain-AVR--GCC-a855f7?style=flat-square&logo=gnu&logoColor=white"/> | **AVR-GCC + avr-libc** | Compilation & linking |
| <img src="https://img.shields.io/badge/Sensor-AHT20-22d3ee?style=flat-square"/> | **AHT20** | Temp & humidity sensing |
| <img src="https://img.shields.io/badge/Display-SSD1306-e879f9?style=flat-square"/> | **SSD1306 OLED** | 128×64 graphical display |
| <img src="https://img.shields.io/badge/Protocol-I2C-f59e0b?style=flat-square"/> | **I2C (TWI)** | Sensor + display bus |
| <img src="https://img.shields.io/badge/PWM-Timer1-00ff88?style=flat-square"/> | **AVR Timer/Counter1** | PWM fan speed control |
| <img src="https://img.shields.io/badge/Simulation-Proteus-0ea5e9?style=flat-square"/> | **Proteus 8** | Circuit simulation |
| <img src="https://img.shields.io/badge/Programmer-AVRDUDE-ffd700?style=flat-square"/> | **AVRDUDE** | Flash programming |

</div>

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## 🏗️ System Architecture

<div align="center">

```
╔══════════════════════════════════════════════════════════════════════════╗
║                    SYSTEM ARCHITECTURE OVERVIEW                          ║
╠══════════════════════════════════════════════════════════════════════════╣
║                                                                          ║
║   ┌─────────────┐     I2C (A4/A5)    ┌──────────────────────────────┐  ║
║   │   AHT20     │ ══════════════════► │        ATmega328P            │  ║
║   │  Sensor     │                     │  ┌────────────────────────┐  │  ║
║   │ Temp + Hum  │                     │  │   Cooperative          │  │  ║
║   └─────────────┘                     │  │   Task Scheduler       │  │  ║
║                                       │  │  ┌──────┐ ┌────────┐  │  │  ║
║   ┌─────────────┐     I2C (A4/A5)    │  │  │ Task │ │  Task  │  │  │  ║
║   │  SSD1306    │ ◄══════════════════  │  │  │  A  │ │   B    │  │  │  ║
║   │   OLED      │                     │  │  └──────┘ └────────┘  │  │  ║
║   │  128 × 64   │                     │  └────────────────────────┘  │  ║
║   └─────────────┘                     │                               │  ║
║                                       │  ┌────────────────────────┐  │  ║
║   ┌─────────────┐     D9 (PWM)        │  │    FSM Alert Engine    │  │  ║
║   │  DC Fan     │ ◄═══════════════════  │  │  NORMAL → WARN →      │  │  ║
║   │ Variable    │                     │  │  CRITICAL → ALARM      │  │  ║
║   │   Speed     │                     │  └────────────────────────┘  │  ║
║   └─────────────┘                     │                               │  ║
║                                       │  ┌────────────────────────┐  │  ║
║   ┌─────────────┐     D4/D5/D6        │  │   Fixed-Point Math     │  │  ║
║   │    LEDs     │ ◄═══════════════════  │  │   No FPU Required      │  │  ║
║   │ G / Y / R   │                     │  └────────────────────────┘  │  ║
║   └─────────────┘                     └──────────────────────────────┘  ║
║                                                       ▲                  ║
║   ┌─────────────┐     D8              ┌──────────────┴───────────────┐  ║
║   │   Buzzer    │ ◄═══════════════════  │        Button Matrix         │  ║
║   │   Active    │                     │  D2 (Right) D3 (Left) D7 (Main)│ ║
║   └─────────────┘                     └──────────────────────────────┘  ║
║                                                                          ║
╚══════════════════════════════════════════════════════════════════════════╝
```

</div>

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## 🔁 FSM Diagram

<div align="center">

```
╔══════════════════════════════════════════════════════════════════╗
║              FINITE STATE MACHINE — ALERT ENGINE                 ║
╠══════════════════════════════════════════════════════════════════╣
║                                                                  ║
║                     ┌──────────────┐                            ║
║            ┌───────►│   NORMAL     │◄──────────────┐           ║
║            │        │  🟢 Green    │               │           ║
║            │        │  Fan: LOW    │               │           ║
║            │        │  Buzz: OFF   │               │           ║
║            │        └──────┬───────┘               │           ║
║            │               │                       │           ║
║            │         Temp > 30°C                   │           ║
║            │         OR Hum > 70%                  │           ║
║            │               │                       │           ║
║            │        ┌──────▼───────┐         Temp < 28°C      ║
║            │        │   WARNING    │         AND Hum < 68%     ║
║            │        │  🟡 Yellow   │               │           ║
║     Temp < │        │  Fan: MED    │               │           ║
║     25°C   │        │  Buzz: SLOW  │◄──────────────┘           ║
║            │        └──────┬───────┘                           ║
║            │               │                                    ║
║            │         Temp > 38°C                               ║
║            │         OR Hum > 85%                              ║
║            │               │                                    ║
║            │        ┌──────▼───────┐                           ║
║            │        │   CRITICAL   │                           ║
║            │        │  🔴 Red      │                           ║
║            │        │  Fan: HIGH   │                           ║
║            └────────│  Buzz: FAST  │                           ║
║                     └──────┬───────┘                           ║
║                            │                                    ║
║                      Temp > 45°C                               ║
║                      OR Hum > 95%                              ║
║                            │                                    ║
║                     ┌──────▼───────┐                           ║
║                     │    ALARM     │                           ║
║                     │  🔴 Red Blink│                           ║
║                     │  Fan: FULL   │                           ║
║                     │  Buzz: CONT. │                           ║
║                     └──────────────┘                           ║
║                                                                  ║
║   State Transitions are threshold-hysteresis guarded            ║
╚══════════════════════════════════════════════════════════════════╝
```

| State | Temp Threshold | Humidity Threshold | Fan PWM | LED | Buzzer |
|-------|---------------|-------------------|---------|-----|--------|
| 🟢 **NORMAL** | < 30°C | < 70% | 20% | Green | OFF |
| 🟡 **WARNING** | 30–38°C | 70–85% | 50% | Yellow | Slow Beep |
| 🔴 **CRITICAL** | 38–45°C | 85–95% | 80% | Red | Fast Beep |
| 🚨 **ALARM** | > 45°C | > 95% | 100% | Red Blink | Continuous |

</div>

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## 📺 OLED Screen Descriptions

<div align="center">

```
╔═══════════════════════════════════════════════════════════════╗
║                   SSD1306 OLED SCREEN TOUR                    ║
╚═══════════════════════════════════════════════════════════════╝
```

</div>

### 🎬 Screen 1 — Animated Splash Screen

```
┌────────────────────────────────┐
│  ╔══════════════════════════╗  │
│  ║   ~~~ ENV MONITOR ~~~   ║  │  ← Title animation (fade in)
│  ╚══════════════════════════╝  │
│                                │
│         [ ████████░░ ]         │  ← Loading progress bar
│                                │
│    Team Embedded Systems 🔥    │  ← Credits scroll
└────────────────────────────────┘
Displayed for ~3 seconds on boot
```

### 🌡️ Screen 2 — Live Data Dashboard

```
┌────────────────────────────────┐
│ TEMP:  ██████ 32.5 °C  [WARN] │  ← Temperature with bar
│ ──────────────────────────── │
│ HUM:   █████░ 67.3 %  [  OK] │  ← Humidity with bar
│ ──────────────────────────── │
│ FAN:   ████░░░░ 50%  PWM     │  ← Fan duty cycle
│ ──────────────────────────── │
│ STATE: ⚠  WARNING MODE       │  ← FSM state indicator
└────────────────────────────────┘
```

### 📈 Screen 3 — Live Graph Plotter

```
┌────────────────────────────────┐
│ TEMP GRAPH (last 64 samples)   │
│ 50°╤                           │
│    │       ╭──╮                │
│    │  ╭────╯  ╰───╮           │
│    │╭─╯           ╰──         │
│  0°╧────────────────────────── │
│    0                       64  │
└────────────────────────────────┘
```

### ⚙️ Screen 4 — System Info & Stats

```
┌────────────────────────────────┐
│ ┌──────────────────────────┐  │
│ │   SYSTEM INFORMATION     │  │
│ └──────────────────────────┘  │
│  MCU  : ATmega328P @ 16MHz    │
│  PROTO: I2C (SDA:A4 SCL:A5)  │
│  UPTIME: 00h 14m 32s          │
│  SAMPLES: 3621                │
└────────────────────────────────┘
```

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## ⚙️ Pin Configuration

<div align="center">

```
╔══════════════════════════════════════════════════════════════════╗
║                   ATmega328P PIN MAP                             ║
╠═══════════════════════╦══════════════════════════════════════════╣
║      PIN              ║         FUNCTION                         ║
╠═══════════════════════╬══════════════════════════════════════════╣
║  A4  (PC4) — SDA      ║  I2C Data  → AHT20 + SSD1306           ║
║  A5  (PC5) — SCL      ║  I2C Clock → AHT20 + SSD1306           ║
╠═══════════════════════╬══════════════════════════════════════════╣
║  D9  (PB1) — OC1A     ║  PWM Output → DC Fan Speed Control     ║
║  D8  (PB0)            ║  Digital Out → Active Buzzer            ║
╠═══════════════════════╬══════════════════════════════════════════╣
║  D4  (PD4)            ║  Digital Out → Green LED (Normal)       ║
║  D5  (PD5)            ║  Digital Out → Yellow LED (Warning)     ║
║  D6  (PD6)            ║  Digital Out → Red LED (Critical)       ║
╠═══════════════════════╬══════════════════════════════════════════╣
║  D2  (PD2) — INT0     ║  Digital In  → Right Button            ║
║  D3  (PD3) — INT1     ║  Digital In  → Left Button             ║
║  D7  (PD7)            ║  Digital In  → Main/Select Button      ║
╚═══════════════════════╩══════════════════════════════════════════╝
```

</div>

| Pin | Port | Direction | Connected To | Function |
|-----|------|-----------|-------------|----------|
| `A4` | PC4 | Bidirectional | AHT20 + SSD1306 | I2C SDA |
| `A5` | PC5 | Bidirectional | AHT20 + SSD1306 | I2C SCL |
| `D9` | PB1 | Output (OC1A) | DC Fan (via transistor) | PWM Speed Control |
| `D8` | PB0 | Output | Active Buzzer | Alert Tones |
| `D4` | PD4 | Output | Green LED | Normal State |
| `D5` | PD5 | Output | Yellow LED | Warning State |
| `D6` | PD6 | Output | Red LED | Critical/Alarm State |
| `D2` | PD2 | Input (INT0) | Right Button | Screen Navigation → |
| `D3` | PD3 | Input (INT1) | Left Button | Screen Navigation ← |
| `D7` | PD7 | Input | Main Button | Select / Confirm |

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## 🗂️ Project Structure

```
Humidity-And-Temperature-Sensor/
│
├── 📁 src/                          # Source files
│   ├── 📄 main.c                    # Entry point, scheduler loop
│   ├── 📄 aht20.c / aht20.h        # AHT20 sensor driver
│   ├── 📄 ssd1306.c / ssd1306.h    # OLED display driver
│   ├── 📄 i2c.c / i2c.h            # TWI/I2C hardware abstraction
│   ├── 📄 pwm.c / pwm.h            # Timer1 PWM fan control
│   ├── 📄 fsm.c / fsm.h            # Finite State Machine engine
│   ├── 📄 scheduler.c / scheduler.h # Cooperative task scheduler
│   ├── 📄 buttons.c / buttons.h    # Button debounce & navigation
│   ├── 📄 leds.c / leds.h          # LED indicator control
│   ├── 📄 buzzer.c / buzzer.h      # Buzzer pattern generator
│   ├── 📄 oled_ui.c / oled_ui.h    # Screen layouts and UI logic
│   ├── 📄 graph.c / graph.h        # Graph plotting engine
│   ├── 📄 splash.c / splash.h      # Animated splash screen
│   └── 📄 fixed_point.h            # Fixed-point arithmetic macros
│
├── 📁 include/                      # Shared headers
│   └── 📄 config.h                  # Global thresholds & pin defs
│
├── 📁 simulation/                   # Proteus simulation files
│   ├── 📄 circuit.pdsprj            # Proteus project file
│   └── 📄 schematic.png             # Exported circuit schematic
│
├── 📁 docs/                         # Documentation assets
│   ├── 📄 architecture.md           # System architecture notes
│   ├── 📄 fsm_diagram.png           # FSM visual diagram
│   └── 📄 oled_screens.png          # OLED screen captures
│
├── 📄 Makefile                      # Build system (AVR-GCC)
├── 📄 .gitignore                    # Git ignore rules
├── 📄 LICENSE                       # MIT License
└── 📄 README.md                     # This file
```

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## 🔄 System Workflow

<div align="center">

```
╔══════════════════════════════════════════════════════════════╗
║                   FIRMWARE EXECUTION FLOW                    ║
╠══════════════════════════════════════════════════════════════╣
║                                                              ║
║   POWER ON                                                   ║
║       │                                                      ║
║       ▼                                                      ║
║   ┌─────────────────────────────────┐                       ║
║   │   Hardware Initialization       │                       ║
║   │   GPIO | Timer1 | TWI | UART   │                       ║
║   └─────────────┬───────────────────┘                       ║
║                 │                                            ║
║                 ▼                                            ║
║   ┌─────────────────────────────────┐                       ║
║   │   Animated Splash Screen        │  ~3 sec               ║
║   │   Logo + Progress Bar           │                       ║
║   └─────────────┬───────────────────┘                       ║
║                 │                                            ║
║                 ▼                                            ║
║   ┌─────────────────────────────────┐                       ║
║   │   Scheduler Main Loop           │ ◄─────────────────┐  ║
║   │   ┌────────────────────────┐   │                    │  ║
║   │   │  Task A: Read AHT20   │   │  Every 500ms       │  ║
║   │   │  (Temp + Humidity)    │   │                    │  ║
║   │   └───────────┬────────────┘   │                    │  ║
║   │               │                │                    │  ║
║   │   ┌───────────▼────────────┐   │                    │  ║
║   │   │  Task B: FSM Update   │   │  Every 500ms       │  ║
║   │   │  (State Transitions)  │   │                    │  ║
║   │   └───────────┬────────────┘   │                    │  ║
║   │               │                │                    │  ║
║   │   ┌───────────▼────────────┐   │                    │  ║
║   │   │  Task C: PWM Adjust   │   │  On state change   │  ║
║   │   │  (Fan Duty Cycle)     │   │                    │  ║
║   │   └───────────┬────────────┘   │                    │  ║
║   │               │                │                    │  ║
║   │   ┌───────────▼────────────┐   │                    │  ║
║   │   │  Task D: OLED Render  │   │  Every 100ms       │  ║
║   │   │  (Screen + Graph)     │   │                    │  ║
║   │   └───────────┬────────────┘   │                    │  ║
║   │               │                │                    │  ║
║   │   ┌───────────▼────────────┐   │                    │  ║
║   │   │  Task E: Buttons Poll │   │  Every 20ms        │  ║
║   │   │  (Debounce + Navigate)│   │                    │  ║
║   │   └────────────────────────┘   │                    │  ║
║   └─────────────────────────────────┘                    │  ║
║                 └────────────────────────────────────────┘  ║
╚══════════════════════════════════════════════════════════════╝
```

</div>

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## 📊 Project Status

<div align="center">

```
╔════════════════════════════════════════════════════════════╗
║                   PROJECT STATUS BOARD                      ║
╠══════════════════════════╦═════════════════════════════════╣
║   Module                 ║   Status                        ║
╠══════════════════════════╬═════════════════════════════════╣
║  AHT20 Sensor Driver     ║  ██████████ 100%  ✅ COMPLETE  ║
║  SSD1306 OLED Driver     ║  ██████████ 100%  ✅ COMPLETE  ║
║  I2C / TWI Layer         ║  ██████████ 100%  ✅ COMPLETE  ║
║  FSM Alert Engine        ║  ██████████ 100%  ✅ COMPLETE  ║
║  PWM Fan Control         ║  ██████████ 100%  ✅ COMPLETE  ║
║  Cooperative Scheduler   ║  ██████████ 100%  ✅ COMPLETE  ║
║  LED Indicators          ║  ██████████ 100%  ✅ COMPLETE  ║
║  Buzzer Patterns         ║  ██████████ 100%  ✅ COMPLETE  ║
║  OLED UI (4 Screens)     ║  ██████████ 100%  ✅ COMPLETE  ║
║  Live Graph Plotter      ║  ██████████ 100%  ✅ COMPLETE  ║
║  Animated Splash Screen  ║  ██████████ 100%  ✅ COMPLETE  ║
║  Button Navigation       ║  ██████████ 100%  ✅ COMPLETE  ║
║  Fixed-Point Math        ║  ██████████ 100%  ✅ COMPLETE  ║
║  Proteus Simulation      ║  ████████░░  80%  🔄 ONGOING  ║
║  Hardware Testing        ║  ██████░░░░  60%  🔄 ONGOING  ║
╚══════════════════════════╩═════════════════════════════════╝
```

<br/>

<img src="https://img.shields.io/badge/Overall%20Progress-93%25-00d4ff?style=for-the-badge&logo=checkmarx&logoColor=white&labelColor=0d1117"/>
<img src="https://img.shields.io/badge/Build-Passing-brightgreen?style=for-the-badge&logo=github-actions&logoColor=white&labelColor=0d1117"/>
<img src="https://img.shields.io/badge/Simulation-Verified-a855f7?style=for-the-badge&logo=proteus&logoColor=white&labelColor=0d1117"/>

</div>

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## 🚀 Future Improvements

<div align="center">

| Priority | Feature | Description |
|----------|---------|-------------|
| 🔴 High | **UART Logging** | Serial data logging for host PC analysis |
| 🔴 High | **SD Card Datalogging** | Store sensor history with timestamps |
| 🟡 Medium | **Wi-Fi Integration** | ESP8266/ESP-01 for wireless IoT dashboard |
| 🟡 Medium | **RTC Module** | DS3231 for timestamped readings |
| 🟡 Medium | **EEPROM Config Save** | Persist thresholds across power cycles |
| 🟢 Low | **Touchscreen UI** | Replace buttons with capacitive touch |
| 🟢 Low | **Mobile Dashboard** | MQTT + Home Assistant integration |
| 🟢 Low | **Multi-Sensor Support** | Add CO₂, pressure, UV index sensors |
| 🟢 Low | **PCB Design** | Custom PCB layout in KiCad |

</div>

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

## 👨‍💻 Team Members

<div align="center">

```
╔════════════════════════════════════════════════════════════════╗
║                    ENGINEERING TEAM                            ║
╠═══════════════════════╦════════════════════════════════════════╣
║   Name                ║   Role                                 ║
╠═══════════════════════╬════════════════════════════════════════╣
║  Muhammad Taha Ansari ║  🏆 Group Leader — Architecture &     ║
║                       ║     Firmware Lead                      ║
╠═══════════════════════╬════════════════════════════════════════╣
║  Ali Nasir            ║  ⚙️  Hardware Design & I2C Driver     ║
╠═══════════════════════╬════════════════════════════════════════╣
║  Farzam Shaikh        ║  🖥️  OLED UI & Graph Engine           ║
╠═══════════════════════╬════════════════════════════════════════╣
║  Ahmed                ║  🔁  FSM Design & Testing             ║
╚═══════════════════════╩════════════════════════════════════════╝
```

<br/>

<a href="https://github.com/Muhammad-Taha-Ansari">
  <img src="https://img.shields.io/badge/GitHub-Muhammad--Taha--Ansari-00d4ff?style=for-the-badge&logo=github&logoColor=white&labelColor=0d1117"/>
</a>

<br/><br/>

<a href="https://github.com/Muhammad-Taha-Ansari">
  <img src="https://github-readme-stats.vercel.app/api?username=Muhammad-Taha-Ansari&show_icons=true&theme=github_dark&hide_border=true&bg_color=0d1117&title_color=00d4ff&icon_color=00d4ff&text_color=7fdbff&ring_color=00d4ff" alt="GitHub Stats"/>
</a>

</div>

---

<img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif" width="100%">

---

<div align="center">

### ⭐ Star this repo if it helped you!

<a href="https://github.com/Muhammad-Taha-Ansari/Humidity-And-Temperature-Sensor/stargazers">
  <img src="https://img.shields.io/github/stars/Muhammad-Taha-Ansari/Humidity-And-Temperature-Sensor?style=for-the-badge&logo=github&color=ffd700&labelColor=0d1117" alt="Stars"/>
</a>
<a href="https://github.com/Muhammad-Taha-Ansari/Humidity-And-Temperature-Sensor/network/members">
  <img src="https://img.shields.io/github/forks/Muhammad-Taha-Ansari/Humidity-And-Temperature-Sensor?style=for-the-badge&logo=github&color=00d4ff&labelColor=0d1117" alt="Forks"/>
</a>

<br/><br/>

```
Built with ❤️  in Embedded C — No HAL. No shortcuts. Just raw silicon and logic.
```

<!-- Footer Wave Animation -->
<img src="https://capsule-render.vercel.app/api?type=waving&color=0:0d1117,50:00d4ff,100:0d1117&height=120&section=footer&animation=fadeIn" width="100%"/>

</div>
