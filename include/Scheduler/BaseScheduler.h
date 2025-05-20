#pragma once
#include <vector>
#include <memory>
#include "Core/Process.h"
#include "Scheduler/ScheduleEvent.h"

class BaseScheduler {
protected:
    std::vector<std::shared_ptr<Process>> m_processes;
    std::shared_ptr<Process> m_currentProcess;
    int m_currentCycle;
    std::vector<ScheduleEvent> m_events;
    int m_lastProcessStartCycle;
    
public:
    BaseScheduler();
    virtual ~BaseScheduler() = default;
    
    void LoadProcesses(const std::vector<std::shared_ptr<Process>>& processes);
    virtual void Initialize();
    virtual std::shared_ptr<Process> GetNextProcess() = 0;
    virtual void ExecuteCycle();
    bool IsComplete() const;
    
    std::vector<ScheduleEvent> GetEvents() const;
    double CalculateAverageWaitingTime() const;
    int GetCurrentCycle() const;
};