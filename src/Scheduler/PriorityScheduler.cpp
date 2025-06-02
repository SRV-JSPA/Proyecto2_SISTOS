#include "Scheduler/PriorityScheduler.h"

std::shared_ptr<Process> PriorityScheduler::GetNextProcess() {
    if (m_currentProcess && !m_currentProcess->IsComplete()) {
        return m_currentProcess;
    }
    
    std::shared_ptr<Process> nextProcess = nullptr;
    int highestPriority = INT_MAX;
    
    for (auto& process : m_processes) {
        if (!process->IsComplete()) {
            if (process->GetPriority() < highestPriority) {
                highestPriority = process->GetPriority();
                nextProcess = process;
            }
        }
    }
    
    return nextProcess;
}

double PriorityScheduler::CalculateAlgorithmSpecificWT() const {
    if (m_processes.empty()) {
        return 0.0;
    }
    
    double totalWT = 0.0;
    
    for (const auto& process : m_processes) {
        int completionTime = 0;
        
        for (const auto& event : m_events) {
            if (event.pid == process->GetPID()) {
                int eventEndTime = event.startCycle + event.duration;
                if (eventEndTime > completionTime) {
                    completionTime = eventEndTime;
                }
            }
        }
        
        double waitingTime = completionTime - process->GetBurstTime();
        
        if (waitingTime < 0) {
            waitingTime = 0;
        }
        
        totalWT += waitingTime;
    }
    return totalWT / m_processes.size();
}