#include "Scheduler/RoundRobinScheduler.h"
#include <iostream> 

RoundRobinScheduler::RoundRobinScheduler(int quantum)
    : m_quantum(quantum), m_timeInQuantum(0) {}

void RoundRobinScheduler::Initialize() {
    BaseScheduler::Initialize();
    m_timeInQuantum = 0;
    
    std::queue<std::shared_ptr<Process>> empty;
    std::swap(m_readyQueue, empty);
    
    for (auto& process : m_processes) {
        if (process->GetArrivalTime() == 0) {
            m_readyQueue.push(process);
        }
    }
}

std::shared_ptr<Process> RoundRobinScheduler::GetNextProcess() {
    bool allCompleted = true;
    for (auto& process : m_processes) {
        if (!process->IsComplete()) {
            allCompleted = false;
            break;
        }
    }
    
    if (allCompleted) {
        return nullptr; 
    }
    
    for (auto& process : m_processes) {
        if (process->GetArrivalTime() <= m_currentCycle && 
            !process->IsComplete() && 
            !process->IsActive()) {
            
            bool alreadyInQueue = false;
            std::queue<std::shared_ptr<Process>> tempQueue = m_readyQueue;
            
            while (!tempQueue.empty()) {
                if (tempQueue.front()->GetPID() == process->GetPID()) {
                    alreadyInQueue = true;
                    break;
                }
                tempQueue.pop();
            }
            

            if (!alreadyInQueue && process->GetArrivalTime() == m_currentCycle) {
                m_readyQueue.push(process);
            }
        }
    }
    
    if (m_currentProcess) {
        if (m_currentProcess->IsComplete()) {
            m_currentProcess = nullptr;
            m_timeInQuantum = 0;
        }
        else if (m_timeInQuantum >= m_quantum) {
            m_readyQueue.push(m_currentProcess);
            m_currentProcess = nullptr;
            m_timeInQuantum = 0;
        }
        else {
            return m_currentProcess;
        }
    }
    
    if (!m_currentProcess && !m_readyQueue.empty()) {
        m_currentProcess = m_readyQueue.front();
        m_readyQueue.pop();
        m_timeInQuantum = 0;
    }
    
    return m_currentProcess;
}

void RoundRobinScheduler::ExecuteCycle() {
    for (auto& process : m_processes) {
        if (process->GetArrivalTime() <= m_currentCycle && 
            !process->IsActive() && 
            !process->IsComplete()) {
            process->IncrementWaitingTime();
        }
    }
    
    std::shared_ptr<Process> nextProcess = GetNextProcess();
    
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
    } 
    else if (m_currentProcess) {
        if (!m_events.empty() && m_events.back().pid == m_currentProcess->GetPID()) {
            m_events.back().duration = m_currentCycle - m_events.back().startCycle + 1;
        }
    }
    
    if (m_currentProcess) {
        m_currentProcess->Execute(1); 
        m_timeInQuantum++;
    }
    
    m_currentCycle++;
}

void RoundRobinScheduler::SetQuantum(int quantum) {
    m_quantum = quantum;
}

int RoundRobinScheduler::GetQuantum() const {
    return m_quantum;
}