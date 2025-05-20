#include "Scheduler/SJFScheduler.h"
#include <algorithm>

std::shared_ptr<Process> SJFScheduler::GetNextProcess() {
    if (m_currentProcess && !m_currentProcess->IsComplete()) {
        return m_currentProcess;
    }
    
    std::shared_ptr<Process> nextProcess = nullptr;
    int shortestBurst = INT_MAX;
    
    for (auto& process : m_processes) {
        if (process->GetArrivalTime() <= m_currentCycle && !process->IsComplete()) {
            if (process->GetBurstTime() < shortestBurst) {
                shortestBurst = process->GetBurstTime();
                nextProcess = process;
            }
            else if (process->GetBurstTime() == shortestBurst && 
                     nextProcess && process->GetArrivalTime() < nextProcess->GetArrivalTime()) {
                nextProcess = process;
            }
        }
    }
    
    return nextProcess;
}