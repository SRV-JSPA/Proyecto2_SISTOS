#pragma once
#include "Scheduler/BaseScheduler.h"
#include <queue>
#include <iostream>

class RoundRobinScheduler : public BaseScheduler {
private:
    int m_quantum;
    int m_timeInQuantum;
    std::queue<std::shared_ptr<Process>> m_readyQueue;
    bool m_justPreempted;  
    
public:
    RoundRobinScheduler(int quantum = 1);
    
    void Initialize() override;
    std::shared_ptr<Process> GetNextProcess() override;
    void ExecuteCycle() override;
    bool IsComplete() const override; 
    
    void SetQuantum(int quantum);
    int GetQuantum() const;
    double CalculateAlgorithmSpecificWT() const override;
};