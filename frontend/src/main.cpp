#include "tests/ActionTableTests.hpp"
#include "tests/LexerTests.hpp"
#include "tests/ParserTests.hpp"

using namespace compiler;

int main() {
  // testLexer(R"(
  //   int a = 5;
  //   int b = 10; // Another variable

  //   bool n = "my string
  //             in two lines";

  //   void main() {
  //     a += b * 20;
  //   }
  // )",
  //           "LEXER TEST");

  // testActionTable();

  testParser(R"(
    &&*** + (2 * 4)
  )",
             "PARSER TEST");

  std::cout << "All tests passed!\n";
  return 0;
}