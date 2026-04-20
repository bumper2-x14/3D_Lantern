#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "scene_descriptor.h"


/// @brief Consumes a flat token stream produced by the Lexer and fills
///        a SceneDescriptor with the parsed scene data.
///        The Parser has no knowledge of the RT or GL renderers —
///        it only validates syntax and maps tokens to descriptor fields.
class Parser {
    public:
        /// @brief Constructs the parser with a reference to the token stream.
        /// @param _tokens Token stream produced by Lexer::tokenize().
        Parser(const std::vector<Token>& _tokens);
        
        /// @brief Parses the full token stream and returns a populated SceneDescriptor.
        /// @return SceneDescriptor holding all parsed scene data ready for the Interpreter.
        SceneDescriptor parse();

    private:
        const std::vector<Token>& tokens; ///< Reference to the token stream, no ownership.
        size_t cursor = 0; ///< Index of the current token in the stream.

        // -------------------------------------------------------
        // Token navigation
        // -------------------------------------------------------
        

        /// @brief Returns the token at the current cursor position without advancing
        const Token& current() const; 

        /// @brief Returns the token at cursor + offset without advancing the cursor.
        /// @param offset Distance ahead to peek (default 1 = next token).
        const Token& peek(int offset = 1) const;

        /// @brief Advances the cursor by one and returns the token that was just passed.
        const Token& next();

        // -------------------------------------------------------
        // Block parsers — each consumes one complete { } block
        // -------------------------------------------------------

        /// @brief Parses a setting { } block and fills scene.settings.
        void parseSettingBlock (SceneDescriptor& scene);

        /// @brief Parses a camera { } block and fills scene.camera.
        void parseCameraBlock (SceneDescriptor& scene);  
        
        /// @brief Parses a texture { } block and inserts into scene.textures.
        void parseTextureBlock (SceneDescriptor& scene);    

        /// @brief Parses a material { } block and inserts into scene.materials.
        void parseMaterialBlock(SceneDescriptor& scene);    

        /// @brief Parses an object { } block and inserts into scene.objects.
        void parseObjectBlock (SceneDescriptor& scene);    

        /// @brief Parses a light { } block and appends to scene.lights.
        void parseLightBlock (SceneDescriptor& scene);    

        // -------------------------------------------------------
        // Helper parsers — consume one or more tokens for a value
        // -------------------------------------------------------
        
        /// @brief Reads three consecutive NUMBER tokens and returns them as a Vec3d.
        Vec3d parseVec3();

        /// @brief Reads three consecutive NUMBER tokens and returns them as a Point3dd.
        Point3d parsePoint3();

        /// @brief Reads a single NUMBER token and returns its value as a double.
        double parseNumber();

        /// @brief Reads a single BOOLEAN token and returns its value.
        bool parseBool();

        /// @brief Reads a single STRING token (quoted) and returns the inner string.
        std::string parseString();

        /// @brief Reads a single IDENTIFIER token and returns the name string.
        std::string parseIdentifier();

        /// @brief Asserts the current token is LBRACE and advances, throws otherwise.
        void expectLBrace();

        /// @brief Asserts the current token is RBRACE and advances, throws otherwise.
        void expectRBrace();
};

#endif