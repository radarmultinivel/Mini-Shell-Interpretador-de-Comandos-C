/* Desenvolvido por L. A. Leandro - Sao Jose dos Campos - SP - 24/05/2026 */

#include "parser.hpp"
#include "executor.hpp"

#include <iostream>
#include <string>
#include <vector>

int main() {
    std::string input;

    while (true) {
        std::cout << "mini-shell> ";

        if (!std::getline(std::cin, input)) {
            std::cout << std::endl;
            break;
        }

        std::vector<std::string> tokens = Parser::tokenize(input);

        if (tokens.empty()) {
            continue;
        }

        if (Executor::is_builtin(tokens[0])) {
            if (!Executor::execute_builtin(tokens)) {
                break;
            }
        } else {
            Executor::execute_external(tokens);
        }
    }

    return 0;
}
