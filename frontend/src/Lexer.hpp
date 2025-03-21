#pragma once

namespace compiler {

  enum class TokenType {
    FIRST,
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    SYMBOL,
    STRING,
    COMMENT,
    UNKNOWN,
    ENDOF,
  };

}  // namespace compiler
