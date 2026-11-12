// Host-side unit tests for firmware/smart_plant_monitor/logic.h
// Build & run:  g++ -std=c++11 -I../firmware/smart_plant_monitor test_logic.cpp -o test_logic && ./test_logic
#include <cstdio>
#include "logic.h"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); failures++; } } while (0)

int main() {
  // Moisture conversion
  CHECK(moistureToPercent(1023, 1023, 400) == 0);
  CHECK(moistureToPercent(400, 1023, 400) == 100);
  CHECK(moistureToPercent(2000, 1023, 400) == 0);    // clamp low
  CHECK(moistureToPercent(0, 1023, 400) == 100);     // clamp high
  int mid = moistureToPercent(711, 1023, 400);
  CHECK(mid >= 49 && mid <= 51);

  // Pump turns ON when dry and cooldown elapsed
  CHECK(nextPumpState(false, 20, 35, 10, 0, 40000, 60000, 30000) == true);
  // ...but not during cooldown
  CHECK(nextPumpState(false, 20, 35, 10, 0, 1000, 60000, 30000) == false);
  // ...and not when moisture is fine
  CHECK(nextPumpState(false, 50, 35, 10, 0, 40000, 60000, 30000) == false);
  // Hysteresis: stays ON between threshold and threshold+hysteresis
  CHECK(nextPumpState(true, 40, 35, 10, 5000, 0, 60000, 30000) == true);
  // Turns OFF when wet enough
  CHECK(nextPumpState(true, 45, 35, 10, 5000, 0, 60000, 30000) == false);
  // Safety cut-off at max run time
  CHECK(nextPumpState(true, 10, 35, 10, 60000, 0, 60000, 30000) == false);

  if (failures == 0) printf("All tests passed\n");
  return failures;
}
