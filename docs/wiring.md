# Wiring Guide

| Part | Part pin | NodeMCU |
|---|---|---|
| DHT11 | VCC / DATA / GND | 3V3 / D4 / GND |
| Soil moisture | VCC / AO / GND | 3V3 / A0 / GND |
| PIR | VCC / OUT / GND | 5V (VIN) / D5 / GND |
| Relay module | VCC / IN / GND | 5V (VIN) / D6 / GND |
| LCD I2C | VCC / SDA / SCL / GND | 5V (VIN) / D2 / D1 / GND |

## Pump circuit
Battery + -> Relay COM; Relay NO -> Pump +; Pump - -> Battery -. Share GND between the NodeMCU and the relay board's low-voltage side.

## Notes
- The ESP8266 A0 pin accepts 0-3.3V on NodeMCU boards (built-in divider). Power the soil sensor from 3V3.
- Use the relay in active-low mode by default; change `RELAY_ACTIVE_LOW` in `config.h` if the pump behaves inverted.
- If the LCD is blank, adjust the contrast pot on the I2C backpack and try address `0x3F`.
- Add your own circuit diagram image here, for example `docs/images/circuit.png`.
