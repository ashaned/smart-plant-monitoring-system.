#pragma once
// Pure logic (no hardware calls) so it can be unit-tested on a PC. See tests/.
#include <stdint.h>

inline int moistureToPercent(int raw, int rawDry, int rawWet) {
  if (rawDry == rawWet) return 0;
  long pct = (long)(rawDry - raw) * 100L / (rawDry - rawWet);
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  return (int)pct;
}

// Decide pump state with hysteresis, a max-run safety cut-off and a cooldown.
// Returns the NEW pump state (true = ON).
inline bool nextPumpState(bool pumpOn, int soilPct, int thresholdPct, int hysteresisPct,
                          unsigned long runningMs, unsigned long sinceStopMs,
                          unsigned long maxRunMs, unsigned long cooldownMs) {
  if (pumpOn) {
    if (runningMs >= maxRunMs) return false;                 // safety cut-off
    if (soilPct >= thresholdPct + hysteresisPct) return false; // wet enough
    return true;
  }
  if (soilPct < thresholdPct && sinceStopMs >= cooldownMs) return true;
  return false;
}
