#include <cctype>
#include <iostream>
#include <unordered_set>
#include <vector>

enum class TokenType {
  FIRST,
  KEYWORD,
  IDENTIFIER,
  NUMBER,
  OPERATOR,
  SYMBOL,
  STRING,
  COMMENT,
  UNKNOWN,
  ENDOF,
};

struct Token {
  TokenType type;
  std::string value;
};

class LexerError {};

class Lexer {
private:
  std::string path;  // Defines the path of where is working
  std::string source;
  size_t pos = 0;   // Horizontal line position
  size_t line = 0;  // Vertical line position

  std::unordered_set<std::string> keywords = {
      "int",   "float",  "double", "char",  "if",     "else",   "for",
      "while", "return", "void",   "class", "struct", "public", "private"};

  bool isOperator(char c) {
    return std::string("+-*/%=!<>|&^").find(c) != std::string::npos;
  }

  bool isSymbol(char c) {
    return std::string("(){}[],;").find(c) != std::string::npos;
  }

  char peek() { return (pos < source.size()) ? source[pos] : '\0'; }

  char advance() { return (pos < source.size()) ? source[pos++] : '\0'; }

  void skipWhitespace() {
    while (isspace(peek())) advance();
  }

  Token makeIdentifier() {
    std::string value;
    while (isalnum(peek()) || peek() == '_') {
      value += advance();
    }
    return {keywords.count(value) ? TokenType::KEYWORD : TokenType::IDENTIFIER,
            value};
  }

  Token makeNumber() {
    std::string value;
    while (isdigit(peek())) {
      value += advance();
    }
    return {TokenType::NUMBER, value};
  }

  Token makeOperator() {
    std::string value;
    value += advance();
    if (isOperator(peek()))
      value += advance();  // Handle two-character operators like '==', '&&'
    return {TokenType::OPERATOR, value};
  }

  Token makeString() {
    std::string value;
    advance();  // Skip opening quote
    while (peek() != '"' && peek() != '\0') {
      value += advance();
    }
    advance();  // Skip closing quote
    return {TokenType::STRING, value};
  }

  Token makeComment() {
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

public:
  Lexer(const std::string& src) : source(src) {}

  std::vector<Token> tokenize() {
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
};

int main() {
  std::string code = R"(
        int main() {
            int x = 10;
            if (x > 5) {
                x = x + 1;
            }
            return 0;
        }
    )";

  Lexer lexer(code);
  std::vector<Token> tokens = lexer.tokenize();

  for (const auto& token : tokens) {
    std::cout << "Token: " << static_cast<int>(token.type) << " -> "
              << token.value << "\n";
  }

  return 0;
}
