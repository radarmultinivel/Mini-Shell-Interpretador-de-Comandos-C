/* Desenvolvido por L. A. Leandro - Sao Jose dos Campos - SP - 24/05/2026 */

#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <string>
#include <vector>

class Executor {
public:
    static bool is_builtin(const std::string& command);
    static bool execute_builtin(const std::vector<std::string>& tokens);
    static int execute_external(const std::vector<std::string>& tokens);
};

#endif
