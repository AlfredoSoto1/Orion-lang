#pragma once

#include <string_view>

#include "Keyword.hpp"
#include "Operators.hpp"
#include "Punctuator.hpp"

namespace compiler {

  /**
   * @brief Represents an identifier in the source code.
   *        The max amount of characters that the identifier can be is 2^16.
   *
   */
  struct Identifier {
    uint16_t start;
    uint16_t end;

    std::string_view view(std::string_view source) const {
      return source.substr(start, end - start);
    }
  };

  /**
   * @brief Represents the end of file after tokenizing.
   *
   */
  struct EndOfFile {
    uint32_t line;
    uint32_t column;
  };

  /**
   * @brief Holds a reference to the start and end of a string literal in the
   *        code-text. The maximum amount of characters that a string literal
   *        can hold is 2^32.
   *
   */
  struct string_lit {
    uint32_t start;
    uint32_t end;

    std::string_view view(std::string_view source) const {
      return source.substr(start, end - start);
    }
  };

  /**
   * @brief Holds the data of the literal.
   *
   */
  union Literal {
    bool boolean;       // Boolean
    char character;     // Single Byte character
    double floating;    // Double precision floating point
    uint64_t integer;   // Unsigned 64-bit integer
    string_lit string;  // String literal
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
    BOOL_LITERAL,
    STR8_LITERAL,
    STR16_LITERAL,
    INT8_LITERAL,
    INT16_LITERAL,
    INT32_LITERAL,
    INT64_LITERAL,
    UINT8_LITERAL,
    UINT16_LITERAL,
    UINT32_LITERAL,
    UINT64_LITERAL,
    FLOAT32_LITERAL,
    FLOAT64_LITERAL,
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
   * @brief Representation of a token. This holds the value and the type of what
   *        it is.
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