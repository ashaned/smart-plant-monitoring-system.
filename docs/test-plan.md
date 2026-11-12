# Test Plan

Approach: unit testing (components), integration testing (components together), system testing (whole system).

| Phase | Objective | Criteria |
|---|---|---|
| Unit | Validate each sensor/component | Meets functional spec |
| Integration | Components work together | Data reaches Blynk |
| System | Full system in real conditions | Runs without failure |
| Performance | Behaviour under varied conditions | Consistent results |
| Security | Data and access integrity | No vulnerabilities found |

## Test cases
| # | Scenario | Steps | Expected | Result |
|---|---|---|---|---|
| 1 | Soil moisture accuracy | Insert sensor in moist soil; read value in Blynk | Accurate moisture % shown | |
| 2 | Automated irrigation | Set threshold; let soil dry below it | Pump starts automatically, stops at threshold + hysteresis | |
| 3 | Temperature/humidity | Place sensor in test environment; compare with a reference | Accurate readings | |
| 4 | Pest detection | Move in front of PIR | Alert in Blynk + notification | |
| 5 | Integration | Run all sensors together | All data arrives without interruption | |
| 6 | Safety cut-off | Keep soil dry for >60 s of pumping | Pump stops at max run time | |

Fill in the Result column (Pass/Fail + date) as you test. Control-logic unit tests live in `tests/test_logic.cpp`.
