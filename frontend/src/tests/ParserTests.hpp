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

  // Expr → Expr + Term
  //      | Term

  // Term → Term * Factor
  //      | Factor

  // Factor  → ( Expr )
  //         | number

  Symbol CONSTANT;
  CONSTANT.type = Symbol::Type::LITERAL;
  CONSTANT.terminal.ident_or_lit = 1;
  Symbol PLUS;
  PLUS.type = Symbol::Type::PUNCTUATOR;
  PLUS.terminal.punctuator == Punctuator::PLUS;
  Symbol STAR;
  STAR.type = Symbol::Type::PUNCTUATOR;
  STAR.terminal.punctuator == Punctuator::STAR;
  Symbol LPAREN;
  LPAREN.type = Symbol::Type::PUNCTUATOR;
  LPAREN.terminal.punctuator == Punctuator::LPAREN;
  Symbol RPAREN;
  RPAREN.type = Symbol::Type::PUNCTUATOR;
  RPAREN.terminal.punctuator == Punctuator::RPAREN;

  Symbol EXPR;
  EXPR.type = Symbol::Type::NON_TERMINAL;
  EXPR.nonterminal = NonTerminal::Expr;
  Symbol TERM;
  TERM.type = Symbol::Type::NON_TERMINAL;
  TERM.nonterminal = NonTerminal::Term;
  Symbol FACT;
  FACT.type = Symbol::Type::NON_TERMINAL;
  FACT.nonterminal = NonTerminal::Factor;

  Grammar grammar;
  grammar.insert({NonTerminal::Expr, {EXPR, PLUS, TERM}});
  grammar.insert({NonTerminal::Expr, {TERM}});
  grammar.insert({NonTerminal::Term, {TERM, STAR, FACT}});
  grammar.insert({NonTerminal::Term, {FACT}});
  grammar.insert({NonTerminal::Factor, {LPAREN, EXPR, RPAREN}});
  grammar.insert({NonTerminal::Factor, {CONSTANT}});

  ActionTableBuilder builder = ActionTableBuilder(grammar);
  builder.build();

  // builder.testGenerateActionTable_SimpleGrammar();

  // Create a parser instance
  // Parser parser = Parser(stream);
  // parser.parse();
  std::cout << "--------------------------------\n";
}