# Blynk IoT Setup

1. Create an account at https://blynk.cloud and create a **Template** (hardware: ESP8266, connection: Wi-Fi).
2. Add these **Datastreams** (Virtual Pins):

| Pin | Name | Type | Range |
|---|---|---|---|
| V0 | Temperature | Double | 0-60 C |
| V1 | Humidity | Double | 0-100 % |
| V2 | Soil Moisture | Integer | 0-100 % |
| V3 | Pump Status | Integer | 0-255 |
| V4 | Manual Pump | Integer | 0-1 |
| V5 | Pest Alert | Integer | 0-255 |
| V6 | Auto Mode | Integer | 0-1 (default 1) |
| V7 | Threshold | Integer | 0-100 (default 35) |

3. Add an **Event** with code `pest_alert` (type Warning) and enable notifications.
4. Web/mobile dashboard widgets: Gauges for V0-V2, LED for V3 and V5, Switch for V4 and V6, Slider for V7, Chart for V0-V2.
5. Create a **Device** from the template and copy `BLYNK_TEMPLATE_ID`, `BLYNK_TEMPLATE_NAME` and `BLYNK_AUTH_TOKEN` into `secrets.h`.
