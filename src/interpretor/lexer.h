#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include <string>
#include "token.h"

class Lexer {
    public:
        Lexer(const std::string& _path);
        void tokenize();
        const std::vector<Token>& getTokens() const;
        void logger(std::ostream& os = std::cout) const;
        static void regressionTest();
    private:
        std::string src_path;
        std::vector<Token> tokens;
};

#endif