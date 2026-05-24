/* Desenvolvido por L. A. Leandro - Sao Jose dos Campos - SP - 24/05/2026 */

#include "parser.hpp"
#include <sstream>
#include <cstring>

std::vector<std::string> Parser::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;

    while (stream >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

char** Parser::to_c_args(const std::vector<std::string>& tokens) {
    char** args = new char*[tokens.size() + 1];

    for (size_t i = 0; i < tokens.size(); ++i) {
        args[i] = new char[tokens[i].size() + 1];
        std::strcpy(args[i], tokens[i].c_str());
    }

    args[tokens.size()] = nullptr;

    return args;
}

void Parser::free_c_args(char** args) {
    if (args == nullptr) return;

    for (size_t i = 0; args[i] != nullptr; ++i) {
        delete[] args[i];
    }

    delete[] args;
}
