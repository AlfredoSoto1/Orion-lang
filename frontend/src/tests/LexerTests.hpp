#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "lexer/Lexer.hpp"

using namespace compiler;

void testLexer(const std::string& input, const std::string& testName) {
  Lexer lexer(input);
  std::cout << "Testing input: \"" << input << "\" (" << testName << ")\n";

  while (true) {
    auto result = lexer.advance();

    if (result) {
      lexer.debugPrintToken(*result);
      if ((*result).type == TokenType::ENDOF) return;
    } else {
      LexerError error = result.error();
      std::cerr << "Lexer Error at line " << error.line << ", col " << error.pos
                << ": " << error.to_string() << "\n";
      break;
    }
  }
  std::cout << "--------------------------------\n";
}