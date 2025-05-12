#include "Parser.hpp"

#include <iostream>

namespace compiler {

  Parser::Parser(TokenStream& tokens, const Grammar& grammar) noexcept
      : tokens(tokens), action_table(grammar), grammar(grammar) {
    // Build action table
    action_table.build();
  }

  void Parser::parse() {
    std::vector<Symbol> symbols;
    std::vector<ActionTable::State> states;

    // Initial conditions
    size_t pos = 0;
    states.push_back(0);
    ParserResult lookahead = nextSymbol();

    while (true) {
      // Validate that the symbol is not garbage.
      if (!lookahead) {
        std::cerr << "Symbol error.\n";
        return;  // Error
      }

      ActionTable::State state = states.back();
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
          const Rule& r = grammar[action.rule_index];
          for (size_t i = 0; i < r.rhs.size(); ++i) {
            states.pop_back();
            symbols.pop_back();
          }
          ActionTable::State t = states.back();

          Symbol sym;
          sym.type = Symbol::Type::NON_TERMINAL;
          sym.nonterminal = r.lhs;
          states.push_back(action_table.stateFrom({t, sym}));
          symbols.push_back(sym);
          break;
        }

        case Action::ACCEPT:
          std::cout << "Parsing successful.\n";
          return;

        case Action::ERROR:
          // Capture state and use that to display error type
          std::cerr << "Syntax error." << std::endl;
          return;
      }
    }
  }

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
        return sym;
      case TokenType::PUNCTUATOR:
        sym.type = Symbol::Type::PUN_TERMINAL;
        sym.terminal.punctuator = result->value.punctuator;
        return sym;
      case TokenType::IDENTIFIER:
        return Symbol::identifier();
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
        return Symbol::literal();
      case TokenType::ENDOF:
        return Symbol::endOF();
      default:
        // This should never happen
        return std::unexpected(
            ParserError{ParserErrorType::UNKNOWN_SYMBOL, result.error()});
    }
  }

}  // namespace compiler
