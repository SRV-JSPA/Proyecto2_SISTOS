#include "Core/Action.h"

Action::Action(const std::string& pid, ActionType type, const std::string& resourceName, int cycle)
    : m_pid(pid), m_type(type), m_resourceName(resourceName), m_cycle(cycle),
      m_executed(false), m_successful(false) {}

std::string Action::GetPID() const {
    return m_pid;
}

ActionType Action::GetType() const {
    return m_type;
}

std::string Action::GetResourceName() const {
    return m_resourceName;
}

int Action::GetCycle() const {
    return m_cycle;
}

bool Action::IsExecuted() const {
    return m_executed;
}

bool Action::WasSuccessful() const {
    return m_successful;
}

void Action::Execute(bool success) {
    m_executed = true;
    m_successful = success;
}