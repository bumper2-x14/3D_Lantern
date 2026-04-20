#include "interpreter/lexer.h"

int main() {
    Lexer lex(ROOT_DIR "test/interpretor_test/lex_test_code.lnt");
    lex.tokenize();
    lex.logger();

    return 0;
}