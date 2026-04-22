#include "interpreter.h"

Interpreter::Interpreter(const std::string& _path) {
    Lexer lex(_path);
    lex.tokenize();
    Parser parser(lex.getTokens());
    scene_des = parser.parse();
}


RT_Renderer& Interpreter::makeRayTracer() {
    //RT_Renderer()
}