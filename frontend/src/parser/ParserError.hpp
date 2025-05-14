#pragma once

#include <optional>
#include <string_view>
#include <vector>

#include "ActionTable.hpp"
#include "lexer/LexerError.hpp"

namespace compiler {

  enum class ParserErrorType {
    UNKNOWN_ERROR,      // Catch-all for unexpected issues
    UNKNOWN_SYMBOL,     // Unknown symbol passed by lexer
    LEXER_ERROR,        // Lexer Error when reading token
    UNEXPECTED_SYMBOL,  // Unexpected symbol when parsing
    INCOMPLETE_ACCEPT,  // Passes an incomplete AST to be accepted
  };

  class ParserError final {
  public:
    ParserErrorType type;

    Symbol unex_symbol;
    uint32_t ex_symbol;
    std::vector<Symbol> ex_rhs;
    std::optional<LexerError> lexer_error;

  public:
    /**
     * @brief Parses the error type into a string literal
     *
     * @return std::string
     */
    std::string_view to_string();
  };

}  // namespace compiler
