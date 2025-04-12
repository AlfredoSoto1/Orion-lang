#pragma once

#include <string>

#include "Keyword.hpp"
#include "Operators.hpp"
#include "Punctuator.hpp"

namespace compiler {

  struct string_view {
    uint32_t start;
    uint32_t end;
  };

  /**
   * @brief Represents an identifier in the source code.
   *
   */
  struct Identifier {
    std::string_view name;
  };

  /**
   * @brief
   *
   */
  struct EndOfFile {
    uint32_t line;
    uint32_t column;
  };

  /**
   * @brief
   *
   */
  union Literal {
    uint64_t integer;         // Unsigned 64-bit integer
    double floating;          // Double precision floating point
    char character;           // Single Byte character
    bool boolean;             // Boolean
    std::string_view string;  // String
  };

  /**
   * @brief
   *
   */
  enum class TokenType : uint8_t {
    UNKNOWN = 0,
    KEYWORD,
    IDENTIFIER,
    PUNCTUATOR,
    CHAR_LITERAL,
    STRING_LITERAL,
    BOOLEAN_LITERAL,
    INTEGER_LITERAL,
    FLOATING_LITERAL,
    COMMENT,
    ENDOF,
  };

  /**
   * @brief Represents a token value.
   *
   * This is a variant type that can hold different types of values
   * that can be associated with a token, such as keywords, literals,
   * identifiers, punctuators, and end-of-file markers.
   *
   */
  union TokenValue {
    Keyword keyword;        // Keyword
    Literal literal;        // Literal
    Identifier identifier;  // Identifier
    Punctuator punctuator;  // Punctuator
    EndOfFile eof;          // End of file
  };

  /**
   * @brief Represents a token in the source code.
   *
   */
  struct Token {
    TokenType type;
    TokenValue value;
  };

  /**
   * @brief
   *
   */
  struct MetaToken {
    Token* token;
    uint32_t line;
    uint32_t column;
  };
}  // namespace compiler