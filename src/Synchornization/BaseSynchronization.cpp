#include "Synchronization/BaseSynchronization.h"

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
    for (const auto& action : m_actions) {
        if (!action->IsExecuted()) {
            return false;
        }
    }
    return true;
}

std::vector<SyncEvent> BaseSynchronization::GetEvents() const {
    return m_events;
}

int BaseSynchronization::GetCurrentCycle() const {
    return m_clock.GetCurrentCycle();
}
