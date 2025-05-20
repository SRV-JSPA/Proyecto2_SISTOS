#include "Scheduler/FIFOScheduler.h"
#include <algorithm>

std::shared_ptr<Process> FIFOScheduler::GetNextProcess() {
    if (m_currentProcess && !m_currentProcess->IsComplete()) {
        return m_currentProcess;
    }
    
    std::shared_ptr<Process> nextProcess = nullptr;
    int earliestArrival = INT_MAX;
    
    for (auto& process : m_processes) {
        if (process->GetArrivalTime() <= m_currentCycle && !process->IsComplete()) {
            if (process->GetArrivalTime() < earliestArrival) {
                earliestArrival = process->GetArrivalTime();
                nextProcess = process;
            }
        }
    }
    
    return nextProcess;
}