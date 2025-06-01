#pragma once
#include <string>

class Process {
private:
    std::string m_pid;
    int m_burstTime;
    int m_arrivalTime;
    int m_priority;
    int m_remainingTime;
    int m_waitingTime;
    bool m_isActive;

public:
    Process(const std::string& pid, int burstTime, int arrivalTime, int priority);

    std::string GetPID() const;
    int GetBurstTime() const;
    int GetArrivalTime() const;
    int GetPriority() const;
    int GetRemainingTime() const;
    int GetWaitingTime() const;
    bool IsActive() const;
    bool IsComplete() const;

    void SetRemainingTime(int time);
    void IncrementWaitingTime();
    void Execute(int timeSlice = 1);
    void Activate();
    void Deactivate();
    void Reset();
};
