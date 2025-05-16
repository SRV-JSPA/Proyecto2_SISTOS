#include "Core/Process.h"

Process::Process(const std::string& pid, int burstTime, int arrivalTime, int priority)
    : m_pid(pid), m_burstTime(burstTime), m_arrivalTime(arrivalTime), m_priority(priority),
      m_remainingTime(burstTime), m_waitingTime(0), m_isActive(false) {}

std::string Process::GetPID() const {
    return m_pid;
}

int Process::GetBurstTime() const {
    return m_burstTime;
}

int Process::GetArrivalTime() const {
    return m_arrivalTime;
}

int Process::GetPriority() const {
    return m_priority;
}

int Process::GetRemainingTime() const {
    return m_remainingTime;
}

int Process::GetWaitingTime() const {
    return m_waitingTime;
}

bool Process::IsActive() const {
    return m_isActive;
}

bool Process::IsComplete() const {
    return m_remainingTime <= 0;
}

void Process::SetRemainingTime(int time) {
    m_remainingTime = time;
}

void Process::IncrementWaitingTime() {
    if (!m_isActive && !IsComplete()) {
        m_waitingTime++;
    }
}

void Process::Execute(int timeSlice) {
    if (m_isActive && m_remainingTime > 0) {
        m_remainingTime -= timeSlice;
        if (m_remainingTime < 0) {
            m_remainingTime = 0;
        }
    }
}

void Process::Activate() {
    m_isActive = true;
}

void Process::Deactivate() {
    m_isActive = false;
}