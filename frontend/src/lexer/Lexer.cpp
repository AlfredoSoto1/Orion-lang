#include "Lexer.hpp"

#include <cctype>
#include <charconv>
#include <cmath>
#include <iostream>

/**
 * @brief Expands into an unexpected lexer error
 *
 */
#define lexerError(type) std::unexpected(LexerError{state(), type})

namespace compiler {

  Lexer::Lexer(std::string_view filename, std::string_view src) noexcept
      : pos(0),              // Current position in the source code
        line(0),             // Current line number
        line_pos(0),         // Current position within the line
        prev_pos(0),         // Previous position in the source code
        prev_line(0),        // Previous line number
        prev_line_pos(0),    // Previous position within the line
        token_start_pos(0),  // Start position of the current token
        source(src),
        last_token(Token::endOF()),
        filename(filename) {}

  LexerState Lexer::state() const noexcept {
    size_t column = (token_start_pos - prev_line_pos) + 1;  // 1-based column

    size_t line_end = source.find('\n', prev_line_pos);
    if (line_end == std::string::npos) line_end = source.size();

    std::string_view line_text =
        source.substr(prev_line_pos, line_end - prev_line_pos);

    return LexerState{prev_line, column, filename, line_text, last_token};
  }

  Lexer::LexerResult Lexer::advance() {
    // Skip all whitespace characters until we reach a non-whitespace
    // character or the end of the source code.
    skipWhitespace();

    // If we are at the end of the source code, return an EOF token
    if (pos >= source.length()) {
      return Token::endOF();
    }

    // Save the starting position of the token
    token_start_pos = pos;

    char c = peek();

    LexerResult result;

    // Check if the current character meets the criteria for a token
    if (std::isalpha(c)) {
      result = makeSymbol();
    } else if (std::isdigit(c)) {
      result = makeNumberLiteral();
    } else if (c == '"') {
      result = makeStringLiteral();
    } else if (c == '\'') {
      result = makeCharLiteral();
    } else if (isBasicPunc(c)) {
      result = makePunctuator();
    } else {
      result = lexerError(LexerErrorType::UNKNOWN_TOKEN);
    }

    // Save a copy of the last token
    last_token = *result;

    return result;
  }

  Lexer::LexerResult Lexer::makeSymbol() {
    size_t lex_start = pos;
    size_t lex_end = nextWhile([](char c) { return isalnum(c) || c == '_'; });

    // Handle lexer error when identifier exceeds length
    if (lex_end - lex_start > std::numeric_limits<uint16_t>::max()) {
      return lexerError(LexerErrorType::INVALID_IDENTIFIER_LENGTH);
    }

    std::string_view lexeme(source.data() + lex_start, lex_end - lex_start);

    // Try to cast it to a keyword. If it fails, it will return an unknown
    // keword meaning that it is an identifier.
    Keyword kw = KeywordHandler::from(lexeme);

    // If the lexeme is not a kewword, return it as an identifier
    if (kw == Keyword::UNDEFINED) {
      return Token::identifier(lex_start, lex_end);
    }

    // If the keyword is a boolean literal, return it as a literal
    if (kw == Keyword::TRUE) {
      return Token::boolean(true);
    } else if (kw == Keyword::FALSE) {
      return Token::boolean(false);
    }

    return Token::keyword(kw);
  }

  Lexer::LexerResult Lexer::makeNumberLiteral() {
    size_t num_start = token_start_pos = pos;
    const uint8_t base = basePrefixFrom(num_start);

    // Capture numeric part after prefix
    size_t int_end = nextWhile([](char c) { return (bool)isalnum(c); });
    std::string_view int_part(source.data() + num_start, int_end - num_start);

    // Handle floating-point numbers (only for decimal)
    if (base == 10 && peek() == '.') {
      next();  // Consume '.'
      size_t float_end = nextWhile([](char c) { return (bool)isdigit(c); });
      std::string_view float_literal(source.data() + num_start,
                                     float_end - num_start);

      if (float_literal.empty()) {
        return lexerError(LexerErrorType::UNEXPECTED_RADIX_SUFIX);
      }

      auto num = toFloat(float_literal);
      if (!num) return std::unexpected(num.error());

      // Obtain token type from sufix (if any)
      TokenType type = typeSufixFrom();

      if (type == TokenType::ENDOF) {
        return lexerError(LexerErrorType::UNEXPECTED_RADIX_SUFIX);
      }

      if (type == TokenType::INT32_LITERAL) {
        type = TokenType::FLOAT32_LITERAL;
      }

      return Token::number(type, *reinterpret_cast<uint64_t*>(&*num));
    }

    // Validate number matches base rules
    if (int_part.empty()) {
      return lexerError(LexerErrorType::UNEXPECTED_RADIX_PREFIX);
    }

    // Validate if the number of different base than 10 is valid
    for (char c : int_part) {
      if (!isValidBaseNumber(c, base)) {
        return lexerError(LexerErrorType::UNEXPECTED_RADIX_PREFIX);
      }
    }

    TokenType type = typeSufixFrom();
    if (type == TokenType::ENDOF) {
      return lexerError(LexerErrorType::UNEXPECTED_RADIX_SUFIX);
    }

    // Convert integer based on detected base
    auto num_result = toInt(int_part, base);
    if (!num_result) return std::unexpected(num_result.error());

    if (type == TokenType::FLOAT32_LITERAL ||
        type == TokenType::FLOAT64_LITERAL) {
      return Token::number(type, *reinterpret_cast<uint64_t*>(&*num_result));
    } else {
      return Token::number(type, *num_result);
    }
  }

  Lexer::LexerResult Lexer::makeStringLiteral() {
    next();  // consume starting "

    string_lit literal{.start = (uint32_t)pos, .end = (uint32_t)pos};
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

      literal.end++;
    }

    if (!balanced) {
      return lexerError(LexerErrorType::UNCLOSED_STRING_LITERAL);
    }

    return Token::string(literal);
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

    return Token::character(c);
  }

  Lexer::LexerResult Lexer::makePunctuator() {
    size_t lex_start = pos;
    size_t lex_end = nextWhile([this](char p) { return isBasicPunc(p); });

    std::string_view punc_view(source.data() + lex_start, lex_end - lex_start);

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
        return Token::punctuator(punc);
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
      if (peek() == '\n') {
        prev_line = line;
        line++;
        prev_line_pos = line_pos;
        line_pos = pos + 1;
      }
      prev_pos = pos;
      pos++;
    }
  }

  void Lexer::skipLineComment() noexcept {
    // Skip all characters before new line hits
    while (peek() != '\0' && peek() != '\n') {
      prev_pos = pos;
      pos++;
    }
    prev_line = line;
    line++;
    prev_line_pos = line_pos;
    line_pos = pos + 1;
  }

  void Lexer::skipBlockComments() noexcept {
    // Set to 1 since we expect to have already consumed the first '/*'
    size_t nested_count = 1;

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

      if (peek() == '\n') {
        prev_line = line;
        line++;
        prev_line_pos = line_pos;
        line_pos = pos + 1;
      }
      prev_pos = pos;
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

  bool Lexer::isBasicPunc(char c) const noexcept {
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
      case 16:
        return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
      default:
        return true;
    }
  }

  TokenType Lexer::typeSufixFrom() noexcept {
    size_t sufix_end = pos;
    size_t sufix_start = pos - 2;
    std::string_view sufix(source.data() + sufix_start,
                           sufix_end - sufix_start);

    while (!sufix.empty() && !isalpha(sufix.at(0))) {
      sufix.remove_prefix(1);
    }

    if (sufix.empty()) {
      return TokenType::INT32_LITERAL;
    }

    if (sufix == "u" || sufix == "U") {
      return TokenType::UINT32_LITERAL;
    }

    if (sufix == "ul" || sufix == "UL") {
      return TokenType::UINT64_LITERAL;
    }

    if (sufix == "i" || sufix == "I") {
      return TokenType::INT32_LITERAL;
    }

    if (sufix == "il" || sufix == "IL") {
      return TokenType::INT64_LITERAL;
    }

    if (sufix == "f" || sufix == "F") {
      return TokenType::FLOAT32_LITERAL;
    }

    if (sufix == "l" || sufix == "L") {
      return TokenType::FLOAT64_LITERAL;
    }

    return TokenType::ENDOF;
  }

  uint8_t Lexer::basePrefixFrom(size_t& lex_start) noexcept {
    // If number doesnt have a prefix, default base is 10
    if (peek() != '0') {
      return 10;
    }

    char next_char = peekNext();

    // Consume 0x
    if (next_char == 'x' || next_char == 'X') {
      next();
      next();
      lex_start += 2;
      return 16;
    }

    // Consume 0b
    if (next_char == 'b' || next_char == 'B') {
      next();
      next();
      lex_start += 2;
      return 2;
    }

    // Consume 0
    if (isdigit(next_char)) {
      next();
      lex_start += 1;
      return 8;
    }

    // Default case is 10
    return 10;
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