#pragma once
#include "Scheduler/BaseScheduler.h"

class SRTScheduler : public BaseScheduler {
public:
    std::shared_ptr<Process> GetNextProcess() override;
    double CalculateAlgorithmSpecificWT() const override;
};