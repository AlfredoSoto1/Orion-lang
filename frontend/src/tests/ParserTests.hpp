#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

#include "lexer/Lexer.hpp"
#include "parser/ActionTableBuilder.hpp"
#include "parser/Parser.hpp"

using namespace compiler;

void print_symbol(const Symbol& s) {
  switch (s.type) {
    case Symbol::Type::NON_TERMINAL:
      std::cout << "NT(" << static_cast<int>(s.nonterminal) << ")";
      break;
    case Symbol::Type::PUNCTUATOR:
      std::cout << "P(" << static_cast<int>(s.terminal.punctuator) << ")";
      break;
    case Symbol::Type::LITERAL:
      std::cout << "L(" << static_cast<int>(s.terminal.ident_or_lit) << ")";
      break;
    default:
      std::cout << "?";
  }
}

void print_item(const RuleN& rule, const ActionTableBuilder::Item& item) {
  std::cout << static_cast<int>(rule.lhs) << " -> ";
  size_t i = 0;
  for (auto it = rule.rhs.begin(); it != rule.rhs.end(); ++it, ++i) {
    if (i == item.dot_position) std::cout << ". ";
    print_symbol(*it);
    std::cout << " ";
  }
  if (item.dot_position == rule.rhs.size()) std::cout << "·";
  std::cout << "\n";
}

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
  PLUS.terminal.punctuator = Punctuator::PLUS;
  Symbol STAR;
  STAR.type = Symbol::Type::PUNCTUATOR;
  STAR.terminal.punctuator = Punctuator::STAR;
  Symbol LPAREN;
  LPAREN.type = Symbol::Type::PUNCTUATOR;
  LPAREN.terminal.punctuator = Punctuator::LPAREN;
  Symbol RPAREN;
  RPAREN.type = Symbol::Type::PUNCTUATOR;
  RPAREN.terminal.punctuator = Punctuator::RPAREN;

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
  grammar.push_back({NonTerminal::Expr, {EXPR, PLUS, TERM}});
  grammar.push_back({NonTerminal::Expr, {TERM}});
  grammar.push_back({NonTerminal::Term, {TERM, STAR, FACT}});
  grammar.push_back({NonTerminal::Term, {FACT}});
  grammar.push_back({NonTerminal::Factor, {LPAREN, EXPR, RPAREN}});
  grammar.push_back({NonTerminal::Factor, {CONSTANT}});

  ActionTableBuilder builder = ActionTableBuilder(grammar);
  // builder.build();

  // Initial item: rule 0, dot at position 0 (· Expr + Term)
  ActionTableBuilder::ItemSet start;
  start.insert({0, 0});

  std::cout << "== CLOSURE of { [0,0] } ==\n";
  ActionTableBuilder::ItemSet closure_result = builder.closure(start);
  for (const auto& item : closure_result) {
    print_item(grammar[item.rule_index], item);
  }

  std::cout << "\n== GOTO on Expr ==\n";
  ActionTableBuilder::ItemSet goto_result = builder.goTo(closure_result, EXPR);
  for (const auto& item : goto_result) {
    print_item(grammar[item.rule_index], item);
  }

  // Create a parser instance
  // Parser parser = Parser(stream);
  // parser.parse();
  std::cout << "--------------------------------\n";
}