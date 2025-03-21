#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "Tokens.hpp"

namespace compiler {

  class LexerError {};

  class TokenStream {
  public:
    const Token& next();
    const Token& current() const;

    bool hasNext() const;
  };

  class Lexer {
  public:
    Lexer(const std::string& src);
    std::vector<Token> tokenize();

    TokenStream stream();

  private:
    std::unordered_set<std::string> keywords = {
        "int",   "float",  "double", "char",  "if",     "else",   "for",
        "while", "return", "void",   "class", "struct", "public", "private"};

    bool isSymbol(char c);
    bool isOperator(char c);
    bool isDigit(char c);
    bool isAlphaNumeric(char c);
    bool isPunctuator(char c);

    bool isEscaped(char c);
    bool isWhitespace(char c);
    bool isWhitespaceOrNewline(char c);

    Token advance();
    Token peekNextToken() const;
    Token peekNextNextToken() const;

    void skipWhitespace();

    Token makeKeyword();
    Token makeIdentifier();
    Token makeStringLiteral();
    Token makeNumericLiteral();
    Token makePunctuator();
    Token makeComment();
    Token makeOperator();
  };

}  // namespace compiler
