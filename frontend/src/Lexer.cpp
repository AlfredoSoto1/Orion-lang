#include "Lexer.hpp"

namespace compiler {

  Lexer::Lexer(const std::string& src) : source(src) {}

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

  Token Lexer::advance() {
    // Skip all whitespace characters until we reach a non-whitespace
    // character or the end of the source code.
    skipWhitespace();

    // If we are at the end of the source code, return an EOF token
    if (pos >= source.length()) {
      EndOfFile eof = {0, line, "INSERT PATH HERE (DONT FORGET THE COLUMN)"};
      return {TokenType::ENDOF, eof};
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
      return {TokenType::UNKNOWN, (uint8_t)TokenType::UNKNOWN};
    }
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

  Token Lexer::makeIdentifierOrKeyword() {
    char lexeme[256];
    unsigned char lexeme_ptr = 0;

    // Append all characters that are alphanumeric into lexeme
    while (isalnum(peek()) && lexeme_ptr < 256) {
      lexeme[lexeme_ptr++] = next();
    }

    lexeme[lexeme_ptr] = '\0';  // Ensure null-termination

    if (lexeme_ptr == 255) {
      // Handle lexer error when identifier exceeds length
      // Throw lexer error or return an unknown token
      return {TokenType::UNKNOWN, (uint8_t)TokenType::UNKNOWN};
    }

    Keyword kw = KeywordHandler::from(lexeme);

    // If the lexeme is not a kewword, return it as an identifier
    if (kw == Keyword::UNDEFINED) {
      Identifier id = {unique_hash++, lexeme};
      return {TokenType::IDENTIFIER, id};
    }

    return {TokenType::KEYWORD, kw};
  }

  Token Lexer::makeStringLiteral() {}

  Token Lexer::makeNumericLiteral() {
    char lexeme[25];
    unsigned char lexeme_ptr = 0;

    // Append all characters that are alphanumeric into lexeme
    while (isalnum(peek()) && lexeme_ptr < 25) {
      lexeme[lexeme_ptr++] = next();
    }

    lexeme[lexeme_ptr] = '\0';  // Ensure null-termination

    if (lexeme_ptr == 25) {
      // Handle lexer error when identifier exceeds length
      // Throw lexer error or return an unknown token
      return {TokenType::UNKNOWN, (uint8_t)TokenType::UNKNOWN};
    }

    return {TokenType::UNKNOWN, 0};
  }

  Token Lexer::makeSpecialPunc() {}

}  // namespace compiler

// namespace compiler {

//   bool Lexer::isOperator(char c) {
//     return std::string("+-*/%=!<>|&^").find(c) != std::string::npos;
//   }

//   bool Lexer::isSymbol(char c) {
//     return std::string("(){}[],;").find(c) != std::string::npos;
//   }

//   char Lexer::peek() { return (pos < source.size()) ? source[pos] : '\0';
//   }

//   char Lexer::advance() { return (pos < source.size()) ? source[pos++] :
//   '\0'; }

//   void Lexer::skipWhitespace() {
//     while (isspace(peek())) advance();
//   }

//   Token Lexer::makeIdentifier() {
//     std::string value;
//     while (isalnum(peek()) || peek() == '_') {
//       value += advance();
//     }
//     return {keywords.count(value) ? TokenType::KEYWORD :
//     TokenType::IDENTIFIER,
//             value};
//   }

//   Token Lexer::makeNumber() {
//     std::string value;
//     while (isdigit(peek())) {
//       value += advance();
//     }
//     return {TokenType::NUMBER, value};
//   }

//   Token Lexer::makeOperator() {
//     std::string value;
//     value += advance();
//     if (isOperator(peek()))
//       value += advance();  // Handle two-character operators like '==',
//       '&&'
//     return {TokenType::OPERATOR, value};
//   }

//   Token Lexer::makeString() {
//     std::string value;
//     advance();  // Skip opening quote
//     while (peek() != '"' && peek() != '\0') {
//       value += advance();
//     }
//     advance();  // Skip closing quote
//     return {TokenType::STRING, value};
//   }

//   Token Lexer::makeComment() {
//     std::string value;
//     advance();  // Skip first '/'
//     if (peek() == '/') {
//       while (peek() != '\n' && peek() != '\0') {
//         value += advance();
//       }
//       return {TokenType::COMMENT, value};
//     }
//     return {TokenType::UNKNOWN, "/"};
//   }

//   Lexer::Lexer(const std::string& src) : source(src) {}

//   std::vector<Token> Lexer::tokenize() {
//     std::vector<Token> tokens;

//     while (pos < source.size()) {
//       skipWhitespace();
//       char c = peek();

//       if (isalpha(c) || c == '_') {
//         tokens.push_back(makeIdentifier());
//       } else if (isdigit(c)) {
//         tokens.push_back(makeNumber());
//       } else if (isOperator(c)) {
//         tokens.push_back(makeOperator());
//       } else if (isSymbol(c)) {
//         tokens.push_back({TokenType::SYMBOL, std::string(1, advance())});
//       } else if (c == '"') {
//         tokens.push_back(makeString());
//       } else if (c == '/') {
//         tokens.push_back(makeComment());
//       } else {
//         tokens.push_back({TokenType::UNKNOWN, std::string(1,
//         advance())});
//       }
//     }

//     tokens.push_back({TokenType::ENDOF, ""});
//     return tokens;
//   }
// }  // namespace compiler