#ifndef TOKEN_H
#define TOKEN_H

#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <variant>
#include "keyword_map.h"

/// @brief Variant used to store any keyword enum type.
using KeywordType = std::variant<
    std::monostate,
    BlockType,
    ObjectType,
    TextureType,
    MaterialType,
    LightType,
    IdentType
>;

/// @brief All possible token types produced by the lexer.
enum class TokenType {
    NONE,
    KEYWORD,
    STRING,
    IDENTIFIER,
    NUMBER,
    BOOLEAN,
    LBRACE,
    RBRACE,
    END
};

/// @brief Represents one token with its type, raw value, keyword value and line number.
struct Token {
    TokenType type; ///< Token category.
    KeywordType key; ///< Keyword value if the token is a keyword.
    std::string raw; ///< Original text value.
    int line = 0; ///< Line number in the source file.

    /// @brief Creates a token with given type, raw text, line and optional keyword value.
    Token(TokenType _ttype = TokenType::NONE, const std::string& _raw = "", 
                    int _line = 0, KeywordType _keyword = std::monostate{}) :
                    
                    type(_ttype), key(_keyword), raw(_raw), line(_line) {}

    /// @brief Checks if this token is a keyword of the expected enum value.
    template<typename T>
    bool isKeyword(T expected) const {
        if (type != TokenType::KEYWORD) return false;
        const T* v = std::get_if<T>(&key);
        return v && *v == expected; 
    }

    /// @brief Gets the stored keyword value with the expected enum type.
    template<typename T>
    T getKeyword() const {
        const T* v = std::get_if<T>(&key);
        if (!v){
            std::cout << "DEBUG: token = " << raw << "\n";
            std::cout << "Stored type = " << key.index() << "\n";
            throw std::runtime_error("LINE : " + std::to_string(line) + " -- Token '" + raw + "' is not the expected keyword type");
        }
        return *v;
    }

    /// @brief Checks if token has the given TokenType.
    bool is(TokenType t) const { return type == t; }

    /// @brief Checks if token raw text matches a word.
    bool is(const std::string& word) const { return raw == word; }

    /// @brief Checks if token is a number.
    bool isNumber() const {
        return type == TokenType::NUMBER;
    }

    /// @brief Converts token raw text to double.
    double getNumber() const {
        if (type != TokenType::NUMBER)
            throw std::runtime_error("LINE: " + std::to_string(line) + " -- Expected number, got '" + raw + "'");
        return std::stod(raw);
    }

    /// @brief Checks if token is a boolean.
    bool isBool() const {
        return type == TokenType::BOOLEAN;
    }

    /// @brief Converts token raw text to bool.
    bool getBool() const {
        if (type != TokenType::BOOLEAN)
            throw std::runtime_error("LINE : " + std::to_string(line) + " --Expected boolean, got '" + raw + "'");
        return raw == "true";
    }

    /// @brief Checks if token is a string.
    bool isString() const {
        return type == TokenType::STRING;
    }

    /// @brief Returns the string value.
    const std::string& getString() const {
        if (type != TokenType::STRING)
            throw std::runtime_error("LINE : " + std::to_string(line) + " --Expected string, got '" + raw + "'");
        return raw;
    }

    /// @brief Checks if token is an opening brace.
    bool isLBrace() const {
        return type == TokenType::LBRACE;
    }

    /// @brief Checks if token is a closing brace.
    bool isRBrace() const {
        return type == TokenType::RBRACE;
    }

};

/// @brief Creates a string token.
inline Token makeStringToken(const std::string& _word, int _line) {
    return Token(TokenType::STRING, _word, _line);
}

/// @brief Creates a number token.
inline Token makeNumberToken(const std::string& _word, int _line) {
    return Token(TokenType::NUMBER, _word, _line);
}

/// @brief Creates a boolean token.
inline Token makeBooleanToken(const std::string& _word, int _line) {
    return Token(TokenType::BOOLEAN, _word, _line);
}

/// @brief Creates an opening brace token.
inline Token makeLBraceToken(int _line) {
    return Token(TokenType::LBRACE, "{", _line);
}

/// @brief Creates a closing brace token.
inline Token makeRBraceToken(int _line) {
    return Token(TokenType::RBRACE, "}", _line);
}

/// @brief Creates the end token.
inline Token makeEndToken(int _line) {
    return Token(TokenType::END, "", _line);
}

/// @brief Tries to convert a word into a known keyword token.
///        If not recognized, it becomes an identifier.
inline Token makeKeywordToken(const std::string& _word, int _line) {
    Token tk;
    tk.type = TokenType::KEYWORD;
    tk.line = _line;
    tk.raw = _word;

    BlockType blk = convertToBlockType(_word);
    if (blk != BlockType::UNKNOWN) { tk.key = blk; return tk; }

    ObjectType ob = convertToObjectType(_word);
    if (ob != ObjectType::UNKNOWN) { tk.key = ob; return tk; }

    TextureType tx = convertToTextureType(_word);
    if (tx != TextureType::UNKNOWN) { tk.key = tx; return tk; }

    MaterialType mat = convertToMaterialType(_word);
    if (mat != MaterialType::UNKNOWN) { tk.key = mat; return tk; }

    LightType l = convertToLightType(_word);
    if (l != LightType::UNKNOWN) { tk.key = l;  return tk; }

    IdentType id = convertToIdentType(_word);
    if (id != IdentType::UNKNOWN) { tk.key = id; return tk; }

    // Truly unrecognized unquoted word — user-defined name
    return Token(TokenType::IDENTIFIER, _word, _line);
}

/// @brief Creates the correct token type from a raw word.
inline Token makeToken(const std::string& word, int line) {
    if (word.size() >= 2 && word.front() == '"' && word.back() == '"')
        return makeStringToken(word.substr(1, word.size() - 2), line);

    if (word == "{")
        return makeLBraceToken(line);

    if (word == "}")
        return makeRBraceToken(line);

    if (word == "true" || word == "false")
        return makeBooleanToken(word, line);

   bool isNumber = !word.empty() &&
                (std::isdigit(word[0]) ||
                ((word[0] == '-' || word[0] == '+') && word.size() > 1));

    if (isNumber) {
        char* end = nullptr;
        std::strtod(word.c_str(), &end);

        if (*end == '\0') {
            return makeNumberToken(word, line);
        }
    }
    return makeKeywordToken(word, line);
}

/// @brief Converts TokenType to string for logs.
inline std::string logString(TokenType t) {
    switch (t) {
        case TokenType::NONE:       return "NONE";
        case TokenType::KEYWORD:    return "KEYWORD";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING:     return "STRING";
        case TokenType::NUMBER:     return "NUMBER";
        case TokenType::BOOLEAN:    return "BOOLEAN";
        case TokenType::LBRACE:     return "LBRACE";
        case TokenType::RBRACE:     return "RBRACE";
        case TokenType::END:        return "END";
        default:                    return "???";
    }
}

/// @brief Converts stored keyword variant to string for logs.
inline std::string logString(const KeywordType& k) {
    return std::visit([](auto&& v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::monostate>) return "-";
        else return logString(v);  // delegates to keyword_map.h logString overloads
    }, k);
}

/// @brief Prints a token in a readable debug format.
inline std::ostream& operator<<(std::ostream& os, const Token& t) {
    os << "[line " << std::setw(3) << t.line << "] "
       << std::setw(12) << std::left << logString(t.type)
       << " raw: " << std::setw(20) << std::left << ("'" + t.raw + "'");

    if (t.type == TokenType::KEYWORD)
        os << " key: " << logString(t.key);

    return os;
}

#endif