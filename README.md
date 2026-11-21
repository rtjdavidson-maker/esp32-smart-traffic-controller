# ESP32 Smart Traffic Controller

An educational **embedded C++ and ESP32** engineering portfolio project featuring a six-phase traffic controller, pedestrian requests, all-red clearance intervals, desktop C++ tests, and an interactive browser demonstration.

> **Safety:** This is a classroom prototype, not a certified road controller. Never use it to control real traffic lights. Physical ESP32 hardware has not yet been validated.

## Open the interactive simulator (no hardware needed)

1. Download the repository as a ZIP and extract it.
2. Open `simulator/index.html` in Edge or Chrome.
3. Click **Request pedestrian crossing** and watch the request wait for the all-red interval.
4. Use **Pause**, **Reset**, and **Speed** to experiment.

The browser simulator is JavaScript illustrating the C++ controller logic, not compiled ESP32 firmware.

## State machine

```text
North/South GREEN → North/South YELLOW → ALL RED
                                         ↓
                      [requested? pedestrian WALK]
                                         ↓
East/West GREEN ←────────────────────────┘
        ↓
East/West YELLOW → ALL RED → [requested? pedestrian WALK] → repeat
```

| Phase | Duration |
|---|---:|
| Each vehicle green | 6 seconds |
| Vehicle yellow | 2 seconds |
| All-red clearance | 1 second |
| Pedestrian WALK, if requested | 4 seconds |

**Safety invariants:** opposing vehicle greens are never simultaneously on; pedestrian WALK is never enabled with a vehicle green. Pedestrian requests are queued until after all-red clearance.

## Repository structure

```text
firmware/TrafficController/TrafficController.ino   ESP32 Arduino sketch and GPIO mapping
firmware/TrafficController/TrafficLogic.h          Platform-independent C++ state machine
simulator/index.html                               Interactive browser simulator
tests/test_traffic_logic.cpp                       Five unit tests
.github/workflows/cpp-tests.yml                    GitHub Actions build and test
```

## Run the automated C++ tests

GitHub Actions runs these automatically after each push. With a desktop C++ compiler:

```bash
g++ -std=c++17 -Wall -Wextra -Werror -o traffic-tests tests/test_traffic_logic.cpp
./traffic-tests
```

## Simulator screenshots and pedestrian crossing demonstration

These screenshots were captured from the locally running **browser simulator**. They demonstrate the traffic-control sequence and the queued pedestrian request; they are **not evidence of testing on a physical ESP32**.

### Normal traffic operation

![Interactive controller showing timed vehicle signals](Screenshot%202026-11-19%20132458.png)

![Simulator event log showing signal phase transitions](Screenshot%202026-11-19%20132528.png)

### Pedestrian Crossing Safety Demonstration

**1. Pedestrian request queued during vehicle GREEN.** The North/South signal remains green, East/West stays red, and the WALK signal is off while the request is pending.

![Pedestrian crossing queued while vehicle traffic is green](Screenshot%202026-11-19%20132739.png)

**2. Pedestrian WALK after all-red clearance.** Both vehicle signals display red while WALK is active. The event log shows the all-red phase occurred before WALK.

![Pedestrian WALK active with both vehicle signals red](Screenshot%202026-11-19%20132757.png)

The desktop C++ unit tests also cover the phase transitions, pedestrian requests, and prevention of conflicting vehicle greens. A real-world safety certification or physical hardware test has **not** been performed.

## Future physical ESP32 wiring (not yet verified)

| Signal | GPIO |
|---|---:|
| North/South red, yellow, green | 14, 27, 26 |
| East/West red, yellow, green | 25, 33, 32 |
| Pedestrian WALK LED | 23 |
| Pedestrian push button (active-low) | 18 |

Use current-limiting resistors for LEDs and common ground. Do not connect high-voltage or high-current loads directly to GPIO pins. ESP32 implementation requires hardware testing before any physical demonstration.

## Skills demonstrated

C++17, finite-state machines, non-blocking timing with `millis()`, GPIO, pedestrian request handling, safety checks, unit tests, GitHub Actions CI, browser prototyping, and technical documentation.

## Next steps

Connect LEDs and a button to a physical ESP32 on a low-voltage breadboard, add hardware tests and event logs, and document measurements and screenshots.
