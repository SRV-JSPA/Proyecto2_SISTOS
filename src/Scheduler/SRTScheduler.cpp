#include "Scheduler/SRTScheduler.h"

std::shared_ptr<Process> SRTScheduler::GetNextProcess() {
    std::shared_ptr<Process> nextProcess = nullptr;
    int shortestRemaining = INT_MAX;
    
    for (auto& process : m_processes) {
        if (process->GetArrivalTime() <= m_currentCycle && !process->IsComplete()) {
            if (process->GetRemainingTime() < shortestRemaining) {
                shortestRemaining = process->GetRemainingTime();
                nextProcess = process;
            }
        }
    }
    
    return nextProcess;
}

double SRTScheduler::CalculateAlgorithmSpecificWT() const {
    return CalculateAverageWaitingTime();
}