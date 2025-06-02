#include "Scheduler/BaseScheduler.h"

BaseScheduler::BaseScheduler()
    : m_currentCycle(0), m_lastProcessStartCycle(-1) {}

void BaseScheduler::LoadProcesses(const std::vector<std::shared_ptr<Process>>& processes) {
    m_processes = processes;
}

void BaseScheduler::Initialize() {
    m_currentCycle = 0;
    m_events.clear();
    m_lastProcessStartCycle = -1;
    
    for (auto& process : m_processes) {
        process->SetRemainingTime(process->GetBurstTime());
        process->Deactivate();
        process->Reset();
    }
    
    m_currentProcess = nullptr;
}

void BaseScheduler::ExecuteCycle() {
    std::shared_ptr<Process> nextProcess = GetNextProcess();
    
    for (auto& process : m_processes) {
        if (process->GetArrivalTime() <= m_currentCycle && 
            !process->IsComplete() && 
            process != nextProcess) {  
            process->IncrementWaitingTime();
        }
    }
    
    if (nextProcess != m_currentProcess) {
        if (m_currentProcess) {
            m_currentProcess->Deactivate();
            
            if (!m_events.empty() && m_events.back().pid == m_currentProcess->GetPID()) {
                m_events.back().duration = m_currentCycle - m_events.back().startCycle;
            }
        }
        
        m_currentProcess = nextProcess;
        
        if (m_currentProcess) {
            m_currentProcess->Activate();
            m_events.emplace_back(m_currentProcess->GetPID(), m_currentCycle, 1);
        }
    } else if (m_currentProcess) {
        if (!m_events.empty() && m_events.back().pid == m_currentProcess->GetPID()) {
            m_events.back().duration = m_currentCycle - m_events.back().startCycle + 1;
        }
    }
    
    if (m_currentProcess) {
        m_currentProcess->Execute();
    }
    
    m_currentCycle++;
}

bool BaseScheduler::IsComplete() const {
    for (const auto& process : m_processes) {
        if (!process->IsComplete()) {
            return false;
        }
    }
    return !m_processes.empty(); 
}

std::vector<ScheduleEvent> BaseScheduler::GetEvents() const {
    return m_events;
}

double BaseScheduler::CalculateAverageWaitingTime() const {
    if (m_processes.empty()) {
        return 0.0;
    }
    
    double totalWaitingTime = 0.0;
    
    for (const auto& process : m_processes) {
        if (process->IsComplete()) {
            int completionTime = 0;
            for (const auto& event : m_events) {
                if (event.pid == process->GetPID()) {
                    int eventEnd = event.startCycle + event.duration;
                    if (eventEnd > completionTime) {
                        completionTime = eventEnd;
                    }
                }
            }
            
            double waitingTime = completionTime - process->GetArrivalTime() - process->GetBurstTime();
            totalWaitingTime += waitingTime;
        } else {
            totalWaitingTime += process->GetWaitingTime();
        }
    }
    
    return totalWaitingTime / m_processes.size();
}

int BaseScheduler::GetCurrentCycle() const {
    return m_currentCycle;
}

double BaseScheduler::CalculateAlgorithmSpecificWT() const {
    return CalculateAverageWaitingTime();
}