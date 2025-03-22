#pragma once

#include <string>
#include <vector>

#include "Tokens.hpp"

namespace compiler {

  class LexerError {};

  class Lexer {
  public:
    Lexer(const std::string& src);

    TokenStream stream();

  private:
    uint64_t pos = 0;
    uint64_t line = 0;
    uint64_t unique_hash = 0;
    const std::string& source;

  private:
    char next();
    char peek() const;
    char peekNext() const;

    Token advance();

  private:
    void skipWhitespace();
    bool isWhitespace(char c);
    bool isSpecialPunc(char c);

  private:
    Token makeIdentifierOrKeyword();
    Token makeStringLiteral();
    Token makeNumericLiteral();
    Token makeSpecialPunc();
    Token makePunctuatorComment();
  };

}  // namespace compiler
