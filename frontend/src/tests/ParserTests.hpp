#pragma once

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>

#include "lexer/Lexer.hpp"
#include "parser/ActionTable.hpp"
#include "parser/Parser.hpp"

using namespace compiler;

void testParser(const std::string& input, const std::string& testName) {
  // Define Symbols
  Symbol START;
  START.type = Symbol::Type::NON_TERMINAL;
  START.nonterminal = NonTerminal::START;

  Symbol ID = Symbol::identifier();
  Symbol CONSTANT = Symbol::literal();

  Symbol PLUS;
  PLUS.type = Symbol::Type::PUN_TERMINAL;
  PLUS.terminal.punctuator = Punctuator::PLUS;

  Symbol STAR;
  STAR.type = Symbol::Type::PUN_TERMINAL;
  STAR.terminal.punctuator = Punctuator::STAR;

  Symbol LPAREN;
  LPAREN.type = Symbol::Type::PUN_TERMINAL;
  LPAREN.terminal.punctuator = Punctuator::LPAREN;

  Symbol RPAREN;
  RPAREN.type = Symbol::Type::PUN_TERMINAL;
  RPAREN.terminal.punctuator = Punctuator::RPAREN;

  Symbol EXPR;
  EXPR.type = Symbol::Type::NON_TERMINAL;
  EXPR.nonterminal = NonTerminal::EXPR;

  Symbol TERM;
  TERM.type = Symbol::Type::NON_TERMINAL;
  TERM.nonterminal = NonTerminal::TERM;

  Symbol FACT;
  FACT.type = Symbol::Type::NON_TERMINAL;
  FACT.nonterminal = NonTerminal::FACT;

  // Define Grammar
  Grammar grammar;
  grammar.push_back({NonTerminal::START, {EXPR}});
  grammar.push_back({NonTerminal::EXPR, {EXPR, PLUS, TERM}});
  grammar.push_back({NonTerminal::EXPR, {TERM}});
  grammar.push_back({NonTerminal::TERM, {TERM, STAR, FACT}});
  grammar.push_back({NonTerminal::TERM, {FACT}});
  grammar.push_back({NonTerminal::FACT, {LPAREN, EXPR, RPAREN}});
  grammar.push_back({NonTerminal::FACT, {CONSTANT}});

  std::cout << "Testing input: \"" << input << "\" (" << testName << ")\n";
  Lexer lexer("no_source.c", input);
  TokenStream stream = TokenStream(lexer, 10);
  Parser parser = Parser(stream, grammar);
  auto result = parser.parse();

  if (result) {
    std::cout << "Parse successful!\n";
  } else {
    std::cerr << "Parse error: " << result.error().toString() << "\n";
  }
}