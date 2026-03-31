# 🌿 House Plant Companion

An automated, real-time environmental monitoring system for indoor plants, built on the **STM32F103RB** (ARM Cortex-M3) platform. This project utilizes a multi-sensor array to track soil hydration and ambient light levels, providing visual and textual feedback to ensure optimal plant health.

---

## ✨ Key Features

- **Dual-Sensor Monitoring:** Integrates a BH1750 digital light sensor (Lux) and a SEN0193 capacitive soil moisture sensor.
- **Priority-Based Alert System:** Employs a state-machine logic that prioritizes hydration (`NEED WATER!`) over light levels (`LOW LIGHT`) to ensure survival-critical needs are met first.
- **12-Bit Precision:** Leverages the STM32's 12-bit ADC for high-resolution moisture tracking (0−4095 range).
- **I2C Bus Efficiency:** Shares a single I2C bus between the LCD and the light sensor to minimize pin usage.
- **Hybrid Display Logic:** Updates the status row only on state changes to prevent flickering, while providing a real-time data stream on the second row.
- **Live Moisture %'s and LUX levels:** Shows the user the moisture percentage of the soil and also the current LUX reading coming from the sensor.
---

## 🛠 Hardware Configuration

| Component                  | Interface         | STM32 Pin            | Function                              |
|---------------------------|-------------------|----------------------|---------------------------------------|
| Capacitive Moisture Sensor | Analog (ADC)      | PA0                  | Measures soil moisture via voltage    |
| BH1750 Light Sensor        | Digital (I2C)     | PB6 (SCL), PB7 (SDA) | Measures ambient Lux                  |
| 16x2 LCD Display           | Digital (I2C)     | PB6 (SCL), PB7 (SDA) | User Interface                        |
| Common Cathode RGB LED     | Digital (GPIO)    | PA8, PA9, PA10       | Visual Status Alerts                  |

---

## 🔬 Technical Implementation

### I2C Shared Bus Architecture

The system utilizes the I2C serial protocol to communicate with the LCD backpack and the BH1750 sensor. By using unique 7-bit addresses (`0x20` for the LCD and `0x23` for the sensor), the microcontroller can query multiple devices using only two wires plus pull-up resistors.

### 12-Bit ADC Calibration

To convert raw analog voltage into a human-readable percentage, the system uses a linear mapping formula that accounts for the inverse ratio of the capacitive sensor (higher voltage = drier soil):

```
Moisture% = (AIR_VALUE - currentReading) / (AIR_VALUE - WATER_VALUE) * 100
```

- **AIR_VALUE** (`3500`): Raw 12-bit value in open air (0% moisture).
- **WATER_VALUE** (`500`): Raw 12-bit value fully submerged (100% moisture).

### Firmware State Machine

The core logic resides in a super-loop that evaluates sensor data against calibrated thresholds:

1. **State 1 — Critical:** If `Moisture > DRY_THRESHOLD` → Red LED + `"NEED WATER!"`
2. **State 2 — Warning:** If `Lux < LIGHT_THRESHOLD` → Blue LED + `"LOW LIGHT"`
3. **State 3 — Normal:** All conditions met → Green LED + `"PLANT: OK"`

---

## 📂 Project Structure

| File | Description |
|------|-------------|
| `main.c` | Core application logic and state machine |
| `LCD.c/h` | I2C character display drivers and 4-bit initialization |
| `I2C.c/h` | Low-level I2C bus driver and address management |
| `adc.c/h` | 12-bit Analog-to-Digital Converter configuration |
| `bh1750.c/h` | Digital light sensor driver |