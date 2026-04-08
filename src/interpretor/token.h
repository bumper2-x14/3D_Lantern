#ifndef TOKEN_H
#define TOKEN_H

#include <stdexcept>
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
        if (!v)
            throw std::runtime_error("Token '" + raw + "' is not the expected keyword type");
        return *v;
    }

    bool is(TokenType t) const { return type == t; }
    bool is(const std::string& word) const { return raw == word; }

    bool isNumber() const {
        return type == TokenType::NUMBER;
    }

    double getNumber() const {
        if (type != TokenType::NUMBER)
            throw std::runtime_error("Expected number, got '" + raw + "'");
        return std::stod(raw);
    }

    bool isBool() const {
        return type == TokenType::BOOLEAN;
    }

    bool getBool() const {
        if (type != TokenType::BOOLEAN)
            throw std::runtime_error("Expected boolean, got '" + raw + "'");
        return raw == "true";
    }

    bool isString() const {
        return type == TokenType::STRING;
    }

    const std::string& getString() const {
        if (type != TokenType::STRING)
            throw std::runtime_error("Expected string, got '" + raw + "'");
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

    tk.key = convertToIdentType(_word);
    return tk;
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
                    (std::isdigit(word[0]) || word[0] == '-' || word[0] == '+');

    if (isNumber) {
        char* end = nullptr;
        std::strtod(word.c_str(), &end);

        if (*end == '\0') {
            return makeNumberToken(word, line);
        }
    }

    return makeKeywordToken(word, line);
}

#endif
