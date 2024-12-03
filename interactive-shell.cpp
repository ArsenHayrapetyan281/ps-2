#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

std::vector<std::string> splitCommand(const std::string& command) {
    std::vector<std::string> args;
    std::istringstream ss(command);
    std::string arg;
    while (ss >> arg) {
        args.push_back(arg);
    }
    return args;
}

void executeCommand(const std::vector<std::string>& args) {
    pid_t pid = fork();
    if (pid == 0) {
        if (args[0] == "exit") {
            exit(0);
        }
        std::string command = args[0];
        char* const env[] = {
            const_cast<char*>("PATH=.:/usr/bin:/bin"),
            nullptr
        };
        if (args[0] == "silent") {
            std::string logFile = std::to_string(getpid()) + ".log";
            int fd = open(logFile.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd != -1) {
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                close(fd);
                execvp(args[1].c_str(), (char* const*)&args[1]);
            }
        } else {
            execvp(command.c_str(), (char* const*)&args[0]);
        }
        std::cerr << "Command not found: " << args[0] << std::endl;
        exit(1);
    } else {
        waitpid(pid, nullptr, 0);
    }
}

int main() {
    std::string command;
    while (true) {
        std::cout << "shell> ";
        std::getline(std::cin, command);
        if (command.empty()) {
            continue;
        }
        std::vector<std::string> args = splitCommand(command);
        if (args[0] == "exit") {
            break;
        }
        executeCommand(args);
    }
    return 0;
}

