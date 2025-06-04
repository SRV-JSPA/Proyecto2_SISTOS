#include "Synchronization/MutexLock.h"
#include <iostream>

void MutexLock::ExecuteCycle() {
    int currentCycle = m_clock.GetCurrentCycle();
    
    std::cout << "\n=== MUTEX CYCLE " << currentCycle << " ===" << std::endl;
    
    for (auto it = m_activeOperations.begin(); it != m_activeOperations.end();) {
        if (it->second.endCycle <= currentCycle) {
            std::shared_ptr<Process> process = nullptr;
            for (auto& p : m_processes) {
                if (p->GetPID() == it->first) {
                    process = p;
                    break;
                }
            }
            
            std::shared_ptr<Resource> resource = m_resources[it->second.resourceName];
            if (resource && process) {
                resource->Release(process); 
                std::cout << "Released resource " << it->second.resourceName 
                         << " from process " << it->first << " at cycle " << currentCycle << std::endl;
            }
            it = m_activeOperations.erase(it);
        } else {
            ++it;
        }
    }
    
    for (auto& action : m_actions) {
        if (action->GetCycle() <= currentCycle && !action->IsExecuted()) {
            std::shared_ptr<Process> process = nullptr;
            
            for (auto& p : m_processes) {
                if (p->GetPID() == action->GetPID()) {
                    process = p;
                    break;
                }
            }
            
            if (!process) {
                std::cout << "ERROR: Process " << action->GetPID() << " not found!" << std::endl;
                continue;
            }
            
            if (process->GetArrivalTime() > currentCycle) {
                std::cout << "Process " << process->GetPID() << " hasn't arrived yet (AT=" 
                          << process->GetArrivalTime() << ", current=" << currentCycle << ")" << std::endl;
                continue;
            }
            
            if (m_activeOperations.find(process->GetPID()) != m_activeOperations.end()) {
                std::cout << "Process " << process->GetPID() << " is already executing another operation" << std::endl;
                continue;
            }
            
            std::shared_ptr<Resource> resource = m_resources[action->GetResourceName()];
            
            if (resource) {
                bool success = TryLock(process, resource);
                
                if (success) {
                    action->Execute(true);
                    
                    ActiveOperation op;
                    op.resourceName = resource->GetName();
                    op.startCycle = currentCycle;
                    op.endCycle = currentCycle + 1; 
                    m_activeOperations[process->GetPID()] = op;
                    
                    std::cout << "Process " << process->GetPID() 
                             << " acquired " << resource->GetName() 
                             << " at cycle " << currentCycle 
                             << " (action from cycle " << action->GetCycle() << ")" << std::endl;
                    
                    m_events.emplace_back(
                        process->GetPID(),
                        resource->GetName(),
                        action->GetType(),
                        false, 
                        currentCycle
                    );
                } else {
                    std::cout << "Process " << process->GetPID() 
                             << " BLOCKED waiting for " << resource->GetName() 
                             << " at cycle " << currentCycle 
                             << " (action from cycle " << action->GetCycle() << ")" << std::endl;
                    
                    m_events.emplace_back(
                        process->GetPID(),
                        resource->GetName(),
                        action->GetType(),
                        true, 
                        currentCycle
                    );
                }
            }
        }
    }
    
    std::cout << "Active operations at end of cycle " << currentCycle << ":" << std::endl;
    for (const auto& pair : m_activeOperations) {
        std::cout << "  " << pair.first << " using " << pair.second.resourceName 
                  << " until cycle " << pair.second.endCycle << std::endl;
    }
    
    std::cout << "Resource states:" << std::endl;
    for (const auto& pair : m_resources) {
        std::cout << "  " << pair.first << ": " << (pair.second->IsAvailable() ? "AVAILABLE" : "BUSY") << std::endl;
    }
    
    std::cout << "=== END MUTEX CYCLE " << currentCycle << " ===" << std::endl;
    
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