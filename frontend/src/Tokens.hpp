#pragma once

#include <optional>
#include <variant>

#include "Keyword.hpp"
#include "Operators.hpp"
#include "Punctuator.hpp"

namespace compiler {

  /**
   * @brief Represents an identifier in the source code.
   *
   */
  struct Identifier {
    uint64_t uid;
    std::string name;
  };

  enum class LiteralType {
    INTEGER,
    FLOAT,
    CHAR,
    STRING,
  };

  struct EndOfFile {
    uint64_t line;
    uint64_t column;
    std::string path;
  };

  using LiteralValue = std::variant<char,    // Single Byte character
                                    double,  // Double precision floating point
                                    uint64_t,      // Unsigned 64-bit integer
                                    int64_t,       // Signed 64-bit integer
                                    std::string>;  // String

  struct Literal {
    LiteralType type;
    std::variant<LiteralValue> value;
  };

  enum class TokenType {
    FIRST,
    KEYWORD,
    LITERAL,
    IDENTIFIER,
    PUNCTUATOR,
    OPERATOR,
    COMMENT,
    ENDOF,
    UNKNOWN,
  };

  using TokenValue = std::variant<Keyword,     // Keyword
                                  Literal,     // Literal
                                  Identifier,  // Identifier
                                  Punctuator,  // Punctuator
                                  Operator,    // Operator
                                  EndOfFile,   // End of file
                                  uint8_t>;    // Unknown error code

  /**
   * @brief Represents a token in the source code.
   *
   */
  struct Token {
    TokenType type;
    std::optional<TokenValue> value;
  };

  struct MetaToken {
    Token* token;
    size_t line;
    size_t column;
  };

  class TokenStream {
  public:
    TokenStream(uint8_t buffer_size);

    const Token& next();
    const Token& current() const;

    const Token& peek() const;
    const Token& peekNext() const;

    bool hasNext() const;

  private:
    uint8_t buffer_size;
  };

}  // namespace compiler