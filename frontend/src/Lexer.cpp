#include "Lexer.hpp"

namespace compiler {

  bool Lexer::isOperator(char c) {
    return std::string("+-*/%=!<>|&^").find(c) != std::string::npos;
  }

  bool Lexer::isSymbol(char c) {
    return std::string("(){}[],;").find(c) != std::string::npos;
  }

  char Lexer::peek() { return (pos < source.size()) ? source[pos] : '\0'; }

  char Lexer::advance() { return (pos < source.size()) ? source[pos++] : '\0'; }

  void Lexer::skipWhitespace() {
    while (isspace(peek())) advance();
  }

  Token Lexer::makeIdentifier() {
    std::string value;
    while (isalnum(peek()) || peek() == '_') {
      value += advance();
    }
    return {keywords.count(value) ? TokenType::KEYWORD : TokenType::IDENTIFIER,
            value};
  }

  Token Lexer::makeNumber() {
    std::string value;
    while (isdigit(peek())) {
      value += advance();
    }
    return {TokenType::NUMBER, value};
  }

  Token Lexer::makeOperator() {
    std::string value;
    value += advance();
    if (isOperator(peek()))
      value += advance();  // Handle two-character operators like '==', '&&'
    return {TokenType::OPERATOR, value};
  }

  Token Lexer::makeString() {
    std::string value;
    advance();  // Skip opening quote
    while (peek() != '"' && peek() != '\0') {
      value += advance();
    }
    advance();  // Skip closing quote
    return {TokenType::STRING, value};
  }

  Token Lexer::makeComment() {
    std::string value;
    advance();  // Skip first '/'
    if (peek() == '/') {
      while (peek() != '\n' && peek() != '\0') {
        value += advance();
      }
      return {TokenType::COMMENT, value};
    }
    return {TokenType::UNKNOWN, "/"};
  }

  Lexer::Lexer(const std::string& src) : source(src) {}

  std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (pos < source.size()) {
      skipWhitespace();
      char c = peek();

      if (isalpha(c) || c == '_') {
        tokens.push_back(makeIdentifier());
      } else if (isdigit(c)) {
        tokens.push_back(makeNumber());
      } else if (isOperator(c)) {
        tokens.push_back(makeOperator());
      } else if (isSymbol(c)) {
        tokens.push_back({TokenType::SYMBOL, std::string(1, advance())});
      } else if (c == '"') {
        tokens.push_back(makeString());
      } else if (c == '/') {
        tokens.push_back(makeComment());
      } else {
        tokens.push_back({TokenType::UNKNOWN, std::string(1, advance())});
      }
    }

    tokens.push_back({TokenType::ENDOF, ""});
    return tokens;
  }
}  // namespace compiler