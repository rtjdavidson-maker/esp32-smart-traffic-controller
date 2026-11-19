#include "../firmware/TrafficController/TrafficLogic.h"
#include <cassert>
#include <iostream>

void test_expected_sequence() {
    TrafficController c;
    assert(c.phase() == Phase::NORTH_SOUTH_GREEN);
    c.tick(6000);
    assert(c.phase() == Phase::NORTH_SOUTH_YELLOW);
    c.tick(2000);
    assert(c.phase() == Phase::ALL_RED);
    c.tick(1000);
    assert(c.phase() == Phase::EAST_WEST_GREEN);
    c.tick(6000 + 2000 + 1000);
    assert(c.phase() == Phase::NORTH_SOUTH_GREEN);
}

void test_pedestrian_waits_until_all_red() {
    TrafficController c;
    c.tick(500);
    c.requestPedestrian();
    assert(c.pedestrianPending());
    assert(!c.walkEnabled());
    c.tick(5500 + 2000);
    assert(c.phase() == Phase::ALL_RED);
    c.tick(1000);
    assert(c.walkEnabled());
    assert(!c.northSouthGreen() && !c.eastWestGreen());
    c.tick(4000);
    assert(c.phase() == Phase::EAST_WEST_GREEN);
}

void test_both_greens_never_on_simultaneously() {
    TrafficController c;
    for (int i = 0; i < 20000; ++i) {
        if (i % 37 == 0) c.requestPedestrian();
        c.tick(25);
        assert(!(c.northSouthGreen() && c.eastWestGreen()));
        assert(!(c.walkEnabled() && (c.northSouthGreen() || c.eastWestGreen())));
    }
}

void test_partial_ticks() {
    TrafficController c;
    c.tick(3000);
    assert(c.phase() == Phase::NORTH_SOUTH_GREEN);
    assert(c.remainingMs() == 3000);
    c.tick(2999);
    assert(c.phase() == Phase::NORTH_SOUTH_GREEN);
    c.tick(1);
    assert(c.phase() == Phase::NORTH_SOUTH_YELLOW);
}

void test_large_step_skips_no_transitions() {
    TrafficController incremental;
    TrafficController single;
    incremental.requestPedestrian();
    single.requestPedestrian();
    for (int i = 0; i < 250; ++i) incremental.tick(100);
    single.tick(25000);
    assert(incremental.phase() == single.phase());
    assert(incremental.remainingMs() == single.remainingMs());
    assert(incremental.pedestrianPending() == single.pedestrianPending());
}

int main() {
    test_expected_sequence();
    test_pedestrian_waits_until_all_red();
    test_both_greens_never_on_simultaneously();
    test_partial_ticks();
    test_large_step_skips_no_transitions();
    std::cout << "All 5 C++ controller tests passed.\n";
}
