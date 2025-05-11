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

  // testParser(R"(
  //   int a = 5;

  //   void main() {
  //     a += b * 20;
  //   }
  // )",
  //            "PARSER TEST");

  testActionTable();

  std::cout << "All tests passed!\n";
  return 0;
}