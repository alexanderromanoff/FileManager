#include "../includes/controller.h"



// COMMANDS
std::string ICommand::resolvePath(std::string& tPath){
    if(tPath == ".") return fs::current_path().string();
    if(tPath == "..") return fs::current_path().parent_path().string();
	return tPath[0] == '~' && tPath.size() >=1 ? std::getenv("HOME") + tPath.substr(1) : tPath;
}

void ICommand::runChecks(const std::string& tPath){
	if(!fs::exists(tPath)) throw std::invalid_argument("warn: no such directory: " + tPath);
    if(!fs::is_directory(tPath)) throw std::invalid_argument("warn: not a directory: " + tPath);
}

std::string pwdCommand::execute(std::optional<std::string>& path, fs::path& curDir){
    if(path.has_value()) throw std::invalid_argument("warn: pwd doesn't require path");
    return curDir.string();
}

std::string lsCommand::execute(std::optional<std::string>& path, fs::path& curDir){
    std::string res;
    std::string tPath = path.value_or(curDir.string());
	const std::string targetPath = resolvePath(tPath);

	runChecks(targetPath);    

    for (const auto & entry : fs::directory_iterator(targetPath)){
        if(entry.path().filename().string()[0] == '.') continue;    // ignore hidden files
        if(!res.empty()) res += "\n";
        res += entry.path().filename().string();
        if(entry.is_directory()) res += "/";
    }
    return res;
}

std::string statCommand::execute(std::optional<std::string>& path, fs::path& curDir){
    int fCount = 0, dCount = 0;
    std::string tPath = path.value_or(curDir.string());
	const std::string targetPath = resolvePath(tPath);

    runChecks(targetPath);

    for (const auto & entry : fs::directory_iterator(targetPath)){
        if(entry.path().string()[0] == '.') continue;
        if(entry.is_directory()) dCount++;
        else fCount++;
    }
    std::string res =   "Files: " + std::to_string(fCount) + 
                        "\nDirectories: " + std::to_string(dCount) + 
                        "\nTotal: " + std::to_string(fCount + dCount);
    return res;
}

std::string cdCommand::execute(std::optional<std::string>& path, fs::path& curDir){
	std::string tPath = path.value_or(std::getenv("HOME"));
	const std::string targetPath = resolvePath(tPath);

    if(fs::exists(targetPath)){
        if(fs::is_directory(targetPath)) {
                fs::path temp = fs::canonical(targetPath);
                fs::current_path(targetPath);   // !! this may throw an exception, 
                curDir = temp;   				// so we don't immediately assign new currentDir !!
        }
        else throw std::invalid_argument("warn: not a directory: " + targetPath);
    }
    else throw std::invalid_argument("warn: no such directory: " + targetPath);

    return "";
}



// CONTROLLER
std::vector<std::string> Controller::split(const std::string& str){
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream stream(str);

    while(std::getline(stream, token, ' ')){
        if(token == " ") continue;
        tokens.push_back(token);
    }

    return tokens;
}

void Controller::clean(std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos) {
        str.clear();
        return;
    }
    
    size_t end = str.find_last_not_of(" \t");
    str = str.substr(start, end - start + 1);

    for(auto& ch : str){
        if(ch == '\t') ch=' ';
    }
}

Controller::Controller(){
    currentDir = fs::current_path();
    commands["pwd"] = std::make_unique<pwdCommand>();
    commands["ls"] = std::make_unique<lsCommand>();
    commands["stat"] = std::make_unique<statCommand>();
    commands["cd"] = std::make_unique<cdCommand>();
}

void Controller::read() {
    bool run = true;
    std::string prompt;
    while(run) {
        std::cout << BOLD_S << currentDir.string() << BOLD_F << " > ";
        std::getline(std::cin, prompt);
        if (parse(prompt)) run = execute();
    }
}

bool Controller::parse(std::string prompt) {
    command.clear();
    argument.clear();
    
    clean(prompt);    
    if (prompt.empty()) return false;
    
    std::vector<std::string> tokens = split(prompt); 
    if (tokens.empty()) return false;
    
    command = tokens[0]; 
    for (size_t i = 1; i < tokens.size(); i++) {
        if (tokens[i].empty()) continue;
        else if (argument.empty()) argument = tokens[i];
    }

    return true;
}

bool Controller::execute(){
    std::optional<std::string> pathArg = argument.empty() ? std::nullopt : std::optional<std::string>(argument);
    std::string result;
    if(command == "exit") return false;
    try{
        result = commands.at(command)->execute(pathArg, currentDir);
    } 
    catch(const std::out_of_range& ex) {
        result = "warn: unknown command";
    }
    catch(const std::invalid_argument& ex) {
        result = ex.what();
    }
    catch(const fs::filesystem_error& ex) {
        if(ex.code() == std::errc::permission_denied) result = "warn: not enough permissions";
        else result = "warn: unknown filesystem error";
    }
    catch(const std::exception& ex) {
        result = "warn: unknown error";
    }
    if(!result.empty()) print(result);
    return true;
}

void Controller::print(std::string text){
    std::cout << text << "\n";
}
