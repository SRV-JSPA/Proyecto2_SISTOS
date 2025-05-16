#pragma once
#include "Synchronization/BaseSynchronization.h"
#include <queue>

class Semaphore : public BaseSynchronization {
private:
    std::map<std::string, std::queue<std::shared_ptr<Process>>> m_waitingQueues;

public:
    void Initialize() override;
    void ExecuteCycle() override;

    bool TryLock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource);
    void Unlock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource);
};
