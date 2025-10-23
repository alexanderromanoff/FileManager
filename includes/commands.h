#include <iostream>
#include <filesystem>
#include <string>
#include <optional>


#ifndef COMMANDS_H
#define COMMANDS_H
namespace fs = std::filesystem;


class ICommand{
public:
    virtual std::string execute(std::optional<std::string>& path, fs::path& curDir) = 0;
	std::string resolvePath(std::string& tPath);
	void runChecks(const std::string& tPath);
};

class pwdCommand : public ICommand{
    std::string execute(std::optional<std::string>& path, fs::path& curDir);
};

class lsCommand : public ICommand{
    std::string execute(std::optional<std::string>& path, fs::path& curDir);
};

class statCommand : public ICommand{
        std::string execute(std::optional<std::string>& path, fs::path& curDir);
};

class cdCommand : public ICommand{
        std::string execute(std::optional<std::string>& path, fs::path& curDir);
};

#endif
