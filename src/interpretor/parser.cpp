#include <iostream>
#include "parser.h"

Parser::Parser(const std::vector<Token>& _tokens): tokens(_tokens){}

const Token& Parser::current() const {
    if (cursor >= tokens.size()) return tokens.back();
    return tokens[cursor];
}

const Token& Parser::peek(int offset) const {
    return tokens[cursor + offset];
}

const Token& Parser::next() {
    if (cursor + 1 < tokens.size()) ++cursor;
    return tokens[cursor];
}

SceneDescriptor Parser::parse() {
    assert(!tokens.empty() && tokens.back().is(TokenType::END)
           && "Lexer must append an END token");
    
    SceneDescriptor scene;

    while (!current().is(TokenType::END)) {
        if (current().type != TokenType::KEYWORD) {
            std::cout << "Parser::parse -> Error: expected block keyword, got '"
                      << current().raw << "' at line " << current().line << "\n";
            exit(1);
        }

        BlockType block = current().getKeyword<BlockType>();

        switch(block) {
            case BlockType::SETTING: parseSettingBlock(scene); break;
            case BlockType::CAMERA: parseCameraBlock(scene); break;
            case BlockType::TEXTURE: parseTextureBlock(scene); break;
            case BlockType::MATERIAL: parseMaterialBlock(scene); break;
            case BlockType::OBJECT: parseObjectBlock(scene); break;
            case BlockType::LIGHT: parseLightBlock(scene); break;

            default:
                std::cout << "Parser::parse -> Error: unknown block '"
                          << current().raw << "' at line " << current().line << "\n";
                exit(1);
        }
        next();
    }

    return scene;
}

// -------------------------------------------------------
// Block parsers — each consumes one complete { } block
// -------------------------------------------------------

void Parser::parseSettingBlock(SceneDescriptor& scene) {
    expectLBrace();
    while(!current().isRBrace()) {
        if (current().type != TokenType::KEYWORD) {
            std::cout << "Parser::parseSettingBlock -> Error: expected keyword in settings, got '"
                      << current().raw << "' at line " << current().line << "\n";
            exit(1);
        }

        IdentType field = current().getKeyword<IdentType>();
    
        switch(field) {
            case IdentType::WIDTH: scene.settings.setWidth((int)parseNumber()); break;
            case IdentType::HEIGHT: scene.settings.setHeight((int)parseNumber()); break;
            case IdentType::SAMPLES: scene.settings.setSamples((int)parseNumber()); break;
            case IdentType::DEPTH: scene.settings.setDepth((int)parseNumber()); break;
            case IdentType::ASPECT_RATIO: scene.settings.setAspectRatio(parseNumber()); break;
            case IdentType::BACKGROUND: scene.settings.setBackground(parseVec3()); break;
            default:
                std::cout << "Parser::parseSettingBlock -> Error: unknown setting field '"
                          << current().raw << "' at line " << current().line << "\n";
                exit(1);
        }
        next();
    }
    expectRBrace();
}

void Parser::parseCameraBlock(SceneDescriptor& scene) {
    expectLBrace();
    while(!current().isRBrace()) {
        if (current().type != TokenType::KEYWORD) {
            std::cout << "Parser::parseCameraBlock -> Error: expected keyword in camera, got '"
                      << current().raw << "' at line " << current().line << "\n";
            exit(1);
        }

        IdentType field = current().getKeyword<IdentType>();

        switch(field) {
            case IdentType::POSITION: scene.camera.setPosition(parseVec3()); break;
            case IdentType::LOOKAT: scene.camera.setLookat(parseVec3()); break;
            case IdentType::UP_VIEW: scene.camera.setUpView(parseVec3()); break;
            case IdentType::VFOV: scene.camera.setVfov(parseNumber()); break;
            case IdentType::DEFOCUS_ANGLE: scene.camera.setDefocusAngle(parseNumber()); break;
            case IdentType::FOCUS_DISTANCE: scene.camera.setFocusDistance(parseNumber()); break;
            default:
                std::cout << "Parser::parseCameraBlock -> Error: unknown camera field '"
                          << current().raw << "' at line " << current().line << "\n";
                exit(1);
        }
        next();
    }
    expectRBrace();
}

void Parser::parseTextureBlock(SceneDescriptor& scene) {
    expectLBrace();

    TextureDescriptor texture;

    while(!current().isRBrace()) {
        if (current().type != TokenType::KEYWORD) {
            std::cout << "Parser::parseTextureBlock -> Error: expected keyword in texture, got '"
                      << current().raw << "' at line " << current().line << "\n";
            exit(1);
        }

        IdentType field = current().getKeyword<IdentType>();

        switch(field) {
            case IdentType::NAME: texture.setName(parseIdentifier()); break;

            case IdentType::TYPE:
                next();
                texture.setType(current().getKeyword<TextureType>());
                break;

            case IdentType::COLOR: texture.setColor(parseVec3()); break;
            case IdentType::EVEN: texture.setEven(parseVec3()); break;
            case IdentType::ODD: texture.setOdd(parseVec3()); break;
            case IdentType::SCALE: texture.setScale(parseNumber()); break;
            case IdentType::FILE: texture.setFile(parseString()); break;

            default:
                std::cout << "Parser::parseTextureBlock -> Error: unknown texture field '"
                          << current().raw << "' at line " << current().line << "\n";
                exit(1);
        }
        next();
    }

    expectRBrace();

    scene.textures[texture.name] = texture;
}

void Parser::parseMaterialBlock(SceneDescriptor& scene) {
    expectLBrace();

    MaterialDescriptor material;

    while(!current().isRBrace()) {
        if (current().type != TokenType::KEYWORD) {
            std::cout << "Parser::parseMaterialBlock -> Error: expected keyword in material, got '"
                      << current().raw << "' at line " << current().line << "\n";
            exit(1);
        }

        IdentType field = current().getKeyword<IdentType>();


        switch(field) {
            case IdentType::NAME: material.setName(parseIdentifier()); break;

            case IdentType::TYPE:
                next();
                material.setType(current().getKeyword<MaterialType>());
                break;

            case IdentType::TEXTURE_REF: material.setTextureRef(parseIdentifier()); break;
            case IdentType::COLOR: material.setColor(parseVec3()); break;
            case IdentType::FUZZ: material.setFuzz(parseNumber()); break;
            case IdentType::IOR: material.setIor(parseNumber()); break;
            case IdentType::INTENSITY: material.setIntensity(parseNumber()); break;

            default:
                std::cout << "Parser::parseMaterialBlock -> Error: unknown material field '"
                          << current().raw << "' at line " << current().line << "\n";
                exit(1);
        }
        next();
    }

    expectRBrace();

    scene.materials[material.name] = material;
}

void Parser::parseObjectBlock(SceneDescriptor& scene) {
    expectLBrace();

    ObjectDescriptor object;

    while(!current().isRBrace()) {
        if (current().type != TokenType::KEYWORD) {
            std::cout << "Parser::parseObjectBlock -> Error: expected keyword in object, got '"
                      << current().raw << "' at line " << current().line << "\n";
            exit(1);
        }

        IdentType field = current().getKeyword<IdentType>();

        switch(field) {
            case IdentType::NAME: object.setName(parseIdentifier()); break;

            case IdentType::TYPE:
                next();
                object.setType(current().getKeyword<ObjectType>());
                break;

            case IdentType::MATERIAL_REF: object.setMaterialRef(parseIdentifier()); break;
            case IdentType::FILE: object.setFile(parseString()); break;
            case IdentType::TRANSLATE: object.setTranslate(parseVec3()); break;
            case IdentType::ROTATE: object.setRotate(parseVec3()); break;
            case IdentType::SCALE: object.setScale(parseVec3()); break;

            default:
                std::cout << "Parser::parseObjectBlock -> Error: unknown object field '"
                          << current().raw << "' at line " << current().line << "\n";
                exit(1);
        }
        next();
    }

    expectRBrace();

    scene.objects[object.name] = object;
}

void Parser::parseLightBlock(SceneDescriptor& scene) {
    expectLBrace();

    LightDescriptor light;

    while(!current().isRBrace()) {
        if (current().type != TokenType::KEYWORD) {
            std::cout << "Parser::parseLightBlock -> Error: expected keyword in light, got '"
                      << current().raw << "' at line " << current().line << "\n";
            exit(1);
        }

        IdentType field = current().getKeyword<IdentType>();

        switch(field) {
            case IdentType::TYPE:
                next();
                light.setType(current().getKeyword<LightType>());
                break;

            case IdentType::POSITION: light.setPosition(parsePoint3()); break;
            case IdentType::DIRECTION: light.setDirection(parseVec3()); break;
            case IdentType::COLOR: light.setColor(parseVec3()); break;
            case IdentType::INTENSITY: light.setIntensity(parseNumber()); break;

            default:
                std::cout << "Parser::parseLightBlock -> Error: unknown light field '"
                          << current().raw << "' at line " << current().line << "\n";
                exit(1);
        }

        next();
    }

    expectRBrace();

    scene.lights.push_back(light);
}
// -------------------------------------------------------
// Helper parsers — consume one or more tokens for a value
// -------------------------------------------------------

double Parser::parseNumber() {
    double value = current().getNumber();
    next();
    return value;
}

bool Parser::parseBool() {
    bool value = current().getBool();
    next();
    return value;
}


std::string Parser::parseString() {
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
        std::cout << "Parser::parseIdentifier -> Error: Expected identifier, got '" << current().raw << "'" << "\n";
        exit(1);
    }

    std::string value = current().raw;
    next();
    return value;
}

void Parser::expectLBrace() {
    if (!current().isLBrace()) {
        std::cout << "Parser::expectLBrace -> Error: Expected '{' at line "<< current().line << "\n";
        exit(1);
    }
    next();
}

void Parser::expectRBrace() {
    if (!current().isRBrace()) {
        std::cout << "Parser::expectRBrace -> Error: Expected '}' at line "<< current().line << "\n";
        exit(1);
    }
    next();
}




