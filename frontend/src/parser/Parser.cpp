#include "Parser.hpp"

#include <iostream>

namespace compiler {

  Parser::Parser(TokenStream& tokens, const Grammar& grammar) noexcept
      : tokens(tokens), action_table(grammar), grammar(grammar), symbols() {}

  Parser::ParserResult Parser::parse() {
    // Prepare parse stack and first lookahead symbol
    SymbolResult lookahead = nextSymbol();

    // Set an initial symbol to start parsing
    symbols.push({Symbol::start(), 0});

    while (true) {
      // Validate that the symbol is not garbage.
      if (!lookahead) {
        return triggerError(lookahead.error());
      }

      // Obtain action from the current state and symbol
      const auto& sym_st = symbols.peekTop();
      Action action = action_table.actionFrom({sym_st.state, *lookahead});

      switch (action.type) {
        case Action::SHIFT: {
          // Shift the current symbol and push to the next state.
          // Advance to the next symbol.
          symbols.push({*lookahead, action.next_state});
          lookahead = nextSymbol();
          break;
        }

        case Action::REDUCE: {
          // Capture the rhs of the grammar rule to reduce
          const Rule& r = grammar[action.rule_index];

          // Build the AST and pick its index position from Program's AST.

          // Pop from stack the rhs symbols that cause a reduction
          symbols.pop(r.rhs.size());

          // Peek the latest symbol-state and push the goto state from reduced
          // symbol expression.
          const auto& sym_st = symbols.peekTop();
          Symbol sym{.type = Symbol::Type::NON_TERMINAL, .nonterminal = r.lhs};
          symbols.push({sym, action_table.gotoFrom({sym_st.state, sym})});
          break;
        }

        case Action::ACCEPT: {
          // If the stack has more than 2 elements (start and the final symbol)
          // it means that the parsing process was not completed correctly, and
          // it is a false accept that it was passed.
          if (symbols.size() > 2) {
            return triggerError(std::nullopt);
          }

          // Return a new result containing the ProgramAST
          std::cout << "Parsing successful.\n";
          return ASTProgram{};
        }

        case Action::ERROR:
          return triggerError(std::nullopt);
      }
    }
  }

  Parser::SymbolResult Parser::nextSymbol() {
    // Consume token and move to next
    auto result = tokens.next();

    // Check if token has any defect
    if (!result) {
      return std::unexpected(ParserError{.type = ParserErrorType::LEXER_ERROR,
                                         .lexer_error = result.error()});
    }

    // Map the token to the specified symbol
    Symbol sym = {};
    switch (result->type) {
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
      case TokenType::IDENTIFIER:
        return Symbol::identifier();
      case TokenType::ENDOF:
        return Symbol::endOF();
      case TokenType::KEYWORD:
        sym.type = Symbol::Type::KW_TERMINAL;
        sym.terminal.keyword = result->value.keyword;
        return sym;
      case TokenType::PUNCTUATOR:
        sym.type = Symbol::Type::PUN_TERMINAL;
        sym.terminal.punctuator = result->value.punctuator;
        return sym;
      default:
        // This should never happen
        return std::unexpected(
            ParserError{ParserErrorType::UNKNOWN_SYMBOL, {}});
    }
  }

  Parser::ParserResult Parser::triggerError(std::optional<ParserError> error) {
    // If the given parameter has an error, return that as
    // main error first.
    if (error.has_value()) {
      return std::unexpected(*error);
    }

    const auto& top = symbols.peekTop();

    std::vector<Symbol> expected_symbols;
    std::vector<std::vector<Symbol>> expected_rhs;

    for (const Symbol& s : grammar.symbols) {
      if (!s.isTerminal()) continue;

      Action a = action_table.actionFrom({top.state, s});
      if (a.type == Action::ERROR) continue;

      expected_symbols.push_back(s);

      if (a.type == Action::REDUCE) {
        const Rule& rule = grammar[a.rule_index];
        expected_rhs.push_back(rule.rhs);
      } else if (a.type == Action::SHIFT) {
        // Optional: Add a special case for SHIFT
        expected_rhs.push_back({s});
      } else if (a.type == Action::ACCEPT) {
        // Accept usually corresponds to an empty RHS or final rule
        // expected_rhs.emplace_back();
      }
    }

    return std::unexpected(
        ParserError{.type = ParserErrorType::UNEXPECTED_SYMBOL,
                    .unex_symbol = current_lookahead,
                    .expected_symbols = std::move(expected_symbols),
                    .expected_rhs = std::move(expected_rhs),
                    .lexer_error = std::nullopt});
  }

}  // namespace compiler
