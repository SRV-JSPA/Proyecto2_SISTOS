#include "Core/Resource.h"
#include <algorithm>

Resource::Resource(const std::string& name, int counter)
    : m_name(name), m_counter(counter) {}

std::string Resource::GetName() const {
    return m_name;
}

int Resource::GetCounter() const {
    return m_counter;
}

bool Resource::IsAvailable() const {
    return static_cast<int>(m_usingProcesses.size()) < m_counter;
}

bool Resource::Acquire(std::shared_ptr<Process> process) {
    if (IsAvailable()) {
        m_usingProcesses.push_back(process);
        return true;
    }
    return false;
}

void Resource::Release(std::shared_ptr<Process> process) {
    auto it = std::find(m_usingProcesses.begin(), m_usingProcesses.end(), process);
    if (it != m_usingProcesses.end()) {
        m_usingProcesses.erase(it);
    }
}