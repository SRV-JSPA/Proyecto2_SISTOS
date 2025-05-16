#pragma once
#include <vector>
#include <memory>
#include <map>
#include "Core/Process.h"
#include "Core/Resource.h"
#include "Core/Action.h"
#include "Core/SimulationClock.h"

struct SyncEvent {
    std::string pid;
    std::string resourceName;
    ActionType actionType;
    bool isWaiting;
    int cycle;

    SyncEvent(const std::string& pid, const std::string& resourceName, 
              ActionType actionType, bool isWaiting, int cycle)
        : pid(pid), resourceName(resourceName), actionType(actionType), 
          isWaiting(isWaiting), cycle(cycle) {}
};

class BaseSynchronization {
protected:
    std::vector<std::shared_ptr<Process>> m_processes;
    std::map<std::string, std::shared_ptr<Resource>> m_resources;
    std::vector<std::shared_ptr<Action>> m_actions;
    SimulationClock m_clock;
    std::vector<SyncEvent> m_events;

public:
    virtual ~BaseSynchronization() = default;

    void LoadProcesses(const std::vector<std::shared_ptr<Process>>& processes);
    void LoadResources(const std::map<std::string, std::shared_ptr<Resource>>& resources);
    void LoadActions(const std::vector<std::shared_ptr<Action>>& actions);

    virtual void Initialize();
    virtual void ExecuteCycle() = 0;
    bool IsComplete() const;

    std::vector<SyncEvent> GetEvents() const;
    int GetCurrentCycle() const;
};