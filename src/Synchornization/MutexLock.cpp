#include "Synchronization/MutexLock.h"

void MutexLock::ExecuteCycle() {
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

                    if (success) {
                        Unlock(process, resource);
                    }
                }
            }
        }
    }
    
    m_clock.AdvanceCycle();
}

bool MutexLock::TryLock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource) {
    if (resource->IsAvailable()) {
        resource->Acquire(process);
        return true;
    }
    return false;
}

void MutexLock::Unlock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource) {
    resource->Release(process);
}