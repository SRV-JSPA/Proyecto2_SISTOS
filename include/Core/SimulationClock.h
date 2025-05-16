#pragma once

class SimulationClock {
private:
    int m_currentCycle;

public:
    SimulationClock();

    int GetCurrentCycle() const;
    void AdvanceCycle();
    void Reset();
};