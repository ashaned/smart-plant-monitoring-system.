# Smart Plant Monitoring System (SPMS)

An IoT system that monitors soil moisture, temperature and humidity, automatically waters plants, detects motion/pests, and lets you monitor and control everything from the **Blynk** mobile/web app.

Built as part of the Pearson BTEC Level 5 HND in Computing (Unit 6: Planning a Computing Project).

## Features
- Real-time temperature, humidity and soil moisture monitoring
- Automatic irrigation with a moisture threshold, hysteresis, cooldown and a max-run safety cut-off
- Manual pump control from the Blynk app
- PIR motion / pest detection with push notification
- Local readout on a 16x2 I2C LCD
- Adjustable threshold from the app (no re-flashing)

## Hardware
| Component | Purpose |
|---|---|
| NodeMCU ESP8266 (ESP-12E) | Microcontroller + Wi-Fi |
| DHT11 | Temperature and humidity |
| Soil moisture sensor (analog) | Soil moisture |
| PIR motion sensor | Pest / motion detection |
| 5V 1-channel relay module | Switches the pump |
| 3-6V mini submersible pump | Irrigation |
| 16x2 LCD with I2C backpack | Local display |
| 7.4V battery (+ regulator to 5V for the NodeMCU, see note) | Power |
| Breadboard + jumper wires | Wiring |

## Wiring
| Part | Pin on part | NodeMCU pin |
|---|---|---|
| DHT11 | DATA | D4 |
| Soil moisture | AO | A0 |
| PIR | OUT | D5 |
| Relay | IN | D6 |
| LCD I2C | SDA | D2 |
| LCD I2C | SCL | D1 |
| All sensors/LCD | VCC / GND | 3V3 or 5V (check each module) / GND |

Relay wiring: pump power goes through the relay **COM** and **NO** terminals. Do not power the pump from the NodeMCU pin. See [docs/wiring.md](docs/wiring.md).

> **Power note:** the NodeMCU `VIN` needs ~5V. If you use a 7.4V battery, step it down with a buck converter/regulator first.

## Software setup
1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Add the ESP8266 board: *File > Preferences > Additional boards manager URLs* ->
   `http://arduino.esp8266.com/stable/package_esp8266com_index.json`, then install **esp8266** in Boards Manager. Select **NodeMCU 1.0 (ESP-12E Module)**.
3. Install libraries (Library Manager): **Blynk**, **DHT sensor library** (Adafruit) + **Adafruit Unified Sensor**, **LiquidCrystal I2C**.
4. Create the Blynk template (see [docs/blynk-setup.md](docs/blynk-setup.md)).
5. In `firmware/smart_plant_monitor/` copy `secrets.h.example` to `secrets.h` and fill in your Wi-Fi and Blynk values.
6. Calibrate the soil sensor in `config.h` (`SOIL_RAW_DRY`, `SOIL_RAW_WET`) using the Serial Monitor at 115200 baud.
7. Open `smart_plant_monitor.ino`, upload, and open the Blynk app.

## Repository layout
```
firmware/smart_plant_monitor/   Arduino sketch, config, logic, secrets example
tests/                          Host-side unit tests for the control logic
docs/                           Wiring, Blynk setup, test plan, project report notes
```

## Testing
- Control logic unit tests (run on your PC): `cd tests && g++ -std=c++11 -I../firmware/smart_plant_monitor test_logic.cpp -o test_logic && ./test_logic`
- Hardware test cases: [docs/test-plan.md](docs/test-plan.md)

## Limitations
Needs stable Wi-Fi for remote monitoring; the DHT11 and the resistive soil sensor have limited accuracy; the ESP8266 has limited processing power. Possible future work: capacitive soil sensor, solar power, offline mode, data logging/analytics.

## License
MIT. See [LICENSE](LICENSE).
