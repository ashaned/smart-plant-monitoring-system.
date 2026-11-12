/*
  Smart Plant Monitoring System (SPMS)
  NodeMCU ESP8266 + DHT11 + soil moisture sensor + PIR + relay/pump + 16x2 I2C LCD + Blynk IoT

  Blynk virtual pins
    V0  Temperature (C)        -> Gauge / Chart
    V1  Humidity (%)           -> Gauge / Chart
    V2  Soil moisture (%)      -> Gauge / Chart
    V3  Pump status (0/1)      -> LED widget
    V4  Manual pump switch     -> Switch  (works when Auto mode is OFF)
    V5  Pest / motion alert    -> LED widget
    V6  Auto irrigation mode   -> Switch  (1 = automatic)
    V7  Moisture threshold (%) -> Slider 0-100
*/
#include "secrets.h"      // Blynk template/auth + WiFi (copy from secrets.h.example)
#include "config.h"
#include "logic.h"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

DHT dht(PIN_DHT, DHT_TYPE);
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);
BlynkTimer timer;

float temperature = NAN, humidity = NAN;
int soilPct = 0;
bool pumpOn = false;
bool autoMode = true;
bool manualPump = false;
int thresholdPct = DEFAULT_THRESHOLD_PCT;
bool pestDetected = false;

unsigned long pumpStartedAt = 0;
unsigned long pumpStoppedAt = 0;
unsigned long lastPestAlert = 0;

void setPump(bool on) {
  if (on == pumpOn) return;
  pumpOn = on;
  bool level = RELAY_ACTIVE_LOW ? !on : on;
  digitalWrite(PIN_RELAY, level ? HIGH : LOW);
  if (on) pumpStartedAt = millis(); else pumpStoppedAt = millis();
  Blynk.virtualWrite(V3, on ? 255 : 0);
  Serial.println(on ? F("Pump ON") : F("Pump OFF"));
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V4, V6, V7);
}
BLYNK_WRITE(V4) { manualPump = param.asInt(); if (!autoMode) setPump(manualPump); }
BLYNK_WRITE(V6) { autoMode = param.asInt(); if (!autoMode) setPump(manualPump); }
BLYNK_WRITE(V7) { thresholdPct = constrain(param.asInt(), 0, 100); }

void updateLcd() {
  lcd.setCursor(0, 0);
  char l1[17];
  snprintf(l1, sizeof(l1), "T:%2.0fC H:%2.0f%% %s", isnan(temperature) ? 0 : temperature,
           isnan(humidity) ? 0 : humidity, pumpOn ? "P:ON " : "P:OFF");
  lcd.print(l1);
  lcd.setCursor(0, 1);
  char l2[17];
  snprintf(l2, sizeof(l2), "Soil:%3d%% %s %s", soilPct, autoMode ? "A" : "M", pestDetected ? "PEST" : "    ");
  lcd.print(l2);
}

void readAndControl() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t)) temperature = t; else Serial.println(F("DHT read failed"));
  if (!isnan(h)) humidity = h;

  soilPct = moistureToPercent(analogRead(A0), SOIL_RAW_DRY, SOIL_RAW_WET);
  pestDetected = digitalRead(PIN_PIR) == HIGH;

  // Irrigation
  unsigned long now = millis();
  if (autoMode) {
    bool next = nextPumpState(pumpOn, soilPct, thresholdPct, HYSTERESIS_PCT,
                              pumpOn ? now - pumpStartedAt : 0,
                              pumpOn ? 0 : now - pumpStoppedAt,
                              MAX_PUMP_RUN_MS, PUMP_COOLDOWN_MS);
    setPump(next);
  } else if (pumpOn && now - pumpStartedAt >= MAX_PUMP_RUN_MS) {
    setPump(false);              // safety cut-off also applies in manual mode
    manualPump = false;
    Blynk.virtualWrite(V4, 0);
  }

  // Cloud updates
  if (!isnan(temperature)) Blynk.virtualWrite(V0, temperature);
  if (!isnan(humidity))    Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, soilPct);
  Blynk.virtualWrite(V5, pestDetected ? 255 : 0);

  if (pestDetected && (lastPestAlert == 0 || now - lastPestAlert > PEST_ALERT_COOLDOWN_MS)) {
    lastPestAlert = now;
    Blynk.logEvent("pest_alert", "Motion/pest detected near the plants");
  }

  Serial.printf("T=%.1f H=%.1f Soil=%d%% Pump=%d Auto=%d PIR=%d\n",
                temperature, humidity, soilPct, pumpOn, autoMode, pestDetected);
  updateLcd();
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, RELAY_ACTIVE_LOW ? HIGH : LOW);  // pump off at boot
  pumpStoppedAt = millis() - PUMP_COOLDOWN_MS;              // allow first run immediately

  dht.begin();
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.print(F("Smart Plant Mon."));
  lcd.setCursor(0, 1);
  lcd.print(F("Connecting WiFi"));

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  lcd.clear();
  timer.setInterval(SENSOR_INTERVAL_MS, readAndControl);
}

void loop() {
  Blynk.run();
  timer.run();
}
