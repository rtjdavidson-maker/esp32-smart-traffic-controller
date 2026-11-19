// Educational ESP32 LED traffic controller; NOT for use on real roads.
#include "TrafficLogic.h"

constexpr int NS_RED = 14;
constexpr int NS_YELLOW = 27;
constexpr int NS_GREEN = 26;
constexpr int EW_RED = 25;
constexpr int EW_YELLOW = 33;
constexpr int EW_GREEN = 32;
constexpr int WALK_LED = 23;
constexpr int BUTTON = 18;  // Push button from GPIO18 to GND

TrafficController controller;
uint32_t lastUpdateMs = 0;
bool previousButton = HIGH;
uint32_t lastRequestMs = 0;

void renderOutputs() {
    const Phase phase = controller.phase();
    const bool nsGreen = phase == Phase::NORTH_SOUTH_GREEN;
    const bool nsYellow = phase == Phase::NORTH_SOUTH_YELLOW;
    const bool ewGreen = phase == Phase::EAST_WEST_GREEN;
    const bool ewYellow = phase == Phase::EAST_WEST_YELLOW;
    digitalWrite(NS_RED, !nsGreen && !nsYellow);
    digitalWrite(NS_YELLOW, nsYellow);
    digitalWrite(NS_GREEN, nsGreen);
    digitalWrite(EW_RED, !ewGreen && !ewYellow);
    digitalWrite(EW_YELLOW, ewYellow);
    digitalWrite(EW_GREEN, ewGreen);
    digitalWrite(WALK_LED, controller.walkEnabled());
}

void setup() {
    const int pins[] = {NS_RED, NS_YELLOW, NS_GREEN, EW_RED,
                        EW_YELLOW, EW_GREEN, WALK_LED};
    for (int pin : pins) pinMode(pin, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);
    Serial.begin(115200);
    lastUpdateMs = millis();
    renderOutputs();
}

void loop() {
    const uint32_t now = millis();
    const bool button = digitalRead(BUTTON);
    if (previousButton == HIGH && button == LOW && (now - lastRequestMs) >= 200) {
        controller.requestPedestrian();
        lastRequestMs = now;
        Serial.println("Pedestrian request queued");
    }
    previousButton = button;
    controller.tick(now - lastUpdateMs);
    lastUpdateMs = now;
    renderOutputs();
    delay(10);
}
