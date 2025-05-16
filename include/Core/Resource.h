#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Core/Process.h"

class Resource {
private:
    std::string m_name;
    int m_counter;
    std::vector<std::shared_ptr<Process>> m_usingProcesses;

public:
    Resource(const std::string& name, int counter);

    std::string GetName() const;
    int GetCounter() const;
    bool IsAvailable() const;

    bool Acquire(std::shared_ptr<Process> process);
    void Release(std::shared_ptr<Process> process);
};