#include "Parser.hpp"

#include <iostream>

namespace compiler {

  Parser::Parser(TokenStream& tokens, const Grammar& grammar) noexcept
      : tokens(tokens), action_table(grammar) {}

  void Parser::parse() {
    std::vector<Symbol> symbols;
    std::vector<uint32_t> states;

    // Initial conditions
    states.push_back(0);
    ParserResult lookahead = nextSymbol();

    while (true) {
      // Validate that the symbol is not garbage.
      if (lookahead) {
        return;  // Error
      }

      uint32_t state = states.back();
      Action action = action_table.actionFrom({state, *lookahead});

      switch (action.type) {
        case Action::SHIFT: {
          // Shift the current symbol to stack and push to the next state.
          symbols.push_back(*lookahead);
          states.push_back(action.next_state);

          // Advance to the next symbol.
          lookahead = nextSymbol();
          break;
        }

        case Action::REDUCE: {
          break;
        }

        case Action::ACCEPT:
          std::cout << "Parsing successful.\n";
          return;

        case Action::ERROR:
          // Capture state and use that to display error type
          std::cerr << "Syntax error.\n";
          return;
      }
    }
  }

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

  Parser::ParserResult Parser::nextSymbol() {
    auto result = tokens.next();

    // Check if token has any defect
    if (!result) {
      return std::unexpected(
          ParserError{ParserErrorType::LEXER_ERROR, result.error()});
    }

    // Map the token to the specified symbol
    Symbol sym = {};
    switch (result->type) {
      case TokenType::KEYWORD:
        sym.type = Symbol::Type::KW_TERMINAL;
        sym.terminal.keyword = result->value.keyword;
        break;
      case TokenType::PUNCTUATOR:
        sym.type = Symbol::Type::PUN_TERMINAL;
        sym.terminal.punctuator = result->value.punctuator;
        break;
      case TokenType::IDENTIFIER:
        sym.type = Symbol::Type::ID_TERMINAL;
        sym.terminal.identifier = 0;
        break;
      case TokenType::CHAR_LITERAL:
      case TokenType::BOOL_LITERAL:
      case TokenType::STR8_LITERAL:
      case TokenType::STR16_LITERAL:
      case TokenType::INT8_LITERAL:
      case TokenType::INT16_LITERAL:
      case TokenType::INT32_LITERAL:
      case TokenType::INT64_LITERAL:
      case TokenType::UINT8_LITERAL:
      case TokenType::UINT16_LITERAL:
      case TokenType::UINT32_LITERAL:
      case TokenType::UINT64_LITERAL:
      case TokenType::FLOAT32_LITERAL:
      case TokenType::FLOAT64_LITERAL:
        sym.type = Symbol::Type::LIT_TERMINAL;
        sym.terminal.literal = 1;
        break;
      case TokenType::ENDOF:
        sym.type = Symbol::Type::EOF_TERMINAL;
        break;
      default:
        // This should never happen
        return std::unexpected(
            ParserError{ParserErrorType::UNKNOWN_SYMBOL, result.error()});
    }

    return sym;
  }

}  // namespace compiler
