#pragma once

#include <expected>
#include <string_view>

#include "LexerError.hpp"
#include "tokens/Tokens.hpp"

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
     * @brief Advances to the next token in the source code.
     * @return LexerResult contining the next token or a lexer error.
     */
    LexerResult advance();

    /**
     * @brief Prints token for debugging.
     *
     * @param token
     */
    void debugPrintToken(const Token& token) noexcept;

  private:
    uint32_t pos;
    uint32_t line;
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
    char next() noexcept;
    char peek() const noexcept;
    char peekNext() const noexcept;
    void skipWhitespace() noexcept;
    void skipLineComment() noexcept;
    void skipBlockComments() noexcept;

  private:
    bool isBasicPunc(char c) const noexcept;
    bool isWhitespace(char c) const noexcept;
    bool isEscapedChar(char c) const noexcept;
    bool isValidBaseNumber(char c, uint8_t base) const noexcept;

    TokenType typeSufixFrom() noexcept;
    uint8_t basePrefixFrom(uint32_t& lex_start) noexcept;

    char toEscapedChar(char c) const noexcept;
    IParseResult toInt(std::string_view integer_literal, uint8_t base) const;
    DParseResult toFloat(std::string_view float_literal) const;

  private:
    // Move the current position forward if the current character
    // meets the condition and it hasn't reached the end of file.
    template <typename Condition>
    uint32_t nextWhile(Condition&& condition) noexcept {
      while (condition(peek()) && peek() != '\0') {
        if (peek() == '\n') line++;
        pos++;
      }
      return pos;
    }
  };

}  // namespace compiler
