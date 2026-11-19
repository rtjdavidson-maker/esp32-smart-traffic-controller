#pragma once
// Platform-independent C++ state machine; safe for desktop tests.
#include <stdint.h>

enum class Phase {
    NORTH_SOUTH_GREEN,
    NORTH_SOUTH_YELLOW,
    ALL_RED,
    EAST_WEST_GREEN,
    EAST_WEST_YELLOW,
    PEDESTRIAN_WALK
};

class TrafficController {
public:
    static constexpr uint32_t GREEN_MS = 6000;
    static constexpr uint32_t YELLOW_MS = 2000;
    static constexpr uint32_t ALL_RED_MS = 1000;
    static constexpr uint32_t WALK_MS = 4000;

    TrafficController() = default;
    void requestPedestrian() { pedestrianPending_ = true; }

    void tick(uint32_t deltaMs) {
        while (deltaMs > 0) {
            const uint32_t remaining = duration() - elapsedMs_;
            const uint32_t step = deltaMs < remaining ? deltaMs : remaining;
            elapsedMs_ += step;
            deltaMs -= step;
            if (elapsedMs_ == duration()) {
                advance();
                elapsedMs_ = 0;
            }
        }
    }

    Phase phase() const { return phase_; }
    bool pedestrianPending() const { return pedestrianPending_; }
    bool northSouthGreen() const { return phase_ == Phase::NORTH_SOUTH_GREEN; }
    bool eastWestGreen() const { return phase_ == Phase::EAST_WEST_GREEN; }
    bool walkEnabled() const { return phase_ == Phase::PEDESTRIAN_WALK; }
    uint32_t remainingMs() const { return duration() - elapsedMs_; }

private:
    Phase phase_ = Phase::NORTH_SOUTH_GREEN;
    Phase nextAfterRed_ = Phase::EAST_WEST_GREEN;
    bool pedestrianPending_ = false;
    uint32_t elapsedMs_ = 0;

    uint32_t duration() const {
        switch (phase_) {
            case Phase::NORTH_SOUTH_GREEN:
            case Phase::EAST_WEST_GREEN: return GREEN_MS;
            case Phase::NORTH_SOUTH_YELLOW:
            case Phase::EAST_WEST_YELLOW: return YELLOW_MS;
            case Phase::ALL_RED: return ALL_RED_MS;
            case Phase::PEDESTRIAN_WALK: return WALK_MS;
        }
        return ALL_RED_MS;
    }

    void advance() {
        switch (phase_) {
            case Phase::NORTH_SOUTH_GREEN:
                phase_ = Phase::NORTH_SOUTH_YELLOW; break;
            case Phase::NORTH_SOUTH_YELLOW:
                nextAfterRed_ = Phase::EAST_WEST_GREEN;
                phase_ = Phase::ALL_RED; break;
            case Phase::EAST_WEST_GREEN:
                phase_ = Phase::EAST_WEST_YELLOW; break;
            case Phase::EAST_WEST_YELLOW:
                nextAfterRed_ = Phase::NORTH_SOUTH_GREEN;
                phase_ = Phase::ALL_RED; break;
            case Phase::ALL_RED:
                if (pedestrianPending_) {
                    pedestrianPending_ = false;
                    phase_ = Phase::PEDESTRIAN_WALK;
                } else {
                    phase_ = nextAfterRed_;
                }
                break;
            case Phase::PEDESTRIAN_WALK:
                phase_ = nextAfterRed_; break;
        }
    }
};
