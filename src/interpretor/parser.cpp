#include <iostream>
#include "parser.h"

Parser::Parser(const std::vector<Token>& _tokens): tokens(_tokens){}

const Token& Parser::current() const {
    return tokens[cursor];
}

const Token& Parser::peek(int offset) const {
    return tokens[cursor + offset];
}

/* DEV NOTE:
    tokens[cursor++] gets the current token pointed by cursor the increments it
    use preinc ++cursor instead
*/
const Token& Parser::next() {
    return tokens[cursor++];
}

// -------------------------------------------------------
// Block parsers — each consumes one complete { } block
// -------------------------------------------------------

/*
DEV NOTE:
    A naive approach given that tokens are designed to hold enum identifiers
    to avoid string comparisons and facilitate debugging.
    
    This block parser compares against raw strings directly, which defeats
    the purpose of the KeywordType variant system in Token. The correct
    approach is to switch on IdentType enum values via getKeyword<IdentType>(), 
    which:
    
      - Eliminates string comparison overhead
      - Lets the compiler warn on unhandled enum cases (-Wswitch)
      - Keeps dispatch consistent with how other block parsers will work
        (ObjectType, TextureType, MaterialType, etc.)
    
    TODO: Replace if/else chain with:
    
        IdentType field = current().getKeyword<IdentType>();
        next();
        switch (field) {
            case IdentType::WIDTH:        ...
            case IdentType::HEIGHT:       ...
            ...
        }

    I'll give the fixed code for parseSettingBlock, other block parser funcs follow 
    the same pattern.
*/


void Parser::parseSettingBlock(SceneDescriptor& scene) {
    expectLBrace();
    while(!current().isRBrace()) {
        if (current().type != TokenType::KEYWORD) {
            std::cout << "Error: expected keyword in settings, got '"
                      << current().raw << "' at line " << current().line << "\n";
            exit(1);
        }

        IdentType field = current().getKeyword<IdentType>();
        next();

        switch(field) {
            case IdentType::WIDTH: scene.settings.setWidth((int)parseNumber()); break;
            case IdentType::HEIGHT: scene.settings.setHeight((int)parseNumber()); break;
            case IdentType::SAMPLES: scene.settings.setSamples((int)parseNumber()); break;
            case IdentType::DEPTH: scene.settings.setDepth((int)parseNumber()); break;
            case IdentType::ASPECT_RATIO: scene.settings.setAspectRatio(parseNumber()); break;
            case IdentType::BACKGROUND: scene.settings.setBackground(parseVec3()); break;
            default:
                std::cout << "Error: unknown setting field '"
                          << current().raw << "' at line " << current().line << "\n";
                exit(1);
        }
    }
    expectRBrace();
}

// -------------------------------------------------------
// Helper parsers — consume one or more tokens for a value
// -------------------------------------------------------

/*
DEV NOTE:
    Redundant code Token::getNumber() already handles type checking and error handeling
*/
double Parser::parseNumber() {
    if (!current().isNumber()) {
        std::cout << "Error: Expected number, got '" << current().raw 
                  << "' at line " << current().line << std::endl;
        exit(1);
    }

    double value = current().getNumber();
    next();
    return value;
}

/*
DEV NOTE:
    Redundant code Token::getBool() already handles type checking and error handeling
*/
bool Parser::parseBool() {
    if (!current().isBool()) {
        std::cout << "Error: Expected boolean, got '" << current().raw << "'" << std::endl;
        exit(1);
    }

    bool value = current().getBool();
    next();
    return value;
}

/*
DEV NOTE:
    Redundant code Token::getString() already handles type checking and error handeling
*/
std::string Parser::parseString() {
    if (!current().isString()) {
        std::cout << "Error: Expected string, got '" << current().raw << "'" << std::endl;
        exit(1);
    }

    std::string value = current().getString();
    next();
    return value;
}

Vec3d Parser::parseVec3() {
    double x = parseNumber();
    double y = parseNumber();
    double z = parseNumber();

    return Vec3d(x, y, z);
}

Point3d Parser::parsePoint3() {
    double x = parseNumber();
    double y = parseNumber();
    double z = parseNumber();

    return Point3d(x, y, z);
}

std::string Parser::parseIdentifier() {
    if (!current().is(TokenType::IDENTIFIER)) {
        std::cout << "Error: Expected identifier, got '" << current().raw << "'" << std::endl;
        exit(1);
    }

    std::string value = current().raw;
    next();
    return value;
}

void Parser::expectLBrace() {
    if (!current().isLBrace()) {
        std::cout << "Error: Expected '{'" << std::endl;
        exit(1);
    }
    next();
}

void Parser::expectRBrace() {
    if (!current().isRBrace()) {
        std::cout << "Error: Expected '}'" << std::endl;
        exit(1);
    }
    next();
}




