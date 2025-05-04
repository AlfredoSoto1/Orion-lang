#include "tests/lexer/LexerTests.hpp"

using namespace compiler;

int main() {
  // testLexer(R"(
  //   int a = 5;
  //   int b = 10; // Another variable

  //   bool n = false;

  //   void main() {
  //     a += b * 20;
  //   }
  // )",
  //           "LEXER TEST");

  testLexer(R"(
    1 1i 1l 1u 1ul 1f
  )",
            "LEXER TEST");

  std::cout << "Token " << sizeof(Token) << "\n";
  std::cout << "TokenType " << sizeof(TokenType) << "\n";
  std::cout << "Literal " << sizeof(Literal) << "\n";
  std::cout << "Identifier " << sizeof(Identifier) << "\n";
  std::cout << "TokenValue " << sizeof(TokenValue) << "\n";
  std::cout << "ShortString_view " << sizeof(string_lit) << "\n";
  std::cout << "string_view " << sizeof(std::string_view) << "\n";

  // Rule r1 = {};
  // r1.symbols[0].type = Symbol::Type::NON_TERMINAL;
  // r1.symbols[0].nonterminal = NonTerminal::EXPR;
  // r1.symbols[1].type = Symbol::Type::IDENTIFIER;
  // r1.symbols[1].terminal.ident_or_lit_or_eof = 1;
  // r1.symbols[2].type = Symbol::Type::NON_TERMINAL;
  // r1.symbols[2].nonterminal = NonTerminal::EXPR;

  // std::cout << "Rule " << sizeof(Rule) << " " << std::bitset<32>(r1.i1) <<
  // ":"
  //           << std::bitset<32>(r1.i2) << ":" << std::bitset<32>(r1.i3) << ":"
  //           << ":" << std::bitset<32>(r1.i4) << ":" << std::bitset<32>(r1.i5)
  //           << "\n";

  std::cout << "All tests passed!\n";
  return 0;
}