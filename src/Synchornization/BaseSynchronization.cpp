#include "Synchronization/BaseSynchronization.h"
#include <iostream>

void BaseSynchronization::LoadProcesses(const std::vector<std::shared_ptr<Process>>& processes) {
    m_processes = processes;
}

void BaseSynchronization::LoadResources(const std::map<std::string, std::shared_ptr<Resource>>& resources) {
    m_resources = resources;
}

void BaseSynchronization::LoadActions(const std::vector<std::shared_ptr<Action>>& actions) {
    m_actions = actions;
}

void BaseSynchronization::Initialize() {
    m_clock.Reset();
    m_events.clear();

    for (auto& process : m_processes) {
        process->SetRemainingTime(process->GetBurstTime());
        process->Deactivate();
    }

    for (auto& action : m_actions) {
    }
}

bool BaseSynchronization::IsComplete() const {
    std::cout << "\n--- Checking if simulation is complete ---" << std::endl;
    std::cout << "Total actions: " << m_actions.size() << std::endl;
    
    int executedCount = 0;
    for (const auto& action : m_actions) {
        bool executed = action->IsExecuted();
        std::cout << "Action " << action->GetPID() << " -> " << action->GetResourceName() 
                  << " at cycle " << action->GetCycle() 
                  << ": " << (executed ? "EXECUTED" : "NOT EXECUTED") << std::endl;
        if (executed) executedCount++;
    }
    
    bool isComplete = (executedCount == m_actions.size());
    std::cout << "Executed: " << executedCount << "/" << m_actions.size() 
              << " -> Complete: " << (isComplete ? "YES" : "NO") << std::endl;
    std::cout << "--- End completion check ---\n" << std::endl;
    
    return isComplete;
}
std::vector<SyncEvent> BaseSynchronization::GetEvents() const {
    return m_events;
}

int BaseSynchronization::GetCurrentCycle() const {
    return m_clock.GetCurrentCycle();
}
