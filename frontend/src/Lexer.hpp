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

    using DParseResult = std::expected<double, LexerError>;
    using IParseResult = std::expected<uint64_t, LexerError>;

  private:
    LexerResult makeSymbol();
    LexerResult makeNumberL();
    LexerResult makeStringL();
    LexerResult makeCharacterL();
    LexerResult makeSpecialPunc();
    LexerResult makePunctuatorComment();

  private:
    char next();
    char peek() const;
    char peekNext() const;
    void skipWhitespace();

  private:
    bool isWhitespace(char c) const;
    bool isSpecialPunc(char c) const;
    bool isEscapedChar(char c) const;

    char toEscapedChar(char c) const;
    IParseResult toInt(std::string_view numeric_literal, uint8_t base) const;
    DParseResult toDouble(std::string_view double_literal) const;

  private:
    // Move the current position forward if the current character
    // meets the condition and it hasn't reached the end of file.
    template <typename Condition>
    uint64_t peekWhile(Condition&& condition) {
      while (condition(peek()) && peek() != '\0') {
        if (peek() == '\n') line++;
        pos++;
      }
      return pos;
    }
  };

}  // namespace compiler
