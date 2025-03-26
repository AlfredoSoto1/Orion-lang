#pragma once

#include <expected>
#include <string_view>

#include "LexerError.hpp"
#include "Tokens.hpp"

namespace compiler {

  /**
   * @class Lexer
   * @brief Responsible for lexical analysis, converting source code into
   *        tokens.
   */
  class Lexer final {
  public:
    using LexerResult = std::expected<Token, LexerError>;

  public:
    /**
     * @brief Constructs a lexer with the given source code.
     * @param src The source code to tokenize.
     */
    explicit Lexer(std::string_view src) noexcept;

    /**
     * @brief Generates a stream of tokens from the source code. This is
     *        generaly used if you want to divide the compiler phases into
     *        stages.
     * @return TokenStream The stream of parsed tokens.
     */
    TokenStream stream();

    /**
     * @brief Advances to the next token in the source code.
     * @return LexerResult The next token or a lexer error.
     */
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
    LexerResult makeCharLiteral();
    LexerResult makeNumberLiteral();
    LexerResult makeStringLiteral();
    LexerResult makePunctuator();

  private:
    char next();
    char peek() const;
    char peekNext() const;
    void skipWhitespace();
    void skipLineComment();
    void skipBlockComments();

  private:
    bool isWhitespace(char c) const;
    bool isPunctuator(char c) const;
    bool isEscapedChar(char c) const;
    bool isValidBaseNumber(char c, uint8_t base) const;

    char toEscapedChar(char c) const;
    IParseResult toInt(std::string_view integer_literal, uint8_t base) const;
    DParseResult toFloat(std::string_view float_literal) const;

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
