/* Desenvolvido por L. A. Leandro - Sao Jose dos Campos - SP - 24/05/2026 */

#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

class Parser {
public:
    static std::vector<std::string> tokenize(const std::string& input);
    static char** to_c_args(const std::vector<std::string>& tokens);
    static void free_c_args(char** args);
};

#endif
