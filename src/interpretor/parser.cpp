#include <iostream>
#include "parser.h"

Parser::Parser(const std::vector<Token>& _tokens): tokens(_tokens){}

const Token& Parser::current() const {
    return tokens[cursor];
}

const Token& Parser::peek(int offset) const {
    return tokens[cursor + offset];
}

const Token& Parser::next() {
    return tokens[cursor++];
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

// -------------------------------------------------------
// Block parsers — each consumes one complete { } block
// -------------------------------------------------------

void Parser::parseSettingBlock(SceneDescriptor& scene) {
    expectLBrace();

    while (!current().isRBrace()) {
        if (current().raw == "width") {
            next();
            scene.settings.setWidth((int) parseNumber());
        }
        else if (current().raw == "height") {
            next();
            scene.settings.setHeight((int) parseNumber());
        }
        else if (current().raw == "samples") {
            next();
            scene.settings.setSamples((int) parseNumber());
        }
        else if (current().raw == "depth") {
            next();
            scene.settings.setDepth((int) parseNumber());
        }
        else if (current().raw == "aspectRatio") {
            next();
            scene.settings.setAspectRatio(parseNumber());
        }
        else if (current().raw == "background") {
            next();
            scene.settings.setBackground(parseVec3());
        }
        else {
            std::cout << "Error: unknown setting field '" 
                      << current().raw 
                      << "' at line " << current().line << std::endl;
            exit(1);
        }
    }

    expectRBrace();
}

// -------------------------------------------------------
// Helper parsers — consume one or more tokens for a value
// -------------------------------------------------------

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

bool Parser::parseBool() {
    if (!current().isBool()) {
        std::cout << "Error: Expected boolean, got '" << current().raw << "'" << std::endl;
        exit(1);
    }

    bool value = current().getBool();
    next();
    return value;
}

std::string Parser::parseString() {
    if (!current().isString()) {
        std::cout << "Error: Expected string, got '" << current().raw << "'" << std::endl;
        exit(1);
    }

    std::string value = current().getString();
    next();
    return value;
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




