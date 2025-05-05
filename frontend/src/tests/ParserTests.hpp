#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

#include "lexer/Lexer.hpp"
#include "parser/ActionTableBuilder.hpp"
#include "parser/Parser.hpp"

using namespace compiler;

void testParser(const std::string& input, const std::string& testName) {
  Lexer lexer(input);
  std::cout << "Testing input: \"" << input << "\" (" << testName << ")\n";

  // Create a TokenStream with a buffer size of 10
  TokenStream stream = TokenStream(lexer, 10);

  ActionTableBuilder builder = ActionTableBuilder();

  builder.testGenerateActionTable_SimpleGrammar();

  // Create a parser instance
  // Parser parser = Parser(stream);
  // parser.parse();
  std::cout << "--------------------------------\n";
}