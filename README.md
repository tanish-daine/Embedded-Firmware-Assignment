# Embedded-Firmware-Assignment
**Candidate:** Tanish Daine  
**Board:** NUCLEO-G070RB (STM32G070RB)  
**Cube IDE:** STM32CubeIDE v2.1.1  
**Date:** June 2026

---

## Repository Structure

```
├── Q1_STM32_LED_Blink/
│   ├── LED_Blink.zip
│       ├── Core/
│       │   ├── Inc/
│       │   │   ├── main.h
│       │   │   ├── stm32g0xx_hal_conf.h
│       │   │   └── stm32g0xx_it.h
│       │   └── Src/
│       │       ├── main.c          ← MAIN code
│       │       ├── stm32g0xx_hal_msp.c
│       │       ├── stm32g0xx_it.c
│       │       ├── syscalls.c
│       │       ├── sysmem.c
│       │       └── system_stm32g0xx.c
│       │   └── Startup/
│       │       └── startup_stm32g070rbtx.s
│       ├── Drivers/
│       │   ├── CMSIS/
│       │   └── STM32G0xx_HAL_Driver/
│       ├── Debug/
│       ├── LED_Blink.ioc
│       └── STM32G070RBTX_FLASH.ld
│    └── Q1_Assumptions.md       ← All assumptions for Q1
│
├── `Question2_Timer/
│   ├── Question2_Timer.c   ← Complete C program
│   └── Q2_Assumptions.md       ← All assumptions for Q2
│
└── README.md                ← This file
```

---

## Q1 — Switch & LED Blinking with STM32G070RB

### Overview
A tactile switch and external LED are connected to the NUCLEO-G070RB board.
The LED blinks at different frequencies depending on how many times the
switch has been pressed. The MCU uses low power SLEEP mode between operations.

### Behaviour
| Switch Press | LED Behaviour         |
|--------------|-----------------------|
| Power ON     | LED OFF               |
| 1st Press    | Blinks at 0.5 Hz      |
| 2nd Press    | Blinks at 1 Hz        |
| 3rd Press    | Blinks at 2 Hz        |
| 4th Press    | LED OFF               |
| 5th Press    | Back to 1st (repeats) |

### Hardware Used
| Component        | Value     | Purpose               |
|------------------|-----------|-----------------------|
| STM32 Pin PA5    | Output    | Drives external LED   |
| STM32 Pin PC13   | Input     | Reads tactile switch  |
| Resistor         | 220Ω      | Current limit for LED |
| Resistor         | Internal  | Pull-up on PC13       |
| Tactile Switch   | 4-pin     | User input            |
| LED              | Any color | Visual output         |

### Low Power Implementation
- MCU enters **SLEEP mode** using `HAL_PWR_EnterSLEEPMode()`
- Uses **WFI (Wait For Interrupt)** instruction
- Wakes on **SysTick** every 1ms to check LED toggle timing
- Wakes immediately on **button press** via EXTI interrupt
- Significantly reduces current draw vs busy-wait loop

---

## Q2 — Sensor Buffer Simulation (Online C Compiler)

### Overview
A C program that simulates two concurrent tasks using a single-threaded loop:
- **Timer Task:** Fires every second, generates 0–5 random bytes, adds to global buffer
- **Wake-up Task:** Fires every 10 seconds, checks if buffer has ≥ 50 bytes,
  prints latest 50 in hex, then deletes them

### How to Run
1. Go to [https://www.programiz.com/c-programming/online-compiler/](https://www.programiz.com/c-programming/online-compiler/)
3. Paste the contents of `Question2_Timer.c`
4. Click **Run**

### Example Output
```
[Second  1] Timer: Added 4 byte(s): 0xA3 0x2F 0xB1 0x09 | Buffer total: 4 bytes
[Second  2] Timer: Added 3 byte(s): 0x77 0xCC 0x11       | Buffer total: 7 bytes
...
[Second 10] ╔══ WAKE-UP TASK ══════════════════════╗
            ║ Buffer holds: 27 byte(s)              ║
            ╚═══════════════════════════════════════╝
            ✗ < 50 bytes. Nothing printed.
            Going back to sleep...
...
[Second 30] ╔══ WAKE-UP TASK ══════════════════════╗
            ║ Buffer holds: 75 byte(s)              ║
            ╚═══════════════════════════════════════╝
            ✓ >= 50 bytes present.
            Printing latest 50 bytes in hex:
            [Hex Dump - Second 30]
            0x13 0xE2 0xB8 0x09 0xEB 0xD3 0xAA 0xDD 0x57 0x93
            ...
            ✓ Deleted 50 bytes from buffer.
            Buffer size now: 25 bytes
```

---

## How to Build and Flash Q1

1. Open **STM32CubeIDE v2.1.1**
2. Import the project from `LED_Blink_1/`
3. Press **Ctrl+B** or Click the Hammer Symbol to build
4. Connect NUCLEO-G070RB via USB
5. Press **F11** to flash

---

## Dependencies

### Q1
- STM32CubeIDE v2.1.1
- STM32G0xx HAL Driver (included in project)
- ARM GCC Toolchain (bundled with CubeIDE)

### Q2
- Any standard C compiler
- Tested on: [programiz.com](https://www.programiz.com/c-programming/online-compiler/)
- Standard libraries used: `stdio.h`, `stdlib.h`, `time.h`, `string.h`

