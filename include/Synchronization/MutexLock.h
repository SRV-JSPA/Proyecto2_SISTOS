#pragma once
#include "Synchronization/BaseSynchronization.h"

class MutexLock : public BaseSynchronization {
public:
    void ExecuteCycle() override;

    bool TryLock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource);
    void Unlock(std::shared_ptr<Process> process, std::shared_ptr<Resource> resource);
};