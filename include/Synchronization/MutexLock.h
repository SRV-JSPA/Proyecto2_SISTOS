#pragma once
#include "Synchronization/BaseSynchronization.h"
#include <map>

struct ActiveOperation {
    std::string resourceName;
    int startCycle;
    int endCycle;
};

class MutexLock : public BaseSynchronization {
private:
    std::map<std::string, ActiveOperation> m_activeOperations; // PID -> ActiveOperation

public:
    void ExecuteCycle() override;
    bool TryLock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource);
    void Unlock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource);
};