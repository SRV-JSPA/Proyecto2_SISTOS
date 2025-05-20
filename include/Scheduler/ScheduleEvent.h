#pragma once
#include <string>

struct ScheduleEvent {
    std::string pid;
    int startCycle;
    int duration;
    
    ScheduleEvent(const std::string& pid, int startCycle, int duration)
        : pid(pid), startCycle(startCycle), duration(duration) {}
};