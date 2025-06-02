#include "Scheduler/SJFScheduler.h"
#include <climits> 

std::shared_ptr<Process> SJFScheduler::GetNextProcess() {
    if (m_currentProcess && !m_currentProcess->IsComplete()) {
        return m_currentProcess;
    }
    
    std::shared_ptr<Process> selectedProcess = nullptr;
    int shortestBurstTime = INT_MAX;
    
    for (auto& process : m_processes) {
        if (!process->IsComplete()) {
            
            if (process->GetBurstTime() < shortestBurstTime) {
                shortestBurstTime = process->GetBurstTime();
                selectedProcess = process;
            }
            else if (process->GetBurstTime() == shortestBurstTime && selectedProcess) {
                if (process->GetArrivalTime() < selectedProcess->GetArrivalTime()) {
                    selectedProcess = process;
                }
                else if (process->GetArrivalTime() == selectedProcess->GetArrivalTime()) {
                }
            }
        }
    }
    
    return selectedProcess;
}

double SJFScheduler::CalculateAlgorithmSpecificWT() const {
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