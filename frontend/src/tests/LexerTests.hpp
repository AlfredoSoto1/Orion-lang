#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "lexer/Lexer.hpp"

using namespace compiler;

void testLexer(const std::string& input, const std::string& testName) {
  Lexer lexer("nosource.c", input);
  std::cout << "Testing input: \"" << input << "\" (" << testName << ")\n";

  while (true) {
    auto result = lexer.advance();

    if (result) {
      std::cout << result->toString(input) << "\n";
      if ((*result).type == TokenType::ENDOF) return;
    } else {
      LexerError error = result.error();
      std::cerr << "Lexer Error at line " << error.state.line << ", col "
                << error.state.column << ": " << error.toString() << "\n";
      break;
    }
  }
  std::cout << "--------------------------------\n";
}