#include "interpretor/lexer.h"
#include "interpretor/parser.h"
#include "interpretor/scene_descriptor_logger.h"

int main() {
    Lexer lex(ROOT_DIR "test/interpretor_test/lex_test_code.lnt");
    lex.tokenize();
    Parser parser(lex.getTokens());
    SceneDescriptor sc = parser.parse();
    logSceneDescriptor(std::cout, sc);

    return 0;
}