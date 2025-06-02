#include "Scheduler/FIFOScheduler.h"
#include <algorithm>
#include <climits> 

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

double FIFOScheduler::CalculateAlgorithmSpecificWT() const {
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