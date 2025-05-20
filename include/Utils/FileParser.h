#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Core/Process.h"
#include "Core/Resource.h"
#include "Core/Action.h"

class FileParser {
public:
    static std::vector<std::shared_ptr<Process>> ParseProcessesFile(const std::string& filePath);
    static std::map<std::string, std::shared_ptr<Resource>> ParseResourcesFile(const std::string& filePath);
    static std::vector<std::shared_ptr<Action>> ParseActionsFile(const std::string& filePath);
};