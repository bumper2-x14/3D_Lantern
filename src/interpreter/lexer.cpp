#include <fstream>
#include <sstream>
#include "lexer.h"

Lexer::Lexer(const std::string& _path) : src_path(_path) {
    int dot = src_path.rfind('.');
    if (dot == std::string::npos || src_path.substr(dot) != ".lnt") {
        throw std::runtime_error("Lexer: invalid file extension, expected '.lnt' -> " + src_path);
    }
}

const std::vector<Token>& Lexer::getTokens() const {
    return tokens;
}

void Lexer::tokenize() {
    std::ifstream file(src_path);
    if (!file.good()) {
        throw std::runtime_error("Lexer::tokenize -> Source file was not found.");
    }

    std::string word;
    int line = 1;
    char c;

    // Define a lambda function in charge of creating tokens
    auto generator = [&]() {
        if (!word.empty()){
            tokens.push_back(makeToken(word, line));// Handle left brace
            word.clear();
        }
    };

    while(file.get(c)) {
        if(c == '\n') {
            // Handle end of line
            generator();
            ++line;
        }
        else if(c == '#'){
            // Handle comments
            generator();
            while (file.get(c) && c != '\n');
            ++line;
        }
        else if(c == '{') {
            // Handle left brace
            generator();
            tokens.push_back(makeLBraceToken(line));
        }
        else if(c == '}') {
            // Handle right brace
            generator();
            tokens.push_back(makeRBraceToken(line));
        } 
        else if(std::isspace(c)) {
            // Handle spaces and tabs
            generator();
        }
        else {
            word += c;
        }
    }

    generator(); // handle last word if file doesn't end with whitespace
    tokens.push_back(makeEndToken(line));
    file.close();
}

void Lexer::tokenizeFromString(const std::string& source) {
    std::istringstream file(source);
    std::string word;
    int line = 1;
    char c;

    auto generator = [&]() {
        if (!word.empty()) {
            tokens.push_back(makeToken(word, line));
            word.clear();
        }
    };

    while (file.get(c)) {
        if (c == '\n') { generator(); ++line; }
        else if (c == '#') { generator(); while (file.get(c) && c != '\n'); ++line; }
        else if (c == '{') { generator(); tokens.push_back(makeLBraceToken(line)); }
        else if (c == '}') { generator(); tokens.push_back(makeRBraceToken(line)); }
        else if (std::isspace(c)) { generator(); }
        else { word += c; }
    }

    generator();
    tokens.push_back(makeEndToken(line));
}

void Lexer::logger(std::ostream& os) const {
        os << "=== Lexer output: " << tokens.size() << " tokens ===\n";
        for (const auto& t : tokens)
            os << t << "\n";
}


void Lexer::regressionTest() {
    struct TestCase {
        std::string name;
        std::string input;
        std::vector<std::pair<TokenType, std::string>> expected;
    };

    std::vector<TestCase> cases = {
        {
            "basic block with spaces",
            "object { type sphere }",
            {
                { TokenType::KEYWORD, "object" },
                { TokenType::LBRACE,  "{"      },
                { TokenType::KEYWORD, "type"   },
                { TokenType::KEYWORD, "sphere" },
                { TokenType::RBRACE,  "}"      },
                { TokenType::END,     ""       }
            }
        },
        {
            "block no spaces around braces",
            "object{type sphere}",
            {
                { TokenType::KEYWORD, "object" },
                { TokenType::LBRACE,  "{"      },
                { TokenType::KEYWORD, "type"   },
                { TokenType::KEYWORD, "sphere" },
                { TokenType::RBRACE,  "}"      },
                { TokenType::END,     ""       }
            }
        },
        {
            "numeric values with negative",
            "position 1.0 -2.5 3.0",
            {
                { TokenType::KEYWORD, "position" },
                { TokenType::NUMBER,  "1.0"      },
                { TokenType::NUMBER,  "-2.5"     },
                { TokenType::NUMBER,  "3.0"      },
                { TokenType::END,     ""         }
            }
        },
        {
            "quoted string value",
            "name \"my_sphere\"",
            {
                { TokenType::KEYWORD, "name"      },
                { TokenType::STRING,  "my_sphere" },
                { TokenType::END,     ""          }
            }
        },
        {
            "user-defined identifier",
            "material red_metal",
            {
                { TokenType::KEYWORD,    "material"  },
                { TokenType::IDENTIFIER, "red_metal" },
                { TokenType::END,        ""          }
            }
        },
        {
            "boolean value",
            "capped true",
            {
                { TokenType::KEYWORD, "capped" },
                { TokenType::BOOLEAN, "true"   },
                { TokenType::END,     ""       }
            }
        },
        {
            "multi-line block",
            "object {\n    type cone\n    radius 1.5\n}",
            {
                { TokenType::KEYWORD, "object" },
                { TokenType::LBRACE,  "{"      },
                { TokenType::KEYWORD, "type"   },
                { TokenType::KEYWORD, "cone"   },
                { TokenType::KEYWORD, "radius" },
                { TokenType::NUMBER,  "1.5"    },
                { TokenType::RBRACE,  "}"      },
                { TokenType::END,     ""       }
            }
        },
        {
            "quoted file path",
            "file \"path/to/mesh.obj\"",
            {
                { TokenType::KEYWORD, "file"             },
                { TokenType::STRING,  "path/to/mesh.obj" },
                { TokenType::END,     ""                 }
            }
        },
        {
            "tabs as whitespace",
            "radius\t2.0",
            {
                { TokenType::KEYWORD, "radius" },
                { TokenType::NUMBER,  "2.0"    },
                { TokenType::END,     ""       }
            }
        },
        {
            "multiple booleans",
            "capped true visible false",
            {
                { TokenType::KEYWORD, "capped"  },
                { TokenType::BOOLEAN, "true"    },
                { TokenType::KEYWORD, "visible" },
                { TokenType::BOOLEAN, "false"   },
                { TokenType::END,     ""        }
            }
        }
    };

    const std::string tmpPath = "__lexer_test_tmp__.rts";
    const std::string sep(60, '-');
    int passed = 0, failed = 0;

    std::cout << sep << "\n";
    std::cout << "  Lexer regression test\n";
    std::cout << sep << "\n";

    for (int i = 0; i < (int)cases.size(); ++i) {
        const auto& tc = cases[i];

        {
            std::ofstream f(tmpPath);
            f << tc.input;
        }

        Lexer lexer(tmpPath);
        lexer.tokenize();
        const auto& tokens = lexer.getTokens();

        // check token count and each token's type + raw
        bool ok = tokens.size() == tc.expected.size();
        if (ok) {
            for (int j = 0; j < (int)tokens.size(); ++j) {
                if (tokens[j].type != tc.expected[j].first ||
                    tokens[j].raw  != tc.expected[j].second) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok) {
            std::cout << "[PASS] (" << i << ") " << tc.name << "\n";
            ++passed;
        } else {
            std::cout << "[FAIL] (" << i << ") " << tc.name << "\n";
            std::cout << "  input   : " << tc.input << "\n";
            std::cout << "  tokens  : " << tokens.size()
                      << " got, " << tc.expected.size() << " expected\n";

            int maxJ = (int)std::max(tokens.size(), tc.expected.size());
            std::cout << "  " << std::setw(3) << "#"
                      << "  " << std::setw(30) << std::left << "got"
                      << "  " << "expected\n";

            for (int j = 0; j < maxJ; ++j) {
                std::string got_str  = "-";
                std::string exp_str  = "-";
                bool mismatch        = false;

                if (j < (int)tokens.size())
                    got_str = logString(tokens[j].type) + " '" + tokens[j].raw + "'";

                if (j < (int)tc.expected.size())
                    exp_str = logString(tc.expected[j].first) + " '" + tc.expected[j].second + "'";

                if (j < (int)tokens.size() && j < (int)tc.expected.size())
                    mismatch = tokens[j].type != tc.expected[j].first ||
                               tokens[j].raw  != tc.expected[j].second;

                std::cout << "  " << std::setw(3) << j
                          << "  " << std::setw(30) << std::left << got_str
                          << "  " << exp_str
                          << (mismatch ? "  <--" : "") << "\n";
            }
            ++failed;
        }
    }

    std::remove(tmpPath.c_str());
    std::cout << sep << "\n";
    std::cout << "  " << passed << "/" << (passed + failed) << " passed";
    if (failed == 0) std::cout << "  -- all good\n";
    else             std::cout << "  -- " << failed << " failed\n";
    std::cout << sep << "\n";
}