/* Desenvolvido por L. A. Leandro - Sao Jose dos Campos - SP - 24/05/2026 */

#include "executor.hpp"
#include "parser.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstring>
#include <cerrno>

bool Executor::is_builtin(const std::string& command) {
    return command == "cd" || command == "exit";
}

bool Executor::execute_builtin(const std::vector<std::string>& tokens) {
    if (tokens.empty()) return true;

    const std::string& cmd = tokens[0];

    if (cmd == "exit") {
        return false;
    }

    if (cmd == "cd") {
        const char* path = nullptr;

        if (tokens.size() >= 2) {
            path = tokens[1].c_str();
        } else {
            path = getenv("HOME");
            if (path == nullptr) {
                std::cerr << "cd: HOME not set" << std::endl;
                return true;
            }
        }

        if (chdir(path) != 0) {
            std::cerr << "cd: " << path << ": " << std::strerror(errno) << std::endl;
        }

        return true;
    }

    return true;
}

int Executor::execute_external(const std::vector<std::string>& tokens) {
    if (tokens.empty()) return -1;

    char** c_args = Parser::to_c_args(tokens);

    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "fork: " << std::strerror(errno) << std::endl;
        Parser::free_c_args(c_args);
        return -1;
    }

    if (pid == 0) {
        if (execvp(c_args[0], c_args) == -1) {
            std::cerr << c_args[0] << ": " << std::strerror(errno) << std::endl;
            Parser::free_c_args(c_args);
            _exit(EXIT_FAILURE);
        }
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            std::cerr << "waitpid: " << std::strerror(errno) << std::endl;
            Parser::free_c_args(c_args);
            return -1;
        }

        Parser::free_c_args(c_args);

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }

        return -1;
    }

    Parser::free_c_args(c_args);
    return 0;
}
