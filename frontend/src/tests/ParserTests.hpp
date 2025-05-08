#pragma once

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

#include "lexer/Lexer.hpp"
#include "parser/ActionTableBuilder.hpp"
#include "parser/Parser.hpp"

using namespace compiler;

std::string print_symbol(const Symbol& s) {
  std::ostringstream oss;

  switch (s.type) {
    case Symbol::Type::NON_TERMINAL:
      oss << "NT(" << static_cast<int>(s.nonterminal) << ")";
      break;
    case Symbol::Type::PUNCTUATOR:
      oss << "P('" << PunctuatorHandler::toString(s.terminal.punctuator)
          << "')";
      break;
    case Symbol::Type::KEYWORD:
      oss << "K('" << KeywordHandler::toString(s.terminal.keyword) << "')";
      break;
    case Symbol::Type::LITERAL:
      oss << "L(" << static_cast<int>(s.terminal.ident_or_lit_endof) << ")";
      break;
    default:
      oss << "?";
  }

  return oss.str();
}

void print_item(const RuleN& rule, const ActionTableBuilder::Item& item) {
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

void printPrettyActionAndGotoTables(
    const ActionTableBuilder& builder,
    const std::vector<ActionTableBuilder::ItemSet>& states) {
  std::vector<Symbol> terminalSymbols;
  std::vector<Symbol> nonTerminalSymbols;

  // Collect terminal and non-terminal symbols
  for (const auto& [key, action] : builder.action_table) {
    const Symbol& symbol = key.second;
    if (symbol.type == Symbol::Type::NON_TERMINAL) {
      if (std::find(nonTerminalSymbols.begin(), nonTerminalSymbols.end(),
                    symbol) == nonTerminalSymbols.end())
        nonTerminalSymbols.push_back(symbol);
    } else {
      if (std::find(terminalSymbols.begin(), terminalSymbols.end(), symbol) ==
          terminalSymbols.end())
        terminalSymbols.push_back(symbol);
    }
  }

  std::sort(terminalSymbols.begin(), terminalSymbols.end(),
            [](const Symbol& a, const Symbol& b) {
              return (int)a.nonterminal < (int)b.nonterminal;
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
    // std::cout << " ";
    // print_symbol(sym);
    // std::cout << " |";
  }
  std::cout << "\n";

  for (size_t i = 0; i < states.size(); ++i) {
    // std::cout << "State " << i << " |";
    printf("State %-3d |", i);
    for (const auto& sym : terminalSymbols) {
      auto it = builder.action_table.find({i, sym});
      if (it != builder.action_table.end()) {
        const auto& action = it->second;
        if (action.type == ActionTableBuilder::Action::Type::SHIFT) {
          printf(" s%-5d |", action.next_state);
          // std::cout << " s" << action.next_state << " |";
        } else if (action.type == ActionTableBuilder::Action::Type::REDUCE) {
          printf(" r%-5d |", action.rule_index);
          // std::cout << " r" << action.rule_index << " |";
        } else if (action.type == ActionTableBuilder::Action::Type::ACCEPT) {
          printf(" acc%-3s |", "");
          // std::cout << " acc |";
        }
      } else {
        printf("  %5s |", "");
        // std::cout << "     |";
      }
    }
    std::cout << "\n";
  }

  // // === GOTO TABLE HEADER ===
  // std::cout << "\n=== GOTO TABLE ===\n     |";
  // for (const auto& sym : nonTerminalSymbols) {
  //   std::cout << " ";
  //   print_symbol(sym);
  //   std::cout << " |";
  // }
  // std::cout << "\n";

  // for (size_t i = 0; i < states.size(); ++i) {
  //   std::cout << "State " << i << " |";
  //   for (const auto& sym : nonTerminalSymbols) {
  //     auto it = builder.goto_table.find({i, sym});
  //     if (it != builder.goto_table.end()) {
  //       std::cout << " " << it->second << " |";
  //     } else {
  //       std::cout << "     |";
  //     }
  //   }
  //   std::cout << "\n";
  // }
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
  CONSTANT.terminal.ident_or_lit_endof = 1;
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

void testActionTableGeneration() {
  std::cout << "Testing Action Table Generation\n";

  // Define Symbols
  Symbol CONSTANT;
  CONSTANT.type = Symbol::Type::LITERAL;
  CONSTANT.terminal.ident_or_lit_endof = 1;

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

  // Define Grammar
  Grammar grammar;
  grammar.push_back({NonTerminal::Expr, {EXPR, PLUS, TERM}});
  grammar.push_back({NonTerminal::Expr, {TERM}});
  grammar.push_back({NonTerminal::Term, {TERM, STAR, FACT}});
  grammar.push_back({NonTerminal::Term, {FACT}});
  grammar.push_back({NonTerminal::Factor, {LPAREN, EXPR, RPAREN}});
  grammar.push_back({NonTerminal::Factor, {CONSTANT}});

  // Build the Action Table
  ActionTableBuilder builder = ActionTableBuilder(grammar);

  // Generate States using buildStates() or other means
  ActionTableBuilder::Table transitions;
  std::vector<ActionTableBuilder::ItemSet> states;
  builder.buildStates(states, transitions);

  // Now build the Action Table using the states and transitions
  builder.buildTables(states, transitions);

  // Print out the actions for each state and symbol in the table
  for (const auto& entry : builder.action_table) {
    const auto& key = entry.first;
    const auto& action = entry.second;

    std::cout << "State " << key.first << ", Symbol ";
    print_symbol(key.second);
    std::cout << " --> Action: ";

    if (action.type == ActionTableBuilder::Action::Type::SHIFT) {
      std::cout << "SHIFT, Next State: " << action.next_state << std::endl;
    } else if (action.type == ActionTableBuilder::Action::Type::REDUCE) {
      std::cout << "REDUCE, Rule Index: " << action.rule_index << std::endl;
    } else if (action.type == ActionTableBuilder::Action::Type::ACCEPT) {
      std::cout << "ACCEPT" << std::endl;
    }
  }
  printPrettyActionAndGotoTables(builder, states);

  // You can also verify if the action table is correct by checking specific
  // states or transitions. Example: check that State 0 on 'Expr' symbol gives a
  // SHIFT to state 1
  auto shift_action = builder.action_table.find({0, EXPR});
  if (shift_action != builder.action_table.end()) {
    const auto& action = shift_action->second;
    if (action.type == ActionTableBuilder::Action::Type::SHIFT) {
      assert(action.next_state == 1);  // Example expectation
    }
  }

  std::cout << "Action Table Test Finished\n";
}

void testActionTable() {
  // Define Symbols
  Symbol CONSTANT;
  CONSTANT.type = Symbol::Type::LITERAL;
  CONSTANT.terminal.ident_or_lit_endof = 1;

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

  // Define Grammar
  Grammar grammar;
  grammar.push_back({NonTerminal::Expr, {EXPR, PLUS, TERM}});
  grammar.push_back({NonTerminal::Expr, {TERM}});
  grammar.push_back({NonTerminal::Term, {TERM, STAR, FACT}});
  grammar.push_back({NonTerminal::Term, {FACT}});
  grammar.push_back({NonTerminal::Factor, {LPAREN, EXPR, RPAREN}});
  grammar.push_back({NonTerminal::Factor, {CONSTANT}});

  // Build action table
  ActionTableBuilder builder(grammar);
  builder.build();

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
      case Symbol::Type::PUNCTUATOR:
        symbol_str = "PUNC(" +
                     std::string(PunctuatorHandler::toString(
                         symbol.terminal.punctuator)) +
                     ")";
        break;
      case Symbol::Type::KEYWORD:
        symbol_str =
            "KEYW(" +
            std::string(KeywordHandler::toString(symbol.terminal.keyword)) +
            ")";
        break;
      case Symbol::Type::LITERAL:
        symbol_str = "LITERAL(" +
                     std::to_string(symbol.terminal.ident_or_lit_endof) + ")";
        break;
      case Symbol::Type::ENDOF:
        symbol_str =
            "ENDOF(" + std::to_string(symbol.terminal.ident_or_lit_endof) + ")";
        break;
      default:
        symbol_str = "UNKNOWN";
    }

    std::string action_str;
    std::string value_str;
    switch (action.type) {
      case ActionTableBuilder::Action::SHIFT:
        action_str = "SHIFT";
        value_str = std::to_string(action.next_state);
        break;
      case ActionTableBuilder::Action::REDUCE:
        action_str = "REDUCE";
        value_str = std::to_string(action.rule_index);
        break;
      case ActionTableBuilder::Action::ACCEPT:
        action_str = "ACCEPT";
        break;
      case ActionTableBuilder::Action::ERROR:
        action_str = "ERROR";
        break;
    }

    std::cout << std::left << std::setw(8) << state << std::setw(20)
              << symbol_str << std::setw(10) << action_str << std::setw(10)
              << value_str << "\n";
  }
}
