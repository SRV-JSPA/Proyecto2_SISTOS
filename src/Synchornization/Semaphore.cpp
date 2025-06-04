#include "Synchronization/Semaphore.h"
#include <iostream>

void Semaphore::Initialize() {
    BaseSynchronization::Initialize();
    
    m_waitingQueues.clear();
    m_activeUsages.clear();  

    
    for (const auto& pair : m_resources) {
        m_waitingQueues[pair.first] = std::queue<std::shared_ptr<Process>>();
        m_activeUsages[pair.first] = std::vector<ResourceUsage>();  


    }
}
void Semaphore::ExecuteCycle() {
    int currentCycle = m_clock.GetCurrentCycle();
    
    std::cout << "\n=== SEMAPHORE CYCLE " << currentCycle << " ===" << std::endl;
    std::cout << "Total actions loaded: " << m_actions.size() << std::endl;
    
    for (const auto& action : m_actions) {
        std::cout << "Action: " << action->GetPID() << " -> " << action->GetResourceName() 
                  << " at cycle " << action->GetCycle() 
                  << " (executed: " << (action->IsExecuted() ? "YES" : "NO") << ")" << std::endl;
    }
    
    for (auto& pair : m_activeUsages) {
        std::string resourceName = pair.first;
        std::vector<ResourceUsage>& usages = pair.second;
        std::shared_ptr<Resource> resource = m_resources[resourceName];
        
        for (auto it = usages.begin(); it != usages.end();) {
            if (it->endCycle <= currentCycle) {
                std::cout << "Releasing resource " << resourceName << " from process " << it->process->GetPID() << std::endl;
                resource->Release(it->process);
                it = usages.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    std::vector<std::shared_ptr<Action>> currentCycleActions;
    
    for (auto& action : m_actions) {
        if (action->GetCycle() == currentCycle && !action->IsExecuted()) {
            currentCycleActions.push_back(action);
            std::cout << "Found action for cycle " << currentCycle << ": " 
                      << action->GetPID() << " -> " << action->GetResourceName() << std::endl;
        }
    }
    
    std::cout << "Actions to process this cycle: " << currentCycleActions.size() << std::endl;
    
    for (auto& action : currentCycleActions) {
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
        
        std::cout << "Processing action: " << process->GetPID() << " -> " << action->GetResourceName() << std::endl;
        
        std::shared_ptr<Resource> resource = m_resources[action->GetResourceName()];
        
        if (resource) {
            bool success = TryLock(process, resource);
            action->Execute(success);  
            
            std::cout << "Action result: " << (success ? "SUCCESS" : "WAITING") << std::endl;
            
            m_events.emplace_back(
                process->GetPID(),
                resource->GetName(),
                action->GetType(),
                !success,  
                currentCycle
            );
            
            if (success) {
                m_activeUsages[resource->GetName()].emplace_back(process, currentCycle + 1);
                std::cout << "Resource " << resource->GetName() << " acquired by " << process->GetPID() 
                          << " until cycle " << (currentCycle + 1) << std::endl;
            } else {
                m_waitingQueues[resource->GetName()].push(process);
                std::cout << "Process " << process->GetPID() << " added to waiting queue for " << resource->GetName() << std::endl;
            }
        } else {
            std::cout << "ERROR: Resource " << action->GetResourceName() << " not found!" << std::endl;
        }
    }
    
    for (auto& pair : m_waitingQueues) {
        std::string resourceName = pair.first;
        std::queue<std::shared_ptr<Process>>& waitingQueue = pair.second;
        std::shared_ptr<Resource> resource = m_resources[resourceName];
        
        if (!waitingQueue.empty()) {
            std::cout << "Processing waiting queue for " << resourceName << " (size: " << waitingQueue.size() << ")" << std::endl;
        }
        
        std::vector<std::shared_ptr<Process>> stillWaiting;
        
        while (!waitingQueue.empty()) {
            std::shared_ptr<Process> process = waitingQueue.front();
            waitingQueue.pop();
            
            if (process->GetArrivalTime() > currentCycle) {
                std::cout << "Waiting process " << process->GetPID() << " hasn't arrived yet (AT=" << process->GetArrivalTime() << ")" << std::endl;
                stillWaiting.push_back(process);
                continue;
            }
            
            bool success = TryLock(process, resource);
            
            if (success) {
                std::cout << "Waiting process " << process->GetPID() << " granted access to " << resourceName << std::endl;
                
                m_events.emplace_back(
                    process->GetPID(),
                    resource->GetName(),
                    ActionType::READ,  
                    false,  
                    currentCycle
                );
                
                m_activeUsages[resourceName].emplace_back(process, currentCycle + 1);
            } else {
                stillWaiting.push_back(process);
                
                std::cout << "Process " << process->GetPID() << " still waiting for " << resourceName << std::endl;
                
                m_events.emplace_back(
                    process->GetPID(),
                    resource->GetName(),
                    ActionType::READ,
                    true,  
                    currentCycle
                );
            }
        }
        
        for (auto& process : stillWaiting) {
            waitingQueue.push(process);
        }
    }
    
    std::cout << "Total events so far: " << m_events.size() << std::endl;
    std::cout << "=== END CYCLE " << currentCycle << " ===" << std::endl;
    
    m_clock.AdvanceCycle();
}

bool Semaphore::TryLock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource) {
    return resource->Acquire(process);
}

void Semaphore::Unlock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource) {
    resource->Release(process);
}
