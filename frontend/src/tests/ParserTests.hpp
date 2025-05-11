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

//----------------------------------------------------------------
// 1) Define the Symbols you'll use in your tests
//----------------------------------------------------------------
static const Symbol ID = [] {
  Symbol s;
  s.type = Symbol::Type::ID_TERMINAL;
  s.terminal.literal = 0;
  return s;
}();

static const Symbol CONSTANT = [] {
  Symbol s;
  s.type = Symbol::Type::LIT_TERMINAL;
  s.terminal.literal = 1;
  return s;
}();

static const Symbol PLUS = [] {
  Symbol s;
  s.type = Symbol::Type::PUN_TERMINAL;
  s.terminal.punctuator = Punctuator::PLUS;
  return s;
}();

static const Symbol STAR = [] {
  Symbol s;
  s.type = Symbol::Type::PUN_TERMINAL;
  s.terminal.punctuator = Punctuator::STAR;
  return s;
}();

static const Symbol LPAREN = [] {
  Symbol s;
  s.type = Symbol::Type::PUN_TERMINAL;
  s.terminal.punctuator = Punctuator::LPAREN;
  return s;
}();

static const Symbol RPAREN = [] {
  Symbol s;
  s.type = Symbol::Type::PUN_TERMINAL;
  s.terminal.punctuator = Punctuator::RPAREN;
  return s;
}();

// (you already have EXPR, TERM, FACT in your ActionTable construction,
// so they’re not needed here for the token stream)

// End-of-input marker:
static const Symbol ENDOF = [] {
  Symbol end_sym;
  end_sym.type = Symbol::Type::EOF_TERMINAL;
  end_sym.terminal.eof = 2;
  return end_sym;
}();

std::string print_symbol(const Symbol& s) {
  std::ostringstream oss;

  switch (s.type) {
    case Symbol::Type::NON_TERMINAL:
      oss << "NT(" << static_cast<int>(s.nonterminal) << ")";
      break;
    case Symbol::Type::PUN_TERMINAL:
      oss << "P('" << PunctuatorHandler::toString(s.terminal.punctuator)
          << "')";
      break;
    case Symbol::Type::KW_TERMINAL:
      oss << "K('" << KeywordHandler::toString(s.terminal.keyword) << "')";
      break;
    case Symbol::Type::LIT_TERMINAL:
      oss << "L(" << static_cast<int>(s.terminal.literal) << ")";
      break;
    case Symbol::Type::ID_TERMINAL:
      oss << "I(" << static_cast<int>(s.terminal.identifier) << ")";
      break;
    default:
      oss << "ENDOF";
  }

  return oss.str();
}

void print_item(const Rule& rule, const ActionTable::Item& item) {
  std::cout << static_cast<int>(rule.lhs) << " -> ";
  size_t i = 0;
  for (auto it = rule.rhs.begin(); it != rule.rhs.end(); ++it, ++i) {
    if (i == item.dot_position) std::cout << ". ";
    std::cout << print_symbol(*it);
    std::cout << " ";
  }
  if (item.dot_position == rule.rhs.size()) std::cout << "·";
  std::cout << "\n";
}

void printItemSets(const std::vector<ActionTable::ItemSet>& states,
                   const std::vector<Rule>& grammar) {
  for (size_t i = 0; i < states.size(); ++i) {
    std::cout << "State " << i << ":\n";
    for (const auto& item : states[i]) {
      const Rule& rule = grammar[item.rule_index];
      std::cout << "  " << static_cast<uint32_t>(rule.lhs) << " ->";

      size_t pos = 0;
      for (const auto& sym : rule.rhs) {
        if (pos == item.dot_position) std::cout << " .";
        std::cout << " " << print_symbol(sym);
        ++pos;
      }

      if (item.dot_position == rule.rhs.size()) std::cout << " .";

      std::cout << "\n";
    }
    std::cout << "\n";
  }
}

void printPrettyActionAndGotoTables(
    const ActionTable& builder,
    const std::vector<ActionTable::ItemSet>& states) {
  std::vector<Symbol> terminalSymbols;
  std::vector<Symbol> nonTerminalSymbols;

  // Collect terminal and non-terminal symbols
  for (const auto& [key, action] : builder.action_table) {
    const Symbol& symbol = key.second;
    if (std::find(terminalSymbols.begin(), terminalSymbols.end(), symbol) ==
        terminalSymbols.end())
      terminalSymbols.push_back(symbol);
  }

  for (const auto& [key, state] : builder.goto_table) {
    const Symbol& symbol = key.second;
    if (symbol.type == Symbol::Type::NON_TERMINAL) {
      if (std::find(nonTerminalSymbols.begin(), nonTerminalSymbols.end(),
                    symbol) == nonTerminalSymbols.end())
        nonTerminalSymbols.push_back(symbol);
    }
  }

  std::sort(terminalSymbols.begin(), terminalSymbols.end(),
            [](const Symbol& a, const Symbol& b) {
              return (int)a.type < (int)b.type;
            });
  std::sort(nonTerminalSymbols.begin(), nonTerminalSymbols.end(),
            [](const Symbol& a, const Symbol& b) {
              return (int)a.nonterminal < (int)b.nonterminal;
            });

  // === ACTION TABLE HEADER ===
  std::cout << "\n=== ACTION TABLE ===\n";
  printf("%-10s|", "");
  for (const auto& sym : terminalSymbols) {
    printf(" %-6s |", print_symbol(sym).c_str());
  }
  std::cout << "\n";

  for (size_t i = 0; i < states.size(); ++i) {
    printf("State %-3d |", i);
    for (const auto& sym : terminalSymbols) {
      auto it = builder.action_table.find({i, sym});
      if (it != builder.action_table.end()) {
        const auto& action = it->second;
        if (action.type == Action::Type::SHIFT) {
          printf(" s%-5d |", action.next_state);
        } else if (action.type == Action::Type::REDUCE) {
          printf(" r%-5d |", action.rule_index);
        } else if (action.type == Action::Type::ACCEPT) {
          printf(" acc%-3s |", "");
        }
      } else {
        printf("  %5s |", "");
      }
    }
    std::cout << "\n";
  }

  // === GOTO TABLE HEADER ===
  std::cout << "\n=== GOTO TABLE ===\n";
  printf("%-10s|", "");
  for (const auto& sym : nonTerminalSymbols) {
    printf(" %-6s |", print_symbol(sym).c_str());
  }
  std::cout << "\n";

  for (size_t i = 0; i < states.size(); ++i) {
    printf("State %-3zu |", i);
    for (const auto& sym : nonTerminalSymbols) {
      auto it = builder.goto_table.find({i, sym});
      if (it != builder.goto_table.end()) {
        printf(" %-6d |", it->second);
      } else {
        printf(" %-6s |", "");
      }
    }
    std::cout << "\n";
  }
}

bool runTest(ActionTable& table, Grammar& grammar,
             const std::vector<Symbol>& input, bool shouldAccept) {
  std::stack<ActionTable::State> states;
  states.push(0);
  size_t pos = 0;

  Symbol end_sym;
  end_sym.type = Symbol::Type::EOF_TERMINAL;
  end_sym.terminal.eof = 2;

  while (true) {
    ActionTable::State s = states.top();
    Symbol a = (pos < input.size() ? input[pos] : end_sym);

    auto it = table.action_table.find({s, a});
    if (it == table.action_table.end()) {
      std::cout << "Error at state " << s << ", symbol " << print_symbol(a)
                << "\n";
      return shouldAccept == false;
    }
    Action act = it->second;
    if (act.type == Action::SHIFT) {
      states.push(act.next_state);
      pos++;
    } else if (act.type == Action::REDUCE) {
      const Rule& r = grammar[act.rule_index];
      for (size_t i = 0; i < r.rhs.size(); ++i) states.pop();
      ActionTable::State t = states.top();

      Symbol sym;
      sym.type = Symbol::Type::NON_TERMINAL;
      sym.nonterminal = r.lhs;
      states.push(table.goto_table[{t, sym}]);
    } else if (act.type == Action::ACCEPT) {
      return (pos == input.size()) == shouldAccept;
    }
  }
}

void testActionTable() {
  // Define Symbols
  Symbol START;
  START.type = Symbol::Type::NON_TERMINAL;
  START.nonterminal = NonTerminal::START;

  Symbol ENDOF;
  ENDOF.type = Symbol::Type::EOF_TERMINAL;
  ENDOF.terminal.eof = 2;

  Symbol ID;
  ID.type = Symbol::Type::ID_TERMINAL;
  ID.terminal.literal = 0;

  Symbol CONSTANT;
  CONSTANT.type = Symbol::Type::LIT_TERMINAL;
  CONSTANT.terminal.literal = 1;

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

  // Build action table
  ActionTable builder(grammar);
  builder.build();

  /**
   *
   *
   *
   *
   */

  // Pretty print action table
  std::cout << std::left << std::setw(8) << "State" << std::setw(20) << "Symbol"
            << std::setw(10) << "Action" << std::setw(10) << "Value" << "\n";
  std::cout
      << "-------------------------------------------------------------\n";

  for (const auto& [key, action] : builder.action_table) {
    const auto& [state, symbol] = key;

    std::string symbol_str;
    switch (symbol.type) {
      case Symbol::Type::NON_TERMINAL:
        symbol_str =
            "NT(" + std::to_string(static_cast<int>(symbol.nonterminal)) + ")";
        break;
      case Symbol::Type::PUN_TERMINAL:
        symbol_str = "PUNC(" +
                     std::string(PunctuatorHandler::toString(
                         symbol.terminal.punctuator)) +
                     ")";
        break;
      case Symbol::Type::KW_TERMINAL:
        symbol_str =
            "KEYW(" +
            std::string(KeywordHandler::toString(symbol.terminal.keyword)) +
            ")";
        break;
      case Symbol::Type::LIT_TERMINAL:
        symbol_str = "LITERAL(" + std::to_string(symbol.terminal.literal) + ")";
        break;
      case Symbol::Type::EOF_TERMINAL:
        symbol_str = "ENDOF(" + std::to_string(symbol.terminal.eof) + ")";
        break;
      default:
        symbol_str = "UNKNOWN";
    }

    std::string action_str;
    std::string value_str;
    switch (action.type) {
      case Action::SHIFT:
        action_str = "SHIFT";
        value_str = std::to_string(action.next_state);
        break;
      case Action::REDUCE:
        action_str = "REDUCE";
        value_str = std::to_string(action.rule_index);
        break;
      case Action::ACCEPT:
        action_str = "ACCEPT";
        break;
      case Action::ERROR:
        action_str = "ERROR";
        break;
    }

    std::cout << std::left << std::setw(8) << state << std::setw(20)
              << symbol_str << std::setw(10) << action_str << std::setw(10)
              << value_str << "\n";
  }

  /**
   *
   *
   *
   *
   */

  printItemSets(builder.states, grammar);
  printPrettyActionAndGotoTables(builder, builder.states);

  // 1+2*3
  assert(runTest(builder, grammar,
                 {
                     CONSTANT,
                     PLUS,
                     CONSTANT,
                     STAR,
                     CONSTANT,
                 },
                 true));

  // (1 + 2) * 3
  assert(runTest(builder, grammar,
                 {
                     LPAREN,
                     CONSTANT,
                     PLUS,
                     CONSTANT,
                     RPAREN,
                     STAR,
                     CONSTANT,
                 },
                 true));

  // 1 + (2 * 3
  assert(runTest(builder, grammar,
                 {
                     CONSTANT,
                     PLUS,
                     LPAREN,
                     CONSTANT,
                     STAR,
                     CONSTANT,
                 },
                 false));
}
