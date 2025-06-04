#pragma once
#include "Synchronization/BaseSynchronization.h"
#include <queue>
#include <map>

struct ResourceUsage {
    std::shared_ptr<Process> process;
    int endCycle;  
    
    ResourceUsage(std::shared_ptr<Process> p, int end) 
        : process(p), endCycle(end) {}
};

class Semaphore : public BaseSynchronization {
private:
    std::map<std::string, std::queue<std::shared_ptr<Process>>> m_waitingQueues;
    std::map<std::string, std::vector<ResourceUsage>> m_activeUsages;  

public:
    void Initialize() override;
    void ExecuteCycle() override;

    bool TryLock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource);
    void Unlock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource);
};
