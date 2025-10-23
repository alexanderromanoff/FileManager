#include <iostream>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <functional>
#include <optional>
#include <sstream>
#include <memory>
#include "commands.h"

#define BOLD_S "\e[1m"
#define BOLD_F "\e[0m"

#ifndef CONTROLLER_H
#define CONTROLLER_H

namespace fs = std::filesystem;

class Controller{
 
private:
    fs::path currentDir;
    std::unordered_map<std::string, std::unique_ptr<ICommand>> commands;
    
    std::string command;
    std::string argument;

    std::vector<std::string> split(const std::string& str);
    void clean(std::string& str);

public:
    Controller();
    void read();
    bool parse(std::string prompt);
    bool execute();
    void print(std::string text);
};

#endif