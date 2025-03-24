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
    } else if (c == '\'') {
      return makeCharLiteral();
    } else if (isSpecialPunc(c)) {
      return makeSpecialPunc();
    } else {
      return lexerError(LexerErrorType::UNKNOWN_CHARACTER,
                        "Unknown character while scanning");
    }
  }

  Lexer::LexerResult Lexer::makeIdentifierOrKeyword() {
    uint64_t lexeme_start = pos;
    uint64_t lexeme_end =
        peekWhile([](char c_peek) { return isalnum(c_peek) || c_peek == '_'; });

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
    next();  // consume starting "

    std::string string_literal;
    bool balanced = false;
    bool in_escape_mode = false;

    while (peek() != '\0') {
      char c = next();

      bool escaped = isEscapedChar(c) && in_escape_mode;

      in_escape_mode = !escaped && c == '\\';
      if (in_escape_mode) {
        continue;
      }

      if (escaped) {
        c = toEscapedChar(c);
      }

      // If we encounter an unescaped ", we've reached the end
      if (!escaped && c == '"') {
        balanced = true;
        break;
      }

      string_literal.push_back(c);
    }

    if (!balanced) {
      return lexerError(LexerErrorType::UNTERMINATED_STRING,
                        "Missing closing quote.");
    }

    return Token{TokenType::LITERAL,
                 Literal{LiteralType::STRING, string_literal}};
  }

  Lexer::LexerResult Lexer::makeCharLiteral() {
    // consume starting '
    next();

    if (peek() == '\'') {
      return lexerError(LexerErrorType::UNTERMINATED_STRING,
                        "Quoted char should contain at least one character.");
    }

    // consume and cache the char
    char c = next();

    if (peek() != '\'') {
      return lexerError(LexerErrorType::UNTERMINATED_STRING,
                        "Too many characters in char literal.");
    }

    // consume last '
    next();

    return Token{TokenType::LITERAL, Literal{LiteralType::CHAR, c}};
  }

  Lexer::LexerResult Lexer::makeNumericLiteral() {
    uint8_t base = 10;
    uint64_t lexeme_start = pos;

    // Check for hexadecimal, octal, or binary prefix
    if (peek() == '0') {
      char next_char = peekNext();
      if (next_char == 'x' || next_char == 'X') {
        // Consume 0x
        next();
        next();
        base = 16;
        lexeme_start += 2;
      } else if (next_char == 'b' || next_char == 'B') {
        // Consume 0b
        next();
        next();
        base = 2;
        lexeme_start += 2;
      } else if (isdigit(next_char)) {
        // Consume 0
        next();
        base = 8;
        lexeme_start += 1;
      }
    }

    // Capture the numeric part
    uint64_t lexeme_end = peekWhile(
        [base](char c) { return base == 16 ? isalnum(c) : isdigit(c); });

    std::string_view number_literal(source.data() + lexeme_start,
                                    lexeme_end - lexeme_start);

    // Handle floating-point numbers (only for decimal)
    if (peek() == '.') {
      next();  // Consume '.'
      uint64_t precision_end =
          peekWhile([](char c) { return (bool)isdigit(c); });
      std::string_view double_literal(source.data() + lexeme_start,
                                      precision_end - lexeme_start);

      auto num_result = toDouble(double_literal);
      if (!num_result) return std::unexpected(num_result.error());
      return Token{TokenType::LITERAL,
                   Literal{LiteralType::FLOAT, *num_result}};
    }

    // Convert integer based on detected base
    auto num_result = toInt(number_literal, base);
    if (!num_result) return std::unexpected(num_result.error());
    return Token{TokenType::LITERAL,
                 Literal{LiteralType::INTEGER, *num_result}};
  }

  Lexer::LexerResult Lexer::makeSpecialPunc() {
    uint64_t punc_start = pos;
    uint64_t punc_end = peekWhile([this](char p) { return isSpecialPunc(p); });

    std::string_view punc_view(source.data() + punc_start,
                               punc_end - punc_start);

    Punctuator punctuator = PunctuatorHandler::from(punc_view);
    return Token{TokenType::PUNCTUATOR, punctuator};
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
      if (peek() == '\n') line++;
      pos++;
    }
  }

  bool Lexer::isWhitespace(char c) const {
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

  bool Lexer::isSpecialPunc(char c) const {
    switch (c) {
      // Punctuators
      case '@':
      case '#':
      case '$':
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
      case '?':
      case '+':
      case '-':
      case '*':
      case '/':
      case '%':
      case '=':
      case '&':
      case '|':
      case '^':
      case '!':
      case '<':
      case '>':
      case '~':
        return true;

      default:
        return false;
    }
  }

  bool Lexer::isEscapedChar(char c) const {
    return c == 'n' || c == 'r' || c == '"' || c == '\'' || c == '0' ||
           c == '\\';
  }

  char Lexer::toEscapedChar(char c) const {
    switch (c) {
      case 'n':
        return '\n';
      case 'r':
        return '\r';
      case '"':
        return '\"';
      case '\'':
        return '\'';
      case '0':
        return '\0';
      case '\\':
        return '\\';
      default:
        return c;
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

  Lexer::IParseResult Lexer::toInt(std::string_view numeric_literal,
                                   uint8_t base) const {
    uint64_t result = 0;
    auto [ptr, ec] = std::from_chars(
        numeric_literal.data(), numeric_literal.data() + numeric_literal.size(),
        result, base);
    if (ec != std::errc()) {
      return lexerError(LexerErrorType::PARSING_INTEGER_ERROR,
                        "Invalid integer literal");
    }
    return result;
  }
}  // namespace compiler