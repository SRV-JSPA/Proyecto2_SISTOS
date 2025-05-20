#include "Scheduler/PriorityScheduler.h"

std::shared_ptr<Process> PriorityScheduler::GetNextProcess() {
    if (m_currentProcess && !m_currentProcess->IsComplete()) {
        return m_currentProcess;
    }
    
    std::shared_ptr<Process> nextProcess = nullptr;
    int highestPriority = INT_MAX;
    
    for (auto& process : m_processes) {
        if (process->GetArrivalTime() <= m_currentCycle && !process->IsComplete()) {
            if (process->GetPriority() < highestPriority) {
                highestPriority = process->GetPriority();
                nextProcess = process;
            }
        }
    }
    
    return nextProcess;
}