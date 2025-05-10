#include "Parser.hpp"

#include <iostream>
#include <string>

namespace compiler {

  Parser::Parser(TokenStream& tokens, const Grammar& grammar) noexcept
      : tokens(tokens) {}

  // void Parser::parse() {
  //   std::vector<Symbol> symbols;
  //   std::vector<uint32_t> states;

  //   // Initial conditions
  //   states.push_back(0);
  //   ParserResult lookahead = nextSymbol();

  //   do {
  //     // Validate that the symbol is not garbage.
  //     if (lookahead) {
  //       return;  // Error
  //     }

  //     // uint32_t state = states.back();
  //     // Action action = actionTable[{state, *lookahead}];

  //     // switch (action.type) {
  //     //   case Action::SHIFT: {
  //     //     // Shift the current symbol to stack and push to the next state.
  //     //     symbols.push_back(*lookahead);
  //     //     states.push_back(action.next_state);

  //     //     // Advance to the next symbol.
  //     //     lookahead = nextSymbol();
  //     //     break;
  //     //   }

  //     //   case Action::REDUCE: {
  //     //     break;
  //     //   }

  //     //   case Action::ACCEPT:
  //     //     std::cout << "Parsing successful.\n";
  //     //     return;

  //     //   case Action::ERROR:
  //     //     // Capture state and use that to display error type
  //     //     std::cerr << "Syntax error.\n";
  //     //     return;
  //     // }

  //     // // Keep reducing while possible
  //     // while (reduce());

  //     // // If no more tokens left, exit
  //     // if (!tokens.hasNext()) {
  //     //   break;
  //     // }

  //     // // Shift the next token
  //     // shift();

  //   } while (true);
  // }

  // bool Parser::reduce() {
  //   bool reduced = false;

  //   // Scan for a possible symbol combination starting from the 10th to 1rst
  //   for (size_t len = std::min(10ull, symbols.size()); len >= 1; --len) {
  //     // Check if the symbols to be scanned are greater than the maxium
  //     // amount of symbols reduced.
  //     if (len > symbols.size()) {
  //       return false;
  //     }

  //     // Build the rule from the sequence of symbols
  //     Rule_old candidate{};
  //     for (size_t i = 0; i < len; ++i)
  //       candidate.symbols[i] = symbols[symbols.size() - len + i];

  //     // Check if the rule exists in the grammar table
  //     auto range = grammar.table.equal_range(candidate);
  //     for (auto it = range.first; it != range.second; ++it) {
  //       // Apply reduction
  //       Symbol reduced_symbol = it->second(*this);

  //       // Check if the reduced symbol is depends on a higher context
  //       // if (reduced_symbol.type == Symbol::Type::DEPENDS_ON_CONTEXT) {
  //       //   continue;
  //       // }

  //       // Remove the symbols that were reduced from the stack
  //       for (size_t i = 0; i < len; ++i) symbols.pop_back();

  //       // Push the new reduced grammar into stack
  //       symbols.push_back(reduced_symbol);
  //       reduced = true;
  //       break;
  //     }
  //   }

  //   return reduced;
  // }

  // Parser::ParserResult Parser::nextSymbol() {
  //   auto result = tokens.next();

  //   // Check if token has any defect
  //   if (!result) {
  //     return std::unexpected(
  //         ParserError{ParserErrorType::LEXER_ERROR, result.error()});
  //   }

  //   // Map the token to the specified symbol
  //   Symbol sym = {};
  //   switch (result->type) {
  //     case TokenType::KEYWORD:
  //       sym.type = Symbol::Type::KEYWORD;
  //       sym.terminal.keyword = result->value.keyword;
  //       break;
  //     case TokenType::PUNCTUATOR:
  //       sym.type = Symbol::Type::PUNCTUATOR;
  //       sym.terminal.punctuator = result->value.punctuator;
  //       break;
  //     case TokenType::IDENTIFIER:
  //       sym.type = Symbol::Type::IDENTIFIER;
  //       sym.terminal.ident_or_lit_endof = 0;
  //       break;
  //     case TokenType::CHAR_LITERAL:
  //     case TokenType::BOOL_LITERAL:
  //     case TokenType::STR8_LITERAL:
  //     case TokenType::STR16_LITERAL:
  //     case TokenType::INT8_LITERAL:
  //     case TokenType::INT16_LITERAL:
  //     case TokenType::INT32_LITERAL:
  //     case TokenType::INT64_LITERAL:
  //     case TokenType::UINT8_LITERAL:
  //     case TokenType::UINT16_LITERAL:
  //     case TokenType::UINT32_LITERAL:
  //     case TokenType::UINT64_LITERAL:
  //     case TokenType::FLOAT32_LITERAL:
  //     case TokenType::FLOAT64_LITERAL:
  //       sym.type = Symbol::Type::LITERAL;
  //       sym.terminal.ident_or_lit_endof = 1;
  //       break;
  //     case TokenType::ENDOF:
  //       sym.type = Symbol::Type::ENDOF;
  //       break;
  //     default:
  //       // This should never happen
  //       return std::unexpected(
  //           ParserError{ParserErrorType::UNKNOWN_SYMBOL, result.error()});
  //   }

  //   return sym;
  // }

  // void Parser::generateActionTable(std::vector<RuleNew>& rules,
  //                                  ActionTable& actionTable) {
  //   uint32_t state = 0;

  //   for (size_t rule_index = 0; rule_index < rules.size(); ++rule_index) {
  //     RuleNew& rule = rules[rule_index];
  //     state = 0;

  //     // For each terminal in the RHS, create a shift action
  //     for (const Symbol& symbol : rule.rhs) {
  //       if (symbol.type != Symbol::Type::NON_TERMINAL) {
  //         Action shift_action;
  //         shift_action.type = Action::SHIFT;
  //         shift_action.next_state = state++;

  //         actionTable[{state, symbol}] = shift_action;
  //       } else {
  //         Action reduce_action;
  //         reduce_action.type = Action::REDUCE;
  //         reduce_action.rule_index = rule_index;

  //         actionTable[{state, rule.lhs}] = reduce_action;
  //       }
  //     }

  //     // // Add a REDUCE action on the LHS (symbol to reduce to)
  //     // Action reduce_action;
  //     // reduce_action.type = Action::REDUCE;
  //     // reduce_action.rule_index = rule_index;

  //     // actionTable[{state, rule.lhs}] = reduce_action;
  //   }

  //   // ACCEPT on end-of-file (ENDOF)
  //   Symbol endOf;
  //   endOf.type = Symbol::Type::ENDOF;

  //   Action accept_action;
  //   accept_action.type = Action::ACCEPT;
  //   accept_action.next_state = 0;

  //   actionTable[{0, endOf}] = accept_action;
  // }
}  // namespace compiler
