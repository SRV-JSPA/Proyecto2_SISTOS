#include "Utils/FileParser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<std::shared_ptr<Process>> FileParser::ParseProcessesFile(const std::string& filePath) {
    std::vector<std::shared_ptr<Process>> processes;
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string pidStr, btStr, atStr, priorityStr;
        
        std::getline(ss, pidStr, ',');
        std::getline(ss, btStr, ',');
        std::getline(ss, atStr, ',');
        std::getline(ss, priorityStr, ',');
        
        pidStr.erase(0, pidStr.find_first_not_of(" \t"));
        pidStr.erase(pidStr.find_last_not_of(" \t") + 1);
        btStr.erase(0, btStr.find_first_not_of(" \t"));
        btStr.erase(btStr.find_last_not_of(" \t") + 1);
        atStr.erase(0, atStr.find_first_not_of(" \t"));
        atStr.erase(atStr.find_last_not_of(" \t") + 1);
        priorityStr.erase(0, priorityStr.find_first_not_of(" \t"));
        priorityStr.erase(priorityStr.find_last_not_of(" \t") + 1);
        
        try {
            int bt = std::stoi(btStr);
            int at = std::stoi(atStr);
            int priority = std::stoi(priorityStr);
            
            processes.push_back(std::make_shared<Process>(pidStr, bt, at, priority));
        } catch (const std::exception& e) {
        }
    }
    
    file.close();
    return processes;
}

std::map<std::string, std::shared_ptr<Resource>> FileParser::ParseResourcesFile(const std::string& filePath) {
    std::map<std::string, std::shared_ptr<Resource>> resources;
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string nameStr, counterStr;
        
        std::getline(ss, nameStr, ',');
        std::getline(ss, counterStr, ',');
        
        nameStr.erase(0, nameStr.find_first_not_of(" \t"));
        nameStr.erase(nameStr.find_last_not_of(" \t") + 1);
        counterStr.erase(0, counterStr.find_first_not_of(" \t"));
        counterStr.erase(counterStr.find_last_not_of(" \t") + 1);
        
        try {
            int counter = std::stoi(counterStr);
            resources[nameStr] = std::make_shared<Resource>(nameStr, counter);
        } catch (const std::exception& e) {
        }
    }
    
    file.close();
    return resources;
}

std::vector<std::shared_ptr<Action>> FileParser::ParseActionsFile(const std::string& filePath) {
    std::vector<std::shared_ptr<Action>> actions;
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string pidStr, actionStr, resourceStr, cycleStr;
        
        std::getline(ss, pidStr, ',');
        std::getline(ss, actionStr, ',');
        std::getline(ss, resourceStr, ',');
        std::getline(ss, cycleStr, ',');
        
        pidStr.erase(0, pidStr.find_first_not_of(" \t"));
        pidStr.erase(pidStr.find_last_not_of(" \t") + 1);
        actionStr.erase(0, actionStr.find_first_not_of(" \t"));
        actionStr.erase(actionStr.find_last_not_of(" \t") + 1);
        resourceStr.erase(0, resourceStr.find_first_not_of(" \t"));
        resourceStr.erase(resourceStr.find_last_not_of(" \t") + 1);
        cycleStr.erase(0, cycleStr.find_first_not_of(" \t"));
        cycleStr.erase(cycleStr.find_last_not_of(" \t") + 1);
        
        try {
            int cycle = std::stoi(cycleStr);
            ActionType actionType = (actionStr == "READ") ? ActionType::READ : ActionType::WRITE;
            
            actions.push_back(std::make_shared<Action>(pidStr, actionType, resourceStr, cycle));
        } catch (const std::exception& e) {
        }
    }
    
    file.close();
    return actions;
}