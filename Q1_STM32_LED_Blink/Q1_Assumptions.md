# Q1 — Assumptions

## Hardware Assumptions

### 1. Pin Selection
- **LED connected to PA5** — An external LED is connected to PA5 via a
  220Ω current limiting resistor.
- **Tactile switch connected to PC13** — An external tactile switch is
  connected to PC13 which is routed to EXTI line 13 in hardware,
  allowing falling edge interrupt detection.

### 2. Tactile Switch Wiring
- One leg of the switch connects to **PC13**
- Other leg connects to **GND**
- **Internal pull-up resistor** is enabled in software on PC13
- This means idle state = HIGH (3.3V), pressed state = LOW (0V)
- A falling edge (HIGH → LOW) triggers the interrupt

### 3. LED Wiring
- LED **anode (+)** → 220Ω resistor → PA5
- LED **cathode (−)** → GND
- PA5 HIGH = LED ON, PA5 LOW = LED OFF
- 220Ω resistor limits current to safe level (~15mA at 3.3V)

### 4. Power Supply
- Board powered via **USB from PC** during development/testing
- 3.3V logic level used throughout

---

## Software Assumptions

### 5. Blink Frequency Implementation
- "Blink at X Hz" is interpreted as the LED toggling at X Hz
- This means the **full ON+OFF cycle** completes in 1/X seconds
- Half-period (time LED stays ON or OFF) = 1/(2*X) seconds:
  - 0.5 Hz → half period = **1000ms**
  - 1.0 Hz → half period = **500ms**
  - 2.0 Hz → half period = **250ms**

### 6. Initial State
- LED is **OFF at power on** before any button press
- This is state 0 (STATE_LED_OFF_INIT)

### 7. State Machine Cycling
- The 5th press returns to the **1st press state** (0.5 Hz blink)
- States cycle as: 0 → 1 → 2 → 3 → 4 → 1 → 2 → 3 → 4 → 1 ...
- Implemented using modulo operator: `(state + 1) % 5`

### 8. Button Debounce
- Real tactile switches produce **electrical bounce** — multiple
  rapid transitions when pressed once
- Software debounce of **200ms** is applied
- Any button press within 200ms of the previous one is ignored
- 200ms chosen as it is longer than typical bounce duration (~10-50ms)
  but short enough to not feel laggy to the user

### 9. Low Power Mode
- MCU enters **SLEEP mode** (not STOP or STANDBY) between events
- SLEEP mode was chosen because:
  - SysTick timer continues running in SLEEP mode
  - HAL_GetTick() keeps working for LED timing
  - Wake-up time is very fast (< 1 cycle)
  - STOP mode would require reconfiguring clocks on wake-up
- `PWR_MAINREGULATOR_ON` keeps the main voltage regulator active
- `PWR_SLEEPENTRY_WFI` uses the ARM WFI instruction

### 10. Clock Configuration
- Using **HSI (Internal 16 MHz oscillator)** — no external crystal needed
- PLL configured for accuracy of HAL_GetTick() timing
- SysTick fires every **1ms** providing the time base for LED toggling

### 11. Interrupt Priority
- SysTick priority = 0 (highest — ensures 1ms tick is accurate)
- EXTI4_15 (button) priority = 1 (lower than SysTick)
- This ensures button press never blocks the system tick

### 12. EXTI Callback
- Using `HAL_GPIO_EXTI_Falling_Callback()` — the newer HAL API
- Verified against the project's HAL driver source that this is
  the correct callback name for this HAL version

---

## Limitations and Notes

- The LED starts OFF and only starts blinking after the **first** button press
- If button is held down, only one state change occurs (debounce prevents multiple)
- No EEPROM/Flash storage — state resets to OFF on power cycle
- TIM3 is initialized but HAL_GetTick() (SysTick-based) is used for timing
  as it provides simpler and more accurate 1ms resolution

