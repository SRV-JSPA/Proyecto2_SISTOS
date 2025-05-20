#pragma once
#include "Scheduler/BaseScheduler.h"

class FIFOScheduler : public BaseScheduler {
public:
    std::shared_ptr<Process> GetNextProcess() override;
};