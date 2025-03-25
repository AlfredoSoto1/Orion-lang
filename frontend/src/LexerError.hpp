#pragma once

#include <string>

namespace compiler {

  enum class LexerErrorType {
    MISSING_QUOTE,
    MISSING_DOUBLE_QUOTE,
    TOO_MANY_CHARS,
    UNTERMINATED_STRING,
    PARSING_DOUBLE_ERROR,
    PARSING_INTEGER_ERROR,
    INVALID_IDENTIFIER_LENGTH,
    UNKNOWN_CHARACTER,
  };

  class LexerError {
  public:
    uint64_t line;
    uint64_t pos;
    LexerErrorType type;
    std::string_view message;

  public:
    /**
     * @brief Parses the error type into a string literal
     *
     * @param type
     * @return std::string_view
     */
    static std::string_view from(LexerErrorType type);
  };
}  // namespace compiler
