#pragma once

#include <expected>
#include <optional>
#include <string>
#include <vector>

#include "ActionTable.hpp"
#include "lexer/LexerError.hpp"

namespace compiler {

  enum class ParserErrorType {
    UNKNOWN_ERROR,      // Catch-all for unexpected issues
    UNKNOWN_SYMBOL,     // Unknown symbol passed by lexer
    LEXER_ERROR,        // Lexer Error when reading token
    UNEXPECTED_SYMBOL,  // Unexpected symbol when parsing
    INTERNAL_ERROR,     // Passes an incomplete AST to be accepted
  };

  struct UnexpectedSymbolError final {
    Symbol found;
    LexerState lexer_state;
    std::vector<Symbol> expected;
    std::vector<std::vector<Symbol>> expected_rhs;
  };

  class ParserError final {
  public:
    ParserErrorType type;

    std::optional<LexerError> lexer_error;
    std::optional<UnexpectedSymbolError> unex_symbol_error;

  public:
    /**
     * @brief Parses the error type into a string literal
     *
     * @return std::string
     */
    std::string toString() const noexcept;

  public:
    /**
     * @brief Creates a ParserError from a LexerError
     *
     * @param error
     * @return std::unexpected<ParserError>
     */
    static std::unexpected<ParserError> makeLexerError(LexerError error);

    /**
     * @brief Creates a ParserError for an unknown symbol.
     *
     * @return std::unexpected<ParserError>
     */
    static std::unexpected<ParserError> makeUnknownSymbolError();

    /**
     * @brief Creates a ParserError for an unknown error.
     *
     * @return std::unexpected<ParserError>
     */
    static std::unexpected<ParserError> makeUnknownError();

    /**
     * @brief Creates a ParserError for an internal error.
     *
     * @return std::unexpected<ParserError>
     */
    static std::unexpected<ParserError> makeInternalError();

    /**
     * @brief Creates a ParserError for an unexpected symbol.
     *
     * @param found
     * @param lexer_state
     * @param expected
     * @param expected_rhs
     * @return std::unexpected<ParserError>
     */
    static std::unexpected<ParserError> makeUnexSymbolError(
        Symbol found, LexerState lexer_state, std::vector<Symbol> expected,
        std::vector<std::vector<Symbol>> expected_rhs);
  };

}  // namespace compiler
