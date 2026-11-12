#pragma once
// ---------- Pin map (NodeMCU ESP8266) ----------
#define PIN_DHT    D4   // DHT11 data
#define PIN_PIR    D5   // PIR motion sensor output
#define PIN_RELAY  D6   // Relay module IN (controls water pump)
// Soil moisture sensor analog output -> A0
// LCD I2C: SDA -> D2, SCL -> D1 (default Wire pins)

#define DHT_TYPE DHT11
#define LCD_ADDR 0x27   // try 0x3F if the LCD stays blank

// ---------- Relay logic ----------
// Most cheap relay boards are ACTIVE-LOW (LOW = relay on). Set false if yours is active-high.
#define RELAY_ACTIVE_LOW true

// ---------- Soil sensor calibration ----------
// Read the raw A0 value with the probe in dry air and in a glass of water, then put them here.
#define SOIL_RAW_DRY 1023   // raw value when completely dry
#define SOIL_RAW_WET 400    // raw value when fully wet

// ---------- Irrigation behaviour ----------
#define DEFAULT_THRESHOLD_PCT 35   // pump ON below this soil moisture %
#define HYSTERESIS_PCT        10   // pump OFF at threshold + hysteresis (avoids rapid on/off)
#define MAX_PUMP_RUN_MS       60000UL  // safety cut-off: max continuous pump time (60 s)
#define PUMP_COOLDOWN_MS      30000UL  // wait after a run before pump can auto-start again

// ---------- Timing ----------
#define SENSOR_INTERVAL_MS 2000UL
#define PEST_ALERT_COOLDOWN_MS 60000UL // min gap between pest notifications
