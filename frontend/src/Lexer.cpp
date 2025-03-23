#include "Lexer.hpp"

#include <charconv>
#include <cmath>

/**
 * @brief Expands into an unexpected lexer error
 *
 */
#define lexerError(type, message) \
  std::unexpected(LexerError{line, pos, type, message})

namespace compiler {

  Lexer::Lexer(std::string_view src) : source(src) {}

  Lexer::LexerResult Lexer::advance() {
    // Skip all whitespace characters until we reach a non-whitespace
    // character or the end of the source code.
    skipWhitespace();

    // If we are at the end of the source code, return an EOF token
    if (pos >= source.length()) {
      EndOfFile eof = {0, line, "INSERT PATH HERE (DONT FORGET THE COLUMN)"};
      return Token{TokenType::ENDOF, eof};
    }

    char c = peek();

    // Check if the current character meets the criteria for a token
    if (isalpha(c)) {
      return makeIdentifierOrKeyword();
    } else if (isdigit(c)) {
      return makeNumericLiteral();
    } else if (c == '"') {
      return makeStringLiteral();
    } else if (isSpecialPunc(c)) {
      return makeSpecialPunc();
    } else {
      return lexerError(LexerErrorType::UNKNOWN_CHARACTER,
                        "Unknown character while scanning");
    }
  }

  Lexer::LexerResult Lexer::makeIdentifierOrKeyword() {
    uint64_t lexeme_start = pos;
    uint64_t lexeme_end = peekWhile([](char c_lex) { return isalnum(c_lex); });

    if (lexeme_end - lexeme_start > 256) {
      // Handle lexer error when identifier exceeds length
      // Throw lexer error or return an unknown token
      return lexerError(LexerErrorType::INVALID_IDENTIFIER_LENGTH,
                        "Identifier exceeds 256 characters.");
    }

    std::string_view lexeme(source.data() + lexeme_start,
                            lexeme_end - lexeme_start);

    // Try to cast it to a keyword. If it fails, it will return an unknown
    // keword meaning that it is an identifier.
    Keyword kw = KeywordHandler::from(lexeme);

    // If the lexeme is not a kewword, return it as an identifier
    if (kw == Keyword::UNDEFINED) {
      Identifier id = {unique_hash++, std::string(lexeme)};
      return Token{TokenType::IDENTIFIER, id};
    }

    return Token{TokenType::KEYWORD, kw};
  }

  Lexer::LexerResult Lexer::makeStringLiteral() {
    return Token{TokenType::UNKNOWN, {}};
  }

  Lexer::LexerResult Lexer::makeNumericLiteral() {
    uint64_t lexeme_start = pos;
    uint64_t lexeme_end = peekWhile([](char c_lex) { return isdigit(c_lex); });

    if (lexeme_end - lexeme_start > 20) {
      // Handle lexer error when numeric literal exceeds length
      // Throw lexer error or return an unknown token
      return lexerError(LexerErrorType::INVALID_NUMERIC_LITERAL_LENGTH,
                        "Numeric literal exceeds 20 digits as minimum.");
    }

    std::string_view number_literal(source.data() + lexeme_start,
                                    lexeme_end - lexeme_start);

    if (peek() == '.') {
      // Consume dot if numeric literal is a floating point
      next();

      // Cache the end of the floating point number
      uint64_t precision_end =
          peekWhile([](char c_lex) { return isdigit(c_lex); });

      std::string_view double_literal(source.data() + lexeme_start,
                                      precision_end - lexeme_start);

      if (precision_end - lexeme_start > 20) {
        // Handle lexer error when numeric literal exceeds length
        // Throw lexer error or return an unknown token
        return lexerError(
            LexerErrorType::INVALID_NUMERIC_LITERAL_LENGTH,
            "Numeric double literal exceeds 20 digits as minimum.");
      }

      auto num_result = toDouble(number_literal);

      // Pass error up if any
      if (!num_result) {
        return std::unexpected(num_result.error());
      }

      return Token{TokenType::LITERAL,
                   Literal{LiteralType::FLOAT, *num_result}};
    }

    auto num_result = toInt(number_literal);

    // Pass error up if any
    if (!num_result) {
      return std::unexpected(num_result.error());
    }

    return Token{TokenType::LITERAL,
                 Literal{LiteralType::INTEGER, *num_result}};
  }

  Lexer::LexerResult Lexer::makeSpecialPunc() {
    return Token{TokenType::UNKNOWN, {}};
  }

  char Lexer::next() {
    // Move the position to the next character in the source code
    // and return the character at the new position. This consumes
    // the character at the current position.
    return (pos < source.length()) ? source[pos++] : '\0';
  }

  char Lexer::peek() const {
    // Return the character at the current position in the source code
    // This does not consume the character at the current position.
    return (pos < source.length()) ? source[pos] : '\0';
  }

  char Lexer::peekNext() const {
    // Return the character at the next position in the source code
    // This does not consume the character at the current position.
    return (pos + 1 < source.length()) ? source[pos + 1] : '\0';
  }

  void Lexer::skipWhitespace() {
    // Skip all whitespace characters until we reach a non-whitespace
    // character or the end of the source code.
    while (peek() != '\0' && isWhitespace(peek())) {
      if (peek() == '\n') {
        line++;
      }
      pos++;
    }
  }

  uint64_t Lexer::peekWhile(Condition condition) {
    // Move the current position forward if the current character
    // meets the condition and it hasn't reached the end of file.
    while (condition(peek()) && peek() != '\0') {
      if (peek() == '\n') {
        line++;
      }
      pos++;
    }
    return pos;
  }

  bool Lexer::isWhitespace(char c) {
    switch (c) {
      case ' ':   // Space
      case '\t':  // Tab
      case '\n':  // Newline
      case '\v':  // Vertical tab
      case '\f':  // Form feed
      case '\r':  // Carriage return
        return true;
      default:
        return false;
    }
  }

  bool Lexer::isSpecialPunc(char c) {
    switch (c) {
      // Punctuators
      case '(':
      case ')':
      case '{':
      case '}':
      case '[':
      case ']':
      case ',':
      case ';':
      case ':':
      case '.':
      case '?':  // As wild card or tenerary
        return true;

      // Operators
      case '+':
      case '-':
      case '*':
      case '/':  // For division and comment
      case '%':
      case '=':
      case '&':
      case '|':
      case '^':
      case '!':
      case '<':
      case '>':
        return true;

      default:
        return false;
    }
  }

  Lexer::DParseResult Lexer::toDouble(std::string_view double_literal) const {
    double result;
    auto [ptr, ec] =
        std::from_chars(double_literal.data(),
                        double_literal.data() + double_literal.size(), result);
    if (ec != std::errc()) {
      return lexerError(LexerErrorType::PARSING_DOUBLE_ERROR,
                        "Error parsing double literal");
    }
    return result;
  }

  Lexer::IParseResult Lexer::toInt(std::string_view numeric_literal) const {
    uint64_t result = 0;

    // Check for hexadecimal (0x or 0X prefix)
    if (numeric_literal.find("0x") == 0 || numeric_literal.find("0X") == 0) {
      auto [ptr, ec] = std::from_chars(
          numeric_literal.data() + 2,
          numeric_literal.data() + numeric_literal.size(), result, 16);
      if (ec != std::errc()) {
        return lexerError(LexerErrorType::PARSING_INTEGER_ERROR,
                          "Invalid hexadecimal integer literal");
      }
    }
    // Check for octal (0 prefix)
    else if (numeric_literal.find("0") == 0 && numeric_literal.size() > 1 &&
             numeric_literal[1] != 'x' && numeric_literal[1] != 'X') {
      auto [ptr, ec] = std::from_chars(
          numeric_literal.data() + 1,
          numeric_literal.data() + numeric_literal.size(), result, 8);
      if (ec != std::errc()) {
        return lexerError(LexerErrorType::PARSING_INTEGER_ERROR,
                          "Invalid octal integer literal");
      }
    }
    // Check for binary (0b or 0B prefix)
    else if (numeric_literal.find("0b") == 0 ||
             numeric_literal.find("0B") == 0) {
      auto [ptr, ec] = std::from_chars(
          numeric_literal.data() + 2,
          numeric_literal.data() + numeric_literal.size(), result, 2);
      if (ec != std::errc()) {
        return lexerError(LexerErrorType::PARSING_INTEGER_ERROR,
                          "Invalid binary integer literal");
      }
    }
    // Default to decimal parsing
    else {
      auto [ptr, ec] = std::from_chars(
          numeric_literal.data(),
          numeric_literal.data() + numeric_literal.size(), result, 10);
      if (ec != std::errc()) {
        return lexerError(LexerErrorType::PARSING_INTEGER_ERROR,
                          "Invalid decimal integer literal");
      }
    }

    return result;
  }
}  // namespace compiler