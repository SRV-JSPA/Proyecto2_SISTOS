#pragma once
#include "Scheduler/BaseScheduler.h"

class PriorityScheduler : public BaseScheduler {
public:
    std::shared_ptr<Process> GetNextProcess() override;
    double CalculateAlgorithmSpecificWT() const override;
};