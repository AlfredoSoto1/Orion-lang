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
  using EndOfFile = bool;

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

    std::string toString(std::string_view source) const noexcept;

    static inline Token endOF() noexcept {
      return Token{.type = TokenType::ENDOF, .value = {.eof = true}};
    }

    static inline Token identifier(size_t start, size_t end) noexcept {
      return Token{
          .type = TokenType::IDENTIFIER,
          .value = {.identifier = {.start = static_cast<uint16_t>(start),
                                   .end = static_cast<uint16_t>(end)}}};
    }

    static inline Token keyword(Keyword keyword) noexcept {
      return Token{.type = TokenType::KEYWORD, .value = {.keyword = keyword}};
    }

    static inline Token boolean(bool boolean) noexcept {
      return Token{.type = TokenType::BOOL_LITERAL,
                   .value = {.literal = {.boolean = boolean}}};
    }

    static inline Token character(char character) noexcept {
      return Token{.type = TokenType::CHAR_LITERAL,
                   .value = {.literal = {.character = character}}};
    }

    static inline Token string(string_lit string) noexcept {
      return Token{.type = TokenType::STR8_LITERAL,
                   .value = {.literal = {.string = string}}};
    }

    static inline Token punctuator(Punctuator punctuator) noexcept {
      return Token{.type = TokenType::PUNCTUATOR,
                   .value = {.punctuator = punctuator}};
    }

    static inline Token number(TokenType type, uint64_t integer) noexcept {
      return Token{.type = type, .value = {.literal = {.integer = integer}}};
    }
  };
}  // namespace compiler