#include "Lexer.hpp"

#include <cctype>
#include <charconv>

/**
 * @brief Expands into an unexpected lexer error
 *
 */
#define lexerError(type) std::unexpected(LexerError{line, pos, type})

namespace compiler {

  Lexer::Lexer(std::string_view src) noexcept
      : pos(0), line(0), unique_hash(0), source(src) {}

  Lexer::LexerResult Lexer::advance() {
    // Skip all whitespace characters until we reach a non-whitespace
    // character or the end of the source code.
    skipWhitespace();

    // If we are at the end of the source code, return an EOF token
    if (pos >= source.length()) {
      Token tok{TokenType::ENDOF};
      tok.value.eof = {0, line};  // Path should be saved in lexer
      return tok;
    }

    char c = peek();

    // Check if the current character meets the criteria for a token
    if (std::isalpha(c)) {
      return makeSymbol();
    } else if (std::isdigit(c)) {
      return makeNumberLiteral();
    } else if (c == '"') {
      return makeStringLiteral();
    } else if (c == '\'') {
      return makeCharLiteral();
    } else if (isPunctuator(c)) {
      return makePunctuator();
    } else {
      return lexerError(LexerErrorType::UNKNOWN_TOKEN);
    }
  }

  Lexer::LexerResult Lexer::makeSymbol() {
    uint64_t lexeme_start = pos;
    uint64_t lexeme_end =
        peekWhile([](char c) { return isalnum(c) || c == '_'; });

    if (lexeme_end - lexeme_start > 256) {
      // Handle lexer error when identifier exceeds length
      // Throw lexer error or return an unknown token
      return lexerError(LexerErrorType::INVALID_IDENTIFIER_LENGTH);
    }

    std::string_view lexeme(source.data() + lexeme_start,
                            lexeme_end - lexeme_start);

    // Try to cast it to a keyword. If it fails, it will return an unknown
    // keword meaning that it is an identifier.
    Keyword kw = KeywordHandler::from(lexeme);

    // If the lexeme is not a kewword, return it as an identifier
    if (kw == Keyword::UNDEFINED) {
      Token tok{TokenType::IDENTIFIER};
      tok.value.identifier.name = lexeme;
      return tok;
    }

    // If the keyword is a boolean literal, return it as a literal
    if (kw == Keyword::TRUE) {
      Token tok{TokenType::BOOLEAN_LITERAL};
      tok.value.literal.boolean = true;
      return tok;
    } else if (kw == Keyword::FALSE) {
      Token tok{TokenType::BOOLEAN_LITERAL};
      tok.value.literal.boolean = false;
      return tok;
    }

    Token tok{TokenType::KEYWORD};
    tok.value.keyword = kw;
    return tok;
    // return Token{TokenType::KEYWORD, kw};
  }

  Lexer::LexerResult Lexer::makeNumberLiteral() {
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

    // Handle floating-point numbers (only for decimal)
    if (base == 10 && peek() == '.') {
      next();  // Consume '.'
      uint64_t lexeme_end = peekWhile([](char c) { return (bool)isdigit(c); });
      std::string_view float_literal(source.data() + lexeme_start,
                                     lexeme_end - lexeme_start);

      auto num_result = toFloat(float_literal);
      if (!num_result) return std::unexpected(num_result.error());

      Token tok{TokenType::FLOATING_LITERAL};
      tok.value.literal.integer = *num_result;
      return tok;
    }

    // Capture numeric part
    uint64_t lexeme_end = peekWhile([](char c) { return (bool)isalnum(c); });
    std::string_view number_literal(source.data() + lexeme_start,
                                    lexeme_end - lexeme_start);

    // Validate number matches base rules
    if (number_literal.empty()) {
      return lexerError(LexerErrorType::UNEXPECTED_RADIX_PREFIX);
    }

    // Validate if the number of different base than 10 is valid
    for (char c : number_literal) {
      if (!isValidBaseNumber(c, base)) {
        return lexerError(LexerErrorType::UNEXPECTED_RADIX_PREFIX);
      }
    }

    // Convert integer based on detected base
    auto num_result = toInt(number_literal, base);
    if (!num_result) return std::unexpected(num_result.error());

    Token tok{TokenType::INTEGER_LITERAL};
    tok.value.literal.integer = *num_result;
    return tok;
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
      return lexerError(LexerErrorType::UNCLOSED_STRING_LITERAL);
    }

    Token tok{TokenType::STRING_LITERAL};
    tok.value.literal.string = string_literal;
    return tok;
  }

  Lexer::LexerResult Lexer::makeCharLiteral() {
    // consume starting '
    next();

    if (peek() == '\'') {
      return lexerError(LexerErrorType::INVALID_CHAR_LITERAL);
    }

    // consume and cache the char
    char c = next();

    if (peek() != '\'') {
      return lexerError(LexerErrorType::MULTI_CHAR_CHAR_LITERAL);
    }

    // consume last '
    next();

    Token tok{TokenType::CHAR_LITERAL};
    tok.value.literal.character = c;
    return tok;
  }

  Lexer::LexerResult Lexer::makePunctuator() {
    uint64_t punc_start = pos;
    uint64_t punc_end = peekWhile([this](char p) { return isPunctuator(p); });

    std::string_view punc_view(source.data() + punc_start,
                               punc_end - punc_start);

    bool is_comment = false;

    // If the longest punctuator is not valid, break it into small punctuators
    // until single punctuator.
    while (!punc_view.empty()) {
      Punctuator punc = PunctuatorHandler::from(punc_view);

      // If the punctuator is a comment, skip it
      if (punc == Punctuator::LINE_COMMENT) {
        skipLineComment();
        is_comment = true;
      }

      // If the punctuator is a block comment, skip it
      if (punc == Punctuator::LBLOCK_COMMENT) {
        skipBlockComments();
        is_comment = true;
      }

      // Return error if block comment doesnt close correctly
      if (punc == Punctuator::RBLOCK_COMMENT) {
        return lexerError(LexerErrorType::UNCLOSED_BLOCK_COMMENT);
      }

      // If the sub-longest punctuator is valid, return it
      if (punc != Punctuator::UNKNOWN) {
        Token tok{TokenType::PUNCTUATOR};
        tok.value.punctuator = punc;
        return tok;
      }

      // Move position back removing sufix
      pos--;
      punc_view.remove_suffix(1);
    }

    // If no punctuator is matched, return unknown token
    return lexerError(LexerErrorType::UNKNOWN_PUNCTUATOR);
  }

  char Lexer::next() noexcept {
    // Move the position to the next character in the source code
    // and return the character at the new position. This consumes
    // the character at the current position.
    return (pos < source.length()) ? source[pos++] : '\0';
  }

  char Lexer::peek() const noexcept {
    // Return the character at the current position in the source code
    // This does not consume the character at the current position.
    return (pos < source.length()) ? source[pos] : '\0';
  }

  char Lexer::peekNext() const noexcept {
    // Return the character at the next position in the source code
    // This does not consume the character at the current position.
    return (pos + 1 < source.length()) ? source[pos + 1] : '\0';
  }

  void Lexer::skipWhitespace() noexcept {
    // Skip all whitespace characters until we reach a non-whitespace
    // character or the end of the source code.
    while (peek() != '\0' && isWhitespace(peek())) {
      if (peek() == '\n') line++;
      pos++;
    }
  }

  void Lexer::skipLineComment() noexcept {
    // Skip all characters before new line hits
    while (peek() != '\0' && peek() != '\n') pos++;
    line++;
  }

  void Lexer::skipBlockComments() noexcept {
    // Set to 1 since we expect to have already consumed the first '/*'
    uint32_t nested_count = 1;

    // Move the position until the opening and closing of block comments is
    // balanced.
    while (peek() != '\0' && nested_count != 0) {
      bool opening = peek() == '/' && peekNext() == '*';
      bool closing = peek() == '*' && peekNext() == '/';

      if (opening) {
        nested_count++;
        pos += 1;
      }
      if (closing) {
        nested_count--;
        pos += 1;
      }

      if (peek() == '\n') line++;
      pos++;
    }
  }

  bool Lexer::isWhitespace(char c) const noexcept {
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

  bool Lexer::isPunctuator(char c) const noexcept {
    switch (c) {
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
      case '_':
        return true;

      default:
        return false;
    }
  }

  bool Lexer::isEscapedChar(char c) const noexcept {
    return c == 'n' || c == 'r' || c == '"' || c == '\'' || c == '0' ||
           c == '\\';
  }

  bool Lexer::isValidBaseNumber(char c, uint8_t base) const noexcept {
    switch (base) {
      case 2:
        return c == '0' || c == '1';
      case 8:
        return c >= '0' && c <= '7';
      case 10:
        return (bool)isdigit(c);
      case 16:
        return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
      default:
        return false;  // Invalid base (should never happen)
    }
  }

  char Lexer::toEscapedChar(char c) const noexcept {
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

  Lexer::DParseResult Lexer::toFloat(std::string_view float_literal) const {
    double result;
    auto [ptr, ec] =
        std::from_chars(float_literal.data(),
                        float_literal.data() + float_literal.size(), result);
    if (ec != std::errc()) {
      return lexerError(LexerErrorType::INVALID_FLOAT_LITERAL);
    }
    return result;
  }

  Lexer::IParseResult Lexer::toInt(std::string_view integer_literal,
                                   uint8_t base) const {
    uint64_t result = 0;
    auto [ptr, ec] = std::from_chars(
        integer_literal.data(), integer_literal.data() + integer_literal.size(),
        result, base);
    if (ec != std::errc()) {
      return lexerError(LexerErrorType::INVALID_NUMBER_LITERAL);
    }
    return result;
  }
}  // namespace compiler