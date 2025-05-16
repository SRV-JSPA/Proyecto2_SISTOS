#pragma once
#include <string>

enum class ActionType {
    READ,
    WRITE
};

class Action {
private:
    std::string m_pid;
    ActionType m_type;
    std::string m_resourceName;
    int m_cycle;
    bool m_executed;
    bool m_successful;

public:
    Action(const std::string& pid, ActionType type, const std::string& resourceName, int cycle);

    std::string GetPID() const;
    ActionType GetType() const;
    std::string GetResourceName() const;
    int GetCycle() const;
    bool IsExecuted() const;
    bool WasSuccessful() const;

    void Execute(bool success);
};