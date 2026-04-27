#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <vector>
#include <string>
#include "token.h"

/// @brief Reads a scene file or string and converts it into a list of tokens.
class Lexer {
    public:
        /// @brief Default constructor.
        Lexer() = default;

        /// @brief Creates a lexer from a file path.
        /// @param _path Path to the source file.
        Lexer(const std::string& _path);

        /// @brief Tokenizes the source file.
        void tokenize();

        /// @brief Returns the generated tokens.
        const std::vector<Token>& getTokens() const;

        /// @brief Tokenizes directly from a string.
        /// @param source Source text to tokenize.
        void tokenizeFromString(const std::string& source);

        /// @brief Prints tokens for debugging.
        void logger(std::ostream& os = std::cout) const;

        /// @brief Runs simple tests for the lexer.
        static void regressionTest();

    private:
        std::string src_path; ///< Path of the source file.
        std::vector<Token> tokens; ///< List of generated tokens.
};

#endif