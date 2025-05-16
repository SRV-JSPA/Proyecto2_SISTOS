#include "Core/SimulationClock.h"

SimulationClock::SimulationClock() : m_currentCycle(0) {}

int SimulationClock::GetCurrentCycle() const {
    return m_currentCycle;
}

void SimulationClock::AdvanceCycle() {
    m_currentCycle++;
}

void SimulationClock::Reset() {
    m_currentCycle = 0;
}