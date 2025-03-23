#pragma once

#include <expected>
#include <string>
#include <vector>

#include "Tokens.hpp"

namespace compiler {

  enum class LexerErrorType {
    PARSING_DOUBLE_ERROR,
    PARSING_INTEGER_ERROR,
    INVALID_IDENTIFIER_LENGTH,
    INVALID_NUMERIC_LITERAL_LENGTH,
    UNKNOWN_CHARACTER,
    UNTERMINATED_STRING,
  };

  struct LexerError {
    uint64_t line;
    uint64_t pos;
    LexerErrorType type;
    std::string message;
  };

  class Lexer {
  public:
    using LexerResult = std::expected<Token, LexerError>;

  public:
    Lexer(std::string_view src);

    TokenStream stream();

    LexerResult advance();

  private:
    uint64_t pos = 0;
    uint64_t line = 0;
    uint64_t unique_hash = 0;
    std::string_view source;

    using Condition = int (*)(char);
    using DParseResult = std::expected<double, LexerError>;
    using IParseResult = std::expected<uint64_t, LexerError>;

  private:
    LexerResult makeIdentifierOrKeyword();
    LexerResult makeStringLiteral();
    LexerResult makeNumericLiteral();
    LexerResult makeSpecialPunc();
    LexerResult makePunctuatorComment();

  private:
    char next();
    char peek() const;
    char peekNext() const;

  private:
    void skipWhitespace();
    uint64_t peekWhile(Condition condition);

    bool isWhitespace(char c);
    bool isSpecialPunc(char c);

    IParseResult toInt(std::string_view numeric_literal) const;
    DParseResult toDouble(std::string_view double_literal) const;
  };

}  // namespace compiler
