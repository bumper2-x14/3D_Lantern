#ifndef TOKEN_H
#define TOKEN_H

#include <stdexcept>
#include <iomanip>
#include <variant>
#include "keyword_map.h"

using KeywordType = std::variant<
    std::monostate,
    BlockType,
    ObjectType,
    TextureType,
    MaterialType,
    LightType,
    IdentType
>;

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


struct Token {
    TokenType type;
    KeywordType key;
    std::string raw;
    int line = 0;

    Token(TokenType _ttype = TokenType::NONE, const std::string& _raw = "", 
                    int _line = 0, KeywordType _keyword = std::monostate{}) :
                    
                    type(_ttype), key(_keyword), raw(_raw), line(_line) {}


    template<typename T>
    bool isKeyword(T expected) const {
        if (type != TokenType::KEYWORD) return false;
        const T* v = std::get_if<T>(&key);
        return v && *v == expected; 
    }

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

    bool is(TokenType t) const { return type == t; }
    bool is(const std::string& word) const { return raw == word; }

    bool isNumber() const {
        return type == TokenType::NUMBER;
    }

    double getNumber() const {
        if (type != TokenType::NUMBER)
            throw std::runtime_error("LINE: " + std::to_string(line) + " -- Expected number, got '" + raw + "'");
        return std::stod(raw);
    }

    bool isBool() const {
        return type == TokenType::BOOLEAN;
    }

    bool getBool() const {
        if (type != TokenType::BOOLEAN)
            throw std::runtime_error("LINE : " + std::to_string(line) + " --Expected boolean, got '" + raw + "'");
        return raw == "true";
    }

    bool isString() const {
        return type == TokenType::STRING;
    }

    const std::string& getString() const {
        if (type != TokenType::STRING)
            throw std::runtime_error("LINE : " + std::to_string(line) + " --Expected string, got '" + raw + "'");
        return raw;
    }

    bool isLBrace() const {
        return type == TokenType::LBRACE;
    }

    bool isRBrace() const {
        return type == TokenType::RBRACE;
    }

};

inline Token makeStringToken(const std::string& _word, int _line) {
    return Token(TokenType::STRING, _word, _line);
}

inline Token makeNumberToken(const std::string& _word, int _line) {
    return Token(TokenType::NUMBER, _word, _line);
}

inline Token makeBooleanToken(const std::string& _word, int _line) {
    return Token(TokenType::BOOLEAN, _word, _line);
}

inline Token makeLBraceToken(int _line) {
    return Token(TokenType::LBRACE, "{", _line);
}

inline Token makeRBraceToken(int _line) {
    return Token(TokenType::RBRACE, "}", _line);
}

inline Token makeEndToken(int _line) {
    return Token(TokenType::END, "", _line);
}

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

inline std::string logString(const KeywordType& k) {
    return std::visit([](auto&& v) -> std::string {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, std::monostate>) return "-";
        else return logString(v);  // delegates to keyword_map.h logString overloads
    }, k);
}

inline std::ostream& operator<<(std::ostream& os, const Token& t) {
    os << "[line " << std::setw(3) << t.line << "] "
       << std::setw(12) << std::left << logString(t.type)
       << " raw: " << std::setw(20) << std::left << ("'" + t.raw + "'");

    if (t.type == TokenType::KEYWORD)
        os << " key: " << logString(t.key);

    return os;
}

#endif
