#include "Synchronization/Semaphore.h"

void Semaphore::Initialize() {
    BaseSynchronization::Initialize();
    
    m_waitingQueues.clear();
    
    for (const auto& pair : m_resources) {
        m_waitingQueues[pair.first] = std::queue<std::shared_ptr<Process>>();
    }
}

void Semaphore::ExecuteCycle() {
    int currentCycle = m_clock.GetCurrentCycle();
    
    for (auto& action : m_actions) {
        if (action->GetCycle() == currentCycle && !action->IsExecuted()) {
            std::shared_ptr<Process> process = nullptr;
            
            for (auto& p : m_processes) {
                if (p->GetPID() == action->GetPID()) {
                    process = p;
                    break;
                }
            }
            
            if (process) {
                std::shared_ptr<Resource> resource = m_resources[action->GetResourceName()];
                
                if (resource) {
                    bool success = TryLock(process, resource);
                    action->Execute(success);
                    
                    m_events.emplace_back(
                        process->GetPID(),
                        resource->GetName(),
                        action->GetType(),
                        !success,  
                        currentCycle
                    );
                    
                    if (!success) {
                        m_waitingQueues[resource->GetName()].push(process);
                    } else {
                        Unlock(process, resource);
                    }
                }
            }
        }
    }
    
    for (auto& pair : m_waitingQueues) {
        std::shared_ptr<Resource> resource = m_resources[pair.first];
        
        while (!pair.second.empty() && resource->IsAvailable()) {
            std::shared_ptr<Process> process = pair.second.front();
            pair.second.pop();
            
            bool success = TryLock(process, resource);
            
            if (success) {
                m_events.emplace_back(
                    process->GetPID(),
                    resource->GetName(),
                    ActionType::READ,  
                    false,
                    currentCycle
                );
                
                Unlock(process, resource);
            } else {
                pair.second.push(process);
                break;
            }
        }
    }
    
    m_clock.AdvanceCycle();
}

bool Semaphore::TryLock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource) {
    if (resource->IsAvailable()) {
        resource->Acquire(process);
        return true;
    }
    return false;
}

void Semaphore::Unlock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource) {
    resource->Release(process);
}