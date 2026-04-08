#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "token.h"

class Lexer {
    public:
        Lexer(const std::string& path);
        void tokenize();
        const std::vector<Token>& getTokens() const;
    private:
        std::string src_path;
        std::vector<Token> tokens;
};

#endif