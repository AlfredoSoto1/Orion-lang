#include "ParserError.hpp"

#include <iomanip>
#include <sstream>

namespace compiler {

  std::unexpected<ParserError> ParserError::makeLexerError(LexerError error) {
    return std::unexpected(ParserError{.type = ParserErrorType::LEXER_ERROR,
                                       .lexer_error = error});
  }

  std::unexpected<ParserError> ParserError::makeUnknownSymbolError() {
    return std::unexpected(
        ParserError{.type = ParserErrorType::UNKNOWN_SYMBOL});
  }

  std::unexpected<ParserError> ParserError::makeUnknownError() {
    return std::unexpected(ParserError{.type = ParserErrorType::UNKNOWN_ERROR});
  }

  std::unexpected<ParserError> ParserError::makeInternalError() {
    return std::unexpected(
        ParserError{.type = ParserErrorType::INTERNAL_ERROR});
  }

  std::unexpected<ParserError> ParserError::makeUnexSymbolError(
      Symbol found, LexerState lexer_state, std::vector<Symbol> expected,
      std::vector<std::vector<Symbol>> expected_rhs) {
    return std::unexpected(ParserError{
        .type = ParserErrorType::UNEXPECTED_SYMBOL,
        .lexer_error = std::nullopt,
        .unex_symbol_error =
            UnexpectedSymbolError{.found = found,
                                  .lexer_state = lexer_state,
                                  .expected = std::move(expected),
                                  .expected_rhs = std::move(expected_rhs)}});
  }

  std::string ParserError::toString() const noexcept {
    std::ostringstream oss;

    if (type == ParserErrorType::UNEXPECTED_SYMBOL) {
      const auto& err = *unex_symbol_error;
      const auto& lexer_state = err.lexer_state;

      // Error header
      oss << "error: unexpected symbol `" << err.found.toString() << "`\n";

      // Source location
      oss << " --> " << lexer_state.filename << ":" << lexer_state.line << ":"
          << lexer_state.column << "\n";

      // Code snippet
      oss << std::setw(4) << "" << " |\n";
      oss << std::setw(4) << lexer_state.line << " | " << lexer_state.line_text
          << '\n';
      oss << std::setw(4) << "" << " | "
          << std::string(lexer_state.column - 1, ' ') << "^\n";

      // Expected symbols
      if (!err.expected.empty()) {
        oss << "  = expected one of: ";
        for (size_t i = 0; i < err.expected.size(); ++i) {
          oss << "`" << err.expected[i].toString() << "`";
          if (i + 1 < err.expected.size()) oss << ", ";
        }
        oss << '\n';
      }

      // Example of valid syntax
      if (!err.expected_rhs.empty()) {
        oss << "  = note: examples of valid syntax:\n";
        for (const auto& rhs : err.expected_rhs) {
          oss << "    -> ";
          for (const auto& sym : rhs) oss << sym.toString() << " ";
          oss << "\n";
        }
      }

    } else if (type == ParserErrorType::LEXER_ERROR) {
      oss << "error: lexer error: " << lexer_error->toString() << '\n';
    } else if (type == ParserErrorType::UNKNOWN_SYMBOL) {
      oss << "error: unknown symbol encountered\n";
    } else if (type == ParserErrorType::UNKNOWN_ERROR) {
      oss << "error: unknown parser error\n";
    } else if (type == ParserErrorType::INTERNAL_ERROR) {
      oss << "error: internal parser error\n";
    }

    return oss.str();
  }

}  // namespace compiler
